/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2014 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editaccountview.h"

@include@ views.cpp
@declare@ Account

SCC TidContext = "EditAccountView";

SCC TidNewAccountWindowTitle = QT_TRANSLATE_NOOP("EditAccountView", "Add new account");
SCC TidEditAccountWindowTitle = QT_TRANSLATE_NOOP("EditAccountView", "Editing a account");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditAccountView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditAccountView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditAccountView", "Previous account");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditAccountView", "Next account");

SCC TidAccountName = QT_TRANSLATE_NOOP("EditAccountView", "Account name:");
SCC TidAccountCurrency = QT_TRANSLATE_NOOP("EditAccountView", "Currency:");
SCC TidAccountIban = QT_TRANSLATE_NOOP("EditAccountView", "IBAN:");
SCC TidAccountSwiftCode = QT_TRANSLATE_NOOP("EditAccountView", "Swift code:");
SCC TidAccountBankOffice = QT_TRANSLATE_NOOP("EditAccountView", "Bank office:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditAccountView", "Please fill the account name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditAccountView", "Account is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditAccountView", "Account is updated.");

EditAccountView::EditAccountView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(accountModel(dbname)),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	partnerEditor(&partnerModel(dbname), Partner::Name)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);

	toolBar.addToolWidget(doneButton);
	toolBar.addToolWidget(resetButton);
	footerBar.addToolWidget(prevButton);
	footerBar.addToolWidget(nextButton);
	footerBar.expanding = true;
	footerBar.spacerEnd = false;

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextSlot()));

	connect(&nameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&currencyEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&ibanEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&swiftCodeEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent

void EditAccountView::mapToGui()
{
	if(cursor.isValid())
		account = Account(model.data(cursor.row()));

	nameEditor.editor.setText(account.name);
	currencyEditor.editor.setText(account.currency);
	ibanEditor.editor.setText(account.iban);
	swiftCodeEditor.editor.setText(account.swiftCode);
	partnerEditor.setText(account.bankOffice);

	updateToolButtonStates();
}

void EditAccountView::mapFromGui()
{
	account.name = nameEditor.editor.text();
	account.currency = currencyEditor.editor.text();
	account.iban = ibanEditor.editor.text();
	account.swiftCode = swiftCodeEditor.editor.text();
	account.bankOffice = partnerEditor.text();
}

void EditAccountView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditAccountWindowTitle));
	else
		setWindowTitle(tr(TidNewAccountWindowTitle));

	nameEditor.label.setText(tr(TidAccountName));
	currencyEditor.label.setText(tr(TidAccountCurrency));
	ibanEditor.label.setText(tr(TidAccountIban));
	swiftCodeEditor.label.setText(tr(TidAccountSwiftCode));
	partnerEditor.label.setText(tr(TidAccountBankOffice));

	relayout();
}

void EditAccountView::applyLayout()
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&currencyEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&ibanEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&swiftCodeEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditAccountView::relayout()
{
	{
		nameEditor.wideLayout();
		currencyEditor.wideLayout();
		ibanEditor.wideLayout();
		swiftCodeEditor.wideLayout();
		partnerEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		currencyEditor.narrowLayout();
		ibanEditor.narrowLayout();
		swiftCodeEditor.narrowLayout();
		partnerEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditAccountView::updateToolButtonStates()
{
	bool modified = !(
			account.name == nameEditor.editor.text() &&
			account.currency == currencyEditor.editor.text() &&
			account.iban == ibanEditor.editor.text() &&
			account.swiftCode == swiftCodeEditor.editor.text() &&
			account.bankOffice == partnerEditor.text()
			);

	bool mandatoriesGiven = nameEditor.editor.text().size();

	footerBar.show(); /* We cant set visible status for a widget having hidden parent. */
	prevButton.setVisible(!modified && cursor.isValid() && 0 < cursor.row());
	nextButton.setVisible(!modified && cursor.isValid() && cursor.row() < model.rowCount()-1);
	doneButton.setVisible(mandatoriesGiven && modified);
	resetButton.setVisible(modified);

	if(modified){
		if(!mandatoriesGiven)
			toolBar.setInfo(tr(TidInfoMandatoryFields));
		else
			toolBar.clearInfo();
	}

	toolBar.updateButtons();
	footerBar.updateButtons();
}

void EditAccountView::saveSlotSpec()
{
	/* Add partner if not yet known. */
	PartnerModel & pm = partnerModel(dbname);
	int i = pm.index(partnerEditor.text());
	if(i == -1){
		Partner partner;
		partner.name = partnerEditor.text();
		pm.addNew(partner);
	}
}

@include@ setCursor prevSlot nextSlot saveSlot resetSlot

