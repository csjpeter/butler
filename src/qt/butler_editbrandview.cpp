/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editbrandview.h"

@include@ views.cpp
@declare@ Brand

SCC TidContext = "EditBrandView";

SCC TidNewBrandWindowTitle = QT_TRANSLATE_NOOP("EditBrandView", "Add new brand");
SCC TidEditBrandWindowTitle = QT_TRANSLATE_NOOP("EditBrandView", "Editing a brand");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditBrandView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditBrandView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditBrandView", "Previous brand");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditBrandView", "Next brand");

SCC TidBrandName = QT_TRANSLATE_NOOP("EditBrandView", "Brand name:");
SCC TidBrandCompany = QT_TRANSLATE_NOOP("EditBrandView", "Company:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditBrandView", "Please fill the brand name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditBrandView", "Brand is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditBrandView", "Brand is updated.");

EditBrandView::EditBrandView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(brandModel(dbname)),
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
	connect(&companyEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent

void EditBrandView::mapToGui()
{
	if(cursor.isValid())
		brand = Brand(model.data(cursor.row()));

	nameEditor.editor.setText(brand.name);
	companyEditor.setText(brand.company);

	updateToolButtonStates();
}

void EditBrandView::mapFromGui()
{
	brand.name <<= nameEditor.editor.text();
	brand.company <<= companyEditor.text();
}

void EditBrandView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditBrandWindowTitle));
	else
		setWindowTitle(tr(TidNewBrandWindowTitle));

	nameEditor.label.setText(tr(TidBrandName));
	companyEditor.label.setText(tr(TidBrandCompany));

	relayout();
}

void EditBrandView::applyLayout()
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&companyEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditBrandView::relayout()
{
	{
		nameEditor.wideLayout();
		companyEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		companyEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditBrandView::updateToolButtonStates()
{
	bool modified = !(
			brand.name == nameEditor.editor.text() &&
			brand.company == companyEditor.text()
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

void EditBrandView::saveSlotSpec()
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

@include@ setCursor prevSlot nextSlot saveSlot resetSlot

