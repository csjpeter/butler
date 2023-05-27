/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_edittagview.h"

SCC TidContext = "EditTagView";

SCC TidNewTagWindowTitle = QT_TRANSLATE_NOOP("EditTagView", "Add new tag");
SCC TidEditTagWindowTitle = QT_TRANSLATE_NOOP("EditTagView", "Editing a tag");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditTagView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditTagView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditTagView", "Previous tag");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditTagView", "Next tag");

SCC TidTagName = QT_TRANSLATE_NOOP("EditTagView", "Tag name:");
SCC TidTagDescription = QT_TRANSLATE_NOOP("EditTagView", "Description:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditTagView", "Please fill the tag name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditTagView", "Tag is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditTagView", "Tag is updated.");

EditTagView::EditTagView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(tagModel(dbname)),
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
	connect(&descEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

void EditTagView::mapToGui()
{
	if(cursor.isValid())
		tag = Tag(model.data(cursor.row()));

	nameEditor.editor.setText(tag.name);
	descEditor.editor.setText(tag.description);

	updateToolButtonStates();
}

void EditTagView::mapFromGui()
{
	tag.name <<= nameEditor.editor.text();
	tag.description <<= descEditor.editor.text();
}

void EditTagView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditTagWindowTitle));
	else
		setWindowTitle(tr(TidNewTagWindowTitle));

	nameEditor.label.setText(tr(TidTagName));
	descEditor.label.setText(tr(TidTagDescription));

	relayout();
}

void EditTagView::applyLayout()
{
	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&descEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditTagView::relayout()
{
	{
		nameEditor.wideLayout();
		descEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		descEditor.narrowLayout();
		applyLayout();
	}

	updateToolButtonStates();
}

void EditTagView::updateToolButtonStates()
{
	bool modified = !(
			tag.name == nameEditor.editor.text() &&
			tag.description == descEditor.editor.text()
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

void EditTagView::saveSlotSpec()
{
}






























void EditTagView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEditTagWindowTitle));
	mapToGui();
}

void EditTagView::prevSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditTagView::nextSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (cursor.row() < model.rowCount() - 1) ?
			(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditTagView::saveSlot()
{
	mapFromGui();

	saveSlotSpec();

	if(cursor.isValid()){
		if(model.data(cursor.row()) != tag)
			model.update(cursor.row(), tag);
		auto row = model.set.index(tag.name);
		setCursor(model.index(row, cursor.column()));
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(tag);

		tag = Tag();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		//nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditTagView::resetSlot()
{
	mapToGui();
}


void EditTagView::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditTagView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void EditTagView::loadState()
{
	QString prefix(cursor.isValid() ? "EditTagView" : "NewTagView");
	PannView::loadState(prefix);
}

void EditTagView::saveState()
{
	QString prefix(cursor.isValid() ? "EditTagView" : "NewTagView");
	PannView::saveState(prefix);
}

void EditTagView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void EditTagView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}



