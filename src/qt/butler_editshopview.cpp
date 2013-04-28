/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editshopview.h"
#include "butler_shopsmodel.h"

SCC TidContext = "EditPartnerView";

SCC TidNewPartnerWindowTitle = QT_TRANSLATE_NOOP("EditPartnerView", "Add new partner");
SCC TidEditPartnerWindowTitle = QT_TRANSLATE_NOOP("EditPartnerView", "Editing a partner");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditPartnerView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditPartnerView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditPartnerView", "Previous partner");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditPartnerView", "Next partner");

SCC TidPartnerName = QT_TRANSLATE_NOOP("EditPartnerView", "Partner name:");
SCC TidPartnerStoreName = QT_TRANSLATE_NOOP("EditPartnerView", "Store name:");
SCC TidPartnerCity = QT_TRANSLATE_NOOP("EditPartnerView", "City:");
SCC TidPartnerAddress = QT_TRANSLATE_NOOP("EditPartnerView", "Address:");
SCC TidPartnerCompany = QT_TRANSLATE_NOOP("EditPartnerView", "Company:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditPartnerView", "Please fill the partner name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditPartnerView", "Partner is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditPartnerView", "Partner is updated.");

EditPartnerView::EditPartnerView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(partnersModel(dbname)),
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
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));

	connect(&nameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&storeNameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&cityEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&addressEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&companyEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	loadState();
	retranslate();
}

void EditPartnerView::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditPartnerView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void EditPartnerView::loadState()
{
	QString prefix(cursor.isValid() ? "EditPartnerView" : "NewPartnerView");
	PannView::loadState(prefix);
}

void EditPartnerView::saveState()
{
	QString prefix(cursor.isValid() ? "EditPartnerView" : "NewPartnerView");
	PannView::saveState(prefix);
}

void EditPartnerView::mapToGui()
{
	if(cursor.isValid())
		partner = Shop(model.partner(cursor.row()));

	nameEditor.editor.setText(partner.name);
	storeNameEditor.editor.setText(partner.storeName);
	cityEditor.editor.setText(partner.city);
	addressEditor.editor.setText(partner.address);
	companyEditor.editor.setText(partner.company);

	updateToolButtonStates();
}

void EditPartnerView::mapFromGui()
{
	partner.name = nameEditor.editor.text();
	partner.storeName = storeNameEditor.editor.text();
	partner.city = cityEditor.editor.text();
	partner.address = addressEditor.editor.text();
	partner.company = companyEditor.editor.text();
}

void EditPartnerView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void EditPartnerView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void EditPartnerView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditPartnerWindowTitle));
	else
		setWindowTitle(tr(TidNewPartnerWindowTitle));

	nameEditor.label.setText(tr(TidPartnerName));
	storeNameEditor.label.setText(tr(TidPartnerStoreName));
	cityEditor.label.setText(tr(TidPartnerCity));
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
	mainLayout->addWidget(&cityEditor);
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
		cityEditor.wideLayout();
		addressEditor.wideLayout();
		companyEditor.wideLayout();
		applyLayout();
	}
	if(width() < sizeHint().width()){
		nameEditor.narrowLayout();
		storeNameEditor.narrowLayout();
		cityEditor.narrowLayout();
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
			partner.storeName == storeNameEditor.editor.text() &&
			partner.city == cityEditor.editor.text() &&
			partner.address == addressEditor.editor.text() &&
			partner.company == companyEditor.editor.text()
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

void EditPartnerView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEditPartnerWindowTitle));
	mapToGui();
}

void EditPartnerView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditPartnerView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditPartnerView::saveSlot()
{
	mapFromGui();

	if(cursor.isValid()){
		if(model.partner(cursor.row()) != partner)
			model.update(cursor.row(), partner);
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(partner);

		partner = Shop();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditPartnerView::resetSlot()
{
	mapToGui();
}
