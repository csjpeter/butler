/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editcompanyview.h"

@include@ views.cpp
@declare@ Company

SCC TidContext = "EditCompanyView";

SCC TidNewCompanyWindowTitle = QT_TRANSLATE_NOOP("EditCompanyView", "Add new company");
SCC TidEditCompanyWindowTitle = QT_TRANSLATE_NOOP("EditCompanyView", "Editing a company");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditCompanyView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditCompanyView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditCompanyView", "Previous company");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditCompanyView", "Next company");

SCC TidCompanyName = QT_TRANSLATE_NOOP("EditCompanyView", "Company name:");
SCC TidCompanyCountry = QT_TRANSLATE_NOOP("EditCompanyView", "Country:");
SCC TidCompanyCity = QT_TRANSLATE_NOOP("EditCompanyView", "City:");
SCC TidCompanyPostalCode = QT_TRANSLATE_NOOP("EditCompanyView", "Postal code:");
SCC TidCompanyAddress = QT_TRANSLATE_NOOP("EditCompanyView", "Address:");
SCC TidCompanyTaxId = QT_TRANSLATE_NOOP("EditCompanyView", "Tax Id:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditCompanyView", "Please fill the company name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditCompanyView", "Company is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditCompanyView", "Company is updated.");

EditCompanyView::EditCompanyView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(companyModel(dbname)),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right))
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
	connect(&countryEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&cityEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&postalCodeEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&addressEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&taxIdEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent

void EditCompanyView::mapToGui()
{
	if(cursor.isValid())
		company = Company(model.data(cursor.row()));

	nameEditor.editor.setText(company.name);
	countryEditor.editor.setText(company.country);
	cityEditor.editor.setText(company.city);
	postalCodeEditor.editor.setText(company.postalCode);
	addressEditor.editor.setText(company.address);
	taxIdEditor.editor.setText(company.taxId);

	updateToolButtonStates();
}

void EditCompanyView::mapFromGui()
{
	company.name = nameEditor.editor.text();
	company.country = countryEditor.editor.text();
	company.city = cityEditor.editor.text();
	company.postalCode = postalCodeEditor.editor.text();
	company.address = addressEditor.editor.text();
	company.taxId = taxIdEditor.editor.text();
}

void EditCompanyView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditCompanyWindowTitle));
	else
		setWindowTitle(tr(TidNewCompanyWindowTitle));

	nameEditor.label.setText(tr(TidCompanyName));
	countryEditor.label.setText(tr(TidCompanyCountry));
	cityEditor.label.setText(tr(TidCompanyCity));
	postalCodeEditor.label.setText(tr(TidCompanyPostalCode));
	addressEditor.label.setText(tr(TidCompanyAddress));
	taxIdEditor.label.setText(tr(TidCompanyTaxId));

	relayout();
}

void EditCompanyView::applyLayout()
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
	mainLayout->addWidget(&taxIdEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditCompanyView::relayout()
{
	{
		nameEditor.wideLayout();
		countryEditor.wideLayout();
		cityEditor.wideLayout();
		postalCodeEditor.wideLayout();
		addressEditor.wideLayout();
		taxIdEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		countryEditor.narrowLayout();
		cityEditor.narrowLayout();
		postalCodeEditor.narrowLayout();
		addressEditor.narrowLayout();
		taxIdEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditCompanyView::updateToolButtonStates()
{
	bool modified = !(
			company.name == nameEditor.editor.text() &&
			company.country == countryEditor.editor.text() &&
			company.city == cityEditor.editor.text() &&
			company.postalCode == postalCodeEditor.editor.text() &&
			company.address == addressEditor.editor.text() &&
			company.taxId == taxIdEditor.editor.text()
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

void EditCompanyView::saveSlotSpec()
{
}

@include@ setCursor prevSlot nextSlot saveSlot resetSlot

