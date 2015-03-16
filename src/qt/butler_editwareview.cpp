/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editwareview.h"
#include "butler_waresmodel.h"
#include "butler_tagsmodel.h"
#include "butler_tagwidget.h"

SCC TidContext = "EditWareView";

SCC TidNewWareWindowTitle = QT_TRANSLATE_NOOP("EditWareView", "Add new ware");
SCC TidEditWareWindowTitle = QT_TRANSLATE_NOOP("EditWareView", "Editing a ware");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditWareView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditWareView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditWareView", "Previous ware");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditWareView", "Next ware");

SCC TidWareName = QT_TRANSLATE_NOOP("EditWareView", "Ware name:");
SCC TidWareUnit = QT_TRANSLATE_NOOP("EditWareView", "Unit:");
SCC TidWareTypes = QT_TRANSLATE_NOOP("EditWareView", "Types:");
SCC TidWareTags = QT_TRANSLATE_NOOP("EditWareView", "Tags:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditWareView", "Please fill the ware name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditWareView", "Ware is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditWareView", "Ware is updated.");

EditWareView::EditWareView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(wareModel(dbname)),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right)),
	tagsWidget(dbname)
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
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));

	connect(&nameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&unitEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&typesEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&tagsWidget, SIGNAL(selectionChanged()),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

void EditWareView::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditWareView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void EditWareView::loadState()
{
	QString prefix(cursor.isValid() ? "EditWareView" : "NewWareView");
	PannView::loadState(prefix);
}

void EditWareView::saveState()
{
	QString prefix(cursor.isValid() ? "EditWareView" : "NewWareView");
	PannView::saveState(prefix);
}

void EditWareView::mapToGui()
{
	if(cursor.isValid())
		ware = Ware(model.data(cursor.row()));

	nameEditor.editor.setText(ware.name);
	unitEditor.editor.setText(ware.unit);
	QString str;
	str <<= ware.types;
	typesEditor.editor.setText(str);
	tagsWidget.setTags(ware.tags);

	updateToolButtonStates();
}

void EditWareView::mapFromGui()
{
	ware.name = nameEditor.editor.text();
	ware.unit = unitEditor.editor.text();
	ware.setAsTypes(typesEditor.editor.text());
	ware.setAsTags(tagsWidget.selectedTags());
}

void EditWareView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void EditWareView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void EditWareView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditWareWindowTitle));
	else
		setWindowTitle(tr(TidNewWareWindowTitle));

	nameEditor.label.setText(tr(TidWareName));
	unitEditor.label.setText(tr(TidWareUnit));
	typesEditor.label.setText(tr(TidWareTypes));
	tagsWidget.label.setText(tr(TidWareTags));

	relayout();
}

void EditWareView::applyLayout(bool test)
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&unitEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&typesEditor);
	mainLayout->addStretch(0);
	if(!test){
		mainLayout->addWidget(&tagsWidget);
		mainLayout->addStretch(0);
	}

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditWareView::relayout()
{
	{
		nameEditor.wideLayout();
		unitEditor.wideLayout();
		typesEditor.wideLayout();
		applyLayout(true);
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		unitEditor.narrowLayout();
		typesEditor.narrowLayout();
		applyLayout(true);
	}

	applyLayout();
	updateToolButtonStates();
}

void EditWareView::updateToolButtonStates()
{
	bool modified = !(
			ware.name == nameEditor.editor.text() &&
			ware.unit == unitEditor.editor.text() &&
			ware.types == typesEditor.editor.text() &&
			ware.tags == tagsWidget.selectedTags()
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

void EditWareView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEditWareWindowTitle));
	mapToGui();
}

void EditWareView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditWareView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditWareView::saveSlot()
{
	mapFromGui();

	if(cursor.isValid()){
		if(model.data(cursor.row()) != ware)
			model.update(cursor.row(), ware);
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(ware);

		ware = Ware();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditWareView::resetSlot()
{
	mapToGui();
}
