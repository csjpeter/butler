/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editpartnerview.h"

SCC TidContext = "EditPartnerView";

SCC TidNewPartnerWindowTitle = QT_TRANSLATE_NOOP("EditPartnerView", "Add new partner");
SCC TidEditPartnerWindowTitle = QT_TRANSLATE_NOOP("EditPartnerView", "Editing a partner");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditPartnerView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditPartnerView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditPartnerView", "Previous partner");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditPartnerView", "Next partner");

SCC TidPartnerName = QT_TRANSLATE_NOOP("EditPartnerView", "Partner name:");
SCC TidPartnerCountry = QT_TRANSLATE_NOOP("EditPartnerView", "Country:");
SCC TidPartnerCity = QT_TRANSLATE_NOOP("EditPartnerView", "City:");
SCC TidPartnerPostalCode = QT_TRANSLATE_NOOP("EditPartnerView", "Postal code:");
SCC TidPartnerAddress = QT_TRANSLATE_NOOP("EditPartnerView", "Address:");
SCC TidPartnerCompany = QT_TRANSLATE_NOOP("EditPartnerView", "Company:");
SCC TidPartnerStoreName = QT_TRANSLATE_NOOP("EditPartnerView", "Store name:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditPartnerView", "Please fill the partner name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditPartnerView", "Partner is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditPartnerView", "Partner is updated.");

EditPartnerView::EditPartnerView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(partnerModel(dbname)),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	companyEditor(&companyModel(dbname), Company::Name)
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
	connect(&storeNameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&countryEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&cityEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&postalCodeEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&addressEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&companyEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

void EditPartnerView::mapToGui()
{
	if(cursor.isValid())
		partner = Partner(model.data(cursor.row()));

	nameEditor.editor.setText(partner.name);
	storeNameEditor.editor.setText(partner.storeName);
	countryEditor.editor.setText(partner.country);
	cityEditor.editor.setText(partner.city);
	postalCodeEditor.editor.setText(partner.postalCode);
	addressEditor.editor.setText(partner.address);
	companyEditor.setText(partner.company);

	updateToolButtonStates();
}

void EditPartnerView::mapFromGui()
{
	partner.name = nameEditor.editor.text();
	partner.storeName = storeNameEditor.editor.text();
	partner.country = countryEditor.editor.text();
	partner.city = cityEditor.editor.text();
	partner.postalCode = postalCodeEditor.editor.text();
	partner.address = addressEditor.editor.text();
	partner.company = companyEditor.text();
}

void EditPartnerView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditPartnerWindowTitle));
	else
		setWindowTitle(tr(TidNewPartnerWindowTitle));

	nameEditor.label.setText(tr(TidPartnerName));
	storeNameEditor.label.setText(tr(TidPartnerStoreName));
	countryEditor.label.setText(tr(TidPartnerCountry));
	cityEditor.label.setText(tr(TidPartnerCity));
	postalCodeEditor.label.setText(tr(TidPartnerPostalCode));
	addressEditor.label.setText(tr(TidPartnerAddress));
	companyEditor.label.setText(tr(TidPartnerCompany));

	relayout();
}

void EditPartnerView::applyLayout()
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&countryEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&cityEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&postalCodeEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&addressEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&companyEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&storeNameEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditPartnerView::relayout()
{
	{
		nameEditor.wideLayout();
		storeNameEditor.wideLayout();
		countryEditor.wideLayout();
		cityEditor.wideLayout();
		postalCodeEditor.wideLayout();
		addressEditor.wideLayout();
		companyEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		storeNameEditor.narrowLayout();
		countryEditor.narrowLayout();
		cityEditor.narrowLayout();
		postalCodeEditor.narrowLayout();
		addressEditor.narrowLayout();
		companyEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditPartnerView::updateToolButtonStates()
{
	bool modified = !(
			partner.name == nameEditor.editor.text() &&
			partner.country == countryEditor.editor.text() &&
			partner.city == cityEditor.editor.text() &&
			partner.postalCode == postalCodeEditor.editor.text() &&
			partner.address == addressEditor.editor.text() &&
			partner.company == companyEditor.text() &&
			partner.storeName == storeNameEditor.editor.text()
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

void EditPartnerView::saveSlotSpec()
{
	/* Add company if not yet known. */
	CompanyModel & cm = companyModel(dbname);
	int i = cm.index(companyEditor.text());
	if(i == -1){
		Company company;
		company.name = companyEditor.text();
		cm.addNew(company);
	}
}

@include@ views.cpp
@declare@ Partner

@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent
@include@ setCursor prevSlot nextSlot saveSlot resetSlot

