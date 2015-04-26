/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editinventoryview.h"

SCC TidContext = "EditInventoryView";

SCC TidNewInventoryWindowTitle = QT_TRANSLATE_NOOP("EditInventoryView", "Add new inventory");
SCC TidEditInventoryWindowTitle = QT_TRANSLATE_NOOP("EditInventoryView", "Editing a inventory");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditInventoryView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditInventoryView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditInventoryView", "Previous inventory");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditInventoryView", "Next inventory");

SCC TidInventoryName = QT_TRANSLATE_NOOP("EditInventoryView", "Inventory name:");
SCC TidInventoryComment = QT_TRANSLATE_NOOP("EditInventoryView", "Comment:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditInventoryView", "Please fill the inventory name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditInventoryView", "Inventory is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditInventoryView", "Inventory is updated.");

EditInventoryView::EditInventoryView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(inventoryModel(dbname)),
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
	connect(&commentEditor.edit, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

void EditInventoryView::mapToGui()
{
	if(cursor.isValid())
		inventory = Inventory(model.data(cursor.row()));

	nameEditor.editor.setText(inventory.name);
	commentEditor.edit.setText(inventory.comment);

	updateToolButtonStates();
}

void EditInventoryView::mapFromGui()
{
	inventory.name <<= nameEditor.editor.text();
	inventory.comment <<= commentEditor.edit.toPlainText();
}

void EditInventoryView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditInventoryWindowTitle));
	else
		setWindowTitle(tr(TidNewInventoryWindowTitle));

	nameEditor.label.setText(tr(TidInventoryName));
	commentEditor.label.setText(tr(TidInventoryComment));

	relayout();
}

void EditInventoryView::applyLayout()
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&commentEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditInventoryView::relayout()
{
	{
		nameEditor.wideLayout();
		commentEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		commentEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditInventoryView::updateToolButtonStates()
{
	bool modified = !(
			inventory.name == nameEditor.editor.text() &&
			inventory.comment == commentEditor.edit.toPlainText()
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

void EditInventoryView::saveSlotSpec()
{
}

@include@ views.cpp
@declare@ Inventory

@include@ setCursor prevSlot nextSlot saveSlot resetSlot
@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent

