/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_editpaymentview.h"

SCC TidContext = "EditPaymentView";

SCC TidNewPaymentWindowTitle = QT_TRANSLATE_NOOP("EditPaymentView", "Already bought new payment");
SCC TidEditPaymentWindowTitle = QT_TRANSLATE_NOOP("EditPaymentView", "Editing an existing payment");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditPaymentView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditPaymentView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditPaymentView", "Previous payment");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditPaymentView", "Next payment");

SCC TidAmountEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Amount:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Business partner:");
SCC TidAccountSelector = QT_TRANSLATE_NOOP("EditPaymentView", "Account (bank):");
SCC TidPayDateTimeEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Date of purchase:");
SCC TidUploadDateTimeEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Date of upload:");
SCC TidCommentEditor = QT_TRANSLATE_NOOP("EditPaymentView", "Comments:");

SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditPaymentView", "Payment is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditPaymentView", "Payment is updated.");

EditPaymentView * EditPaymentView::newPaymentViewFactory(const csjp::String & dbname)
{
	csjp::Object<PaymentModel> ownModel = paymentModel(dbname);
	EditPaymentView * view = new EditPaymentView(dbname, *ownModel);
	view->ownModel = ownModel.release();
	return view;
}

EditPaymentView::EditPaymentView(const csjp::String & dbname, PaymentModel & model, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(model),
	ownModel(NULL),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	accountEditor(&accountModel(dbname), Account::Name),
	partnerEditor(&partnerModel(dbname), Partner::Name)
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);
	
	payment.payDate = QDateTime::currentDateTime();
	uploadDateTime.setEnabled(false);

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
	
	connect(&accountEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&payDateTime.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&commentEditor.edit, SIGNAL(textChanged()),
			this, SLOT(updateToolButtonStates()));

	connect(&accountEditor.editor, SIGNAL(editingFinished()),
			this, SLOT(accountNameEditFinishedSlot()));
	connect(&accountEditor.box, SIGNAL(currentIndexChanged(int)),
			this, SLOT(accountNameEditFinishedSlot(int)));

	setupView();
	retranslate();
	loadState();
}

EditPaymentView::~EditPaymentView()
{
	if(ownModel)
		delete ownModel;
}

void EditPaymentView::showEvent(QShowEvent *event)
{
	lastAccountName.clear();
	mapToGui();
	if(!cursor.isValid())
		uploadDateTime.edit.setDateTime(QDateTime::currentDateTime());

	PannView::showEvent(event);
	amountEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditPaymentView::mapToGui()
{
	if(cursor.isValid()){
		payment = Payment(model.data(cursor.row()));
		accountEditor.setText(payment.partner);
		accountNameEditFinishedSlot();
		partnerEditor.setText(payment.partner);
	}

	uploadDateTime.edit.setDateTime(payment.uploadDate);

	commentEditor.edit.setText(payment.comment);

	payDateTime.edit.setDateTime(payment.payDate);

	amountEditor.blockSignals(true);
	amountEditor.setValue(payment.amount);
	amountEditor.blockSignals(false);

	updateToolButtonStates();
}

void EditPaymentView::mapFromGui()
{
	payment.uploadDate = uploadDateTime.edit.dateTime();

	payment.account = accountEditor.text();
	payment.partner = partnerEditor.text();
	payment.comment = commentEditor.edit.toPlainText();

	payment.amount = amountEditor.value();
	payment.payDate = payDateTime.edit.dateTime();
}

void EditPaymentView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditPaymentWindowTitle));
	else
		setWindowTitle(tr(TidNewPaymentWindowTitle));

	amountEditor.label.setText(tr(TidAmountEditor));
	accountEditor.label.setText(tr(TidAccountSelector));
	partnerEditor.label.setText(tr(TidPartnerSelector));
	payDateTime.label.setText(tr(TidPayDateTimeEditor));
	uploadDateTime.label.setText(tr(TidUploadDateTimeEditor));
	commentEditor.label.setText(tr(TidCommentEditor));

	relayout();
}

void EditPaymentView::applyLayout(bool test)
{
	HLayout * hlayout = new HLayout;
	hlayout->addWidget(&amountEditor);

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addLayout(hlayout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&accountEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&payDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&uploadDateTime);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&commentEditor);
	mainLayout->addStretch(0);
	(void)test;

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditPaymentView::relayout()
{
	{
		amountEditor.wideLayout();
		accountEditor.wideLayout();
		partnerEditor.wideLayout();
		payDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout( true);
	}
	if(width() < sizeHint().width()){
		amountEditor.narrowLayout();
		accountEditor.wideLayout();
		partnerEditor.wideLayout();
		payDateTime.wideLayout();
		uploadDateTime.wideLayout();
		commentEditor.wideLayout();
		applyLayout(true);
	}
	if(width() < sizeHint().width()){
		amountEditor.narrowLayout();
		accountEditor.narrowLayout();
		partnerEditor.narrowLayout();
		payDateTime.narrowLayout();
		uploadDateTime.narrowLayout();
		commentEditor.narrowLayout();
		applyLayout(true);
	}

	applyLayout();
	updateToolButtonStates();
}

void EditPaymentView::updateToolButtonStates()
{
	bool modified = !(
			payment.account == accountEditor.text() &&
			payment.partner == partnerEditor.text() &&
			payment.comment == commentEditor.edit.toPlainText() &&
			(payment.amount - amountEditor.value()).abs() < 0.01 &&
			payment.payDate == payDateTime.edit.dateTime()
			);

	footerBar.show(); /* We cant set visible status for a widget having hidden parent. */
	prevButton.setVisible(!modified && cursor.isValid() && 0 < cursor.row());
	nextButton.setVisible(!modified && cursor.isValid() && cursor.row() < model.rowCount()-1);
	doneButton.setVisible(modified);
	resetButton.setVisible(modified);

	if(modified)
			toolBar.clearInfo();

	toolBar.updateButtons();
	footerBar.updateButtons();
}

void EditPaymentView::saveSlotSpec()
{
	int i;

	/* Add partner if not yet known. */
	PartnerModel & sm = partnerModel(dbname);
	i = sm.index(partnerEditor.text());
	if(i == -1){
		Partner partner;
		partner.name = partnerEditor.text();
		partner.company = partnerEditor.text();
		/* Add company if not yet known. */
		CompanyModel & cm = companyModel(dbname);
		int i = cm.index(partner.company);
		if(i == -1){
			Company company;
			company.name = partner.company;
			cm.addNew(company);
		}
		sm.addNew(partner);
	}

	/* Add account if not yet known. */
	AccountModel & am = accountModel(dbname);
	i = am.index(accountEditor.text());
	if(i == -1){
		Account account;
		account.name = accountEditor.text();
		am.addNew(account);
	}
}

void EditPaymentView::resetSlot()
{
	mapToGui();
}

void EditPaymentView::accountNameEditFinishedSlot()
{
	if(lastAccountName == accountEditor.editor.text())
		return;

	lastAccountName = accountEditor.editor.text();

	Account account;
	AccountModel & am = accountModel(dbname);
	int i = am.index(lastAccountName);
	if(i != -1)
		account = am.data(i);

	amountEditor.setSuffix(account.currency);
}

/* We need this for cases when keyboard focus was not used,
 * just a mouse selection from the drop down popup. */
void EditPaymentView::accountNameEditFinishedSlot(int)
{
	accountNameEditFinishedSlot();
}

@include@ views.cpp
@declare@ Payment

@include@ setCursor prevSlot nextSlot saveSlot
@include@ closeEvent loadState saveState changeEvent resizeEvent

