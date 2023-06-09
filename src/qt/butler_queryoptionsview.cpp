/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_queryoptionsview.h"
#include "butler_tagwidget.h"

SCC TidContext = "ItemQueryOptionsView";

SCC TidEditQueryWindowTitle = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Select and edit query");

SCC TidQueryButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Query");
SCC TidSaveButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Save");
SCC TidResetButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Reset");
SCC TidDelButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Delete");

SCC TidQuerySelector = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Query name:");
SCC TidPartnerFilter = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Filter by partner:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Business partner:");
SCC TidWareFilter = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Filter by ware:");
SCC TidWareSelector = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Common ware name:");

SCC TidFromDateTimeEditor = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "From:");
SCC TidTillDateTimeEditor = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Till:");

SCC TidWithTagFilter = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Filter by tags to have");
//SCC TidWithTags = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "With some or all of these tags:");
SCC TidWithoutTagFilter = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Filter by tags not to have");
SCC TidWithoutTags = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Without any of these tags:");

SCC TidStockOptions = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Stock option:");
SCC TidStockOptAllRadioButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "all items");
SCC TidStockOptOnStockRadioButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "items on stock");
SCC TidStockOptUsedUpRadioButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "items used up");

SCC TidTagOptions = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Tags need to match with");
SCC TidTagOptAllMatchRadioButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "all selected tags");
SCC TidTagOptAnyMatchRadioButton = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "any selected tags");

SCC TidQuerySavedInfo = QT_TRANSLATE_NOOP("ItemQueryOptionsView", "Query is saved.");

ItemQueryOptionsView::ItemQueryOptionsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	queryButton(TidQueryButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Enter)),
	saveButton(TidSaveButton, TidContext, QKeySequence(QKeySequence::Save)),
	delButton(TidDelButton, TidContext, QKeySequence(QKeySequence::Delete)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	nameEditor(&itemQueryModel(dbname), ItemQuery::Name),
	wareSelector(&wareModel(dbname), Ware::Name),
	partnerSelector(&partnerModel(dbname), Partner::Name),
	tagsWidget(dbname),
	withoutTagsWidget(dbname)
{
	setWindowModality(Qt::ApplicationModal);

	query.endDate = DateTime::now();

	stockOptions.group.addButton(&stockOptAll);
	stockOptions.group.addButton(&stockOptOnStock);
	stockOptions.group.addButton(&stockOptUsedUp);

	tagOptions.group.addButton(&tagOptAllMatch);
	tagOptions.group.addButton(&tagOptAnyMatch);

	toolBar.addToolWidget(queryButton);
	toolBar.addToolWidget(saveButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(resetButton);

	connect(&queryButton, SIGNAL(clicked()), this, SLOT(querySlot()));
	connect(&saveButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));

	connect(&wareFilter.box, SIGNAL(stateChanged(int)), this, SLOT(layoutContentChangeSlot()));
	connect(&partnerFilter.box, SIGNAL(stateChanged(int)), this, SLOT(layoutContentChangeSlot()));
	connect(&withTagFilter.box, SIGNAL(stateChanged(int)),
			this, SLOT(layoutContentChangeSlot()));
	connect(&withoutTagFilter.box, SIGNAL(stateChanged(int)),
			this, SLOT(layoutContentChangeSlot()));

	connect(&nameEditor.box, SIGNAL(activated(const QString &)),
			this, SLOT(querySelectedSlot()));
	connect(&nameEditor.editor, SIGNAL(editingFinished()), this, SLOT(querySelectedSlot()));

	connect(&nameEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&startDate.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&endDate.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&wareSelector.box, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerSelector.box, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&stockOptAll, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
	connect(&stockOptOnStock, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
	connect(&stockOptUsedUp, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
/*	connect(&stockOptions, SIGNAL(buttonClicked(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&tagOptions, SIGNAL(buttonClicked(int)),
			this, SLOT(updateToolButtonStates()));*/
	connect(&tagOptAllMatch, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
	connect(&tagOptAnyMatch, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
	connect(&tagsWidget, SIGNAL(selectionChanged()),
			this, SLOT(updateToolButtonStates()));
	connect(&withoutTagsWidget, SIGNAL(selectionChanged()),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

ItemQueryOptionsView::~ItemQueryOptionsView()
{
}

void ItemQueryOptionsView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
	relayout();
}

void ItemQueryOptionsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void ItemQueryOptionsView::loadState()
{
	QString prefix("ItemQueryOptionsView");
	PannView::loadState(prefix);
}

void ItemQueryOptionsView::saveState()
{
	QString prefix("ItemQueryOptionsView");
	PannView::saveState(prefix);
}

void ItemQueryOptionsView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void ItemQueryOptionsView::resizeEvent(QResizeEvent *event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	updateGeometry();
	relayout();
}

void ItemQueryOptionsView::mapToGui()
{
	nameEditor.setText(query.name);
	startDate.edit.setDateTime(query.startDate);
	endDate.edit.setDateTime(query.endDate);

	if(query.wares.size()){
		WareModel & wm = wareModel(dbname);
		int row = wm.index(query.wares.queryAt(0).ware);
		if(0 <= row){
			wareFilter.box.blockSignals(true);
			wareFilter.box.setChecked(true);
			wareFilter.box.blockSignals(false);
			wareSelector.box.setCurrentIndex(row);
		}
	} else
		wareFilter.box.setChecked(false);

	if(query.partners.size()){
		PartnerModel & sm = partnerModel(dbname);
		int row = sm.index(query.partners.queryAt(0).partner);
		if(0 <= row){
			partnerFilter.box.blockSignals(true);
			partnerFilter.box.setChecked(true);
			partnerFilter.box.blockSignals(false);
			partnerSelector.box.setCurrentIndex(row);
		}
	} else {
		partnerFilter.box.blockSignals(true);
		partnerFilter.box.setChecked(false);
		partnerFilter.box.blockSignals(false);
	}

	if(query.stockOption == ItemQueryStockOptions::AllItemChanges)
		stockOptAll.setChecked(true);
	else if(query.stockOption == ItemQueryStockOptions::Gains)
		stockOptOnStock.setChecked(true);
	else if(query.stockOption == ItemQueryStockOptions::Looses)
		stockOptUsedUp.setChecked(true);

	if(query.withTags.size()){
		withTagFilter.box.blockSignals(true);
		withTagFilter.box.setChecked(true);
		withTagFilter.box.blockSignals(false);
	} else {
		withTagFilter.box.blockSignals(true);
		withTagFilter.box.setChecked(false);
		withTagFilter.box.blockSignals(false);
	}
	if(query.tagOption == ItemQueryTagOptions::MatchAll)
		tagOptAllMatch.setChecked(true);
	else if(query.tagOption == ItemQueryTagOptions::MatchAny)
		tagOptAnyMatch.setChecked(true);
	tagsWidget.setTags(query.withTags);

	if(query.withoutTags.size()){
		withoutTagFilter.box.blockSignals(true);
		withoutTagFilter.box.setChecked(true);
		withoutTagFilter.box.blockSignals(false);
	} else {
		withoutTagFilter.box.blockSignals(true);
		withoutTagFilter.box.setChecked(false);
		withoutTagFilter.box.blockSignals(false);
	}
	withoutTagsWidget.setTags(query.withoutTags);

	updateToolButtonStates();
}

void ItemQueryOptionsView::mapFromGui()
{
	query.name = nameEditor.text();
	query.startDate = startDate.edit.dateTime();
	query.endDate = endDate.edit.dateTime();

	query.wares.clear();
	if(wareFilter.box.isChecked()){
		int i = wareSelector.box.currentIndex();
		WareModel & wm = wareModel(dbname);
		if(0 <= i && i < wm.rowCount())
			query.wares.add(new ItemQueryWare(query.name, wm.data(i).name.trimmed()));
	}
	
	query.partners.clear();
	if(partnerFilter.box.isChecked()){
		int i = partnerSelector.box.currentIndex();
		PartnerModel & sm = partnerModel(dbname);
		if(0 <= i && i < sm.rowCount())
			query.partners.add(new ItemQueryPartner(query.name, sm.data(i).name.trimmed()));
	}

	if(stockOptions.group.checkedButton() == &stockOptAll)
		query.stockOption = ItemQueryStockOptions::AllItemChanges;
	else if(stockOptions.group.checkedButton() == &stockOptOnStock)
		query.stockOption = ItemQueryStockOptions::Gains;
	else if(stockOptions.group.checkedButton() == &stockOptUsedUp)
		query.stockOption = ItemQueryStockOptions::Looses;

	if(withTagFilter.box.isChecked()){
		if(tagOptions.group.checkedButton() == &tagOptAllMatch)
			query.tagOption = ItemQueryTagOptions::MatchAll;
		else if(tagOptions.group.checkedButton() == &tagOptAnyMatch)
			query.tagOption = ItemQueryTagOptions::MatchAny;
		query.setAsWithTags(tagsWidget.selectedTags());
	} else {
		query.tagOption = ItemQueryTagOptions::MatchAny;
		query.withTags.clear();
	}

	if(withoutTagFilter.box.isChecked()){
		query.setAsWithoutTags(withoutTagsWidget.selectedTags());
	} else {
		query.withoutTags.clear();
	}
}

void ItemQueryOptionsView::retranslate()
{
	setWindowTitle(tr(TidEditQueryWindowTitle));

	nameEditor.label.setText(tr(TidQuerySelector));

	startDate.label.setText(tr(TidFromDateTimeEditor));
	endDate.label.setText(tr(TidTillDateTimeEditor));

	wareSelector.label.setText(tr(TidWareSelector));

	partnerFilter.label.setText(tr(TidPartnerFilter));
	partnerSelector.label.setText(tr(TidPartnerSelector));

	withTagFilter.label.setText(tr(TidWithTagFilter));
	tagsWidget.label.setText("");//tr(TidWithTags));

	withoutTagFilter.label.setText(tr(TidWithoutTagFilter));
	withoutTagsWidget.label.setText(tr(TidWithoutTags));

	stockOptions.label.setText(tr(TidStockOptions));
	stockOptAll.setText(tr(TidStockOptAllRadioButton));
	stockOptOnStock.setText(tr(TidStockOptOnStockRadioButton));
	stockOptUsedUp.setText(tr(TidStockOptUsedUpRadioButton));

	tagOptions.label.setText(tr(TidTagOptions));
	tagOptAllMatch.setText(tr(TidTagOptAllMatchRadioButton));
	tagOptAnyMatch.setText(tr(TidTagOptAnyMatchRadioButton));

	relayout();
}

void ItemQueryOptionsView::applyLayout(LayoutState state, bool test)
{
	delete layout();

	HLayout * filterLayout = 0;
	if(state == LayoutState::Wide){
		filterLayout = new HLayout;
		filterLayout->addWidget(&wareFilter);
		filterLayout->addStretch(0);
		filterLayout->addWidget(&partnerFilter);
		filterLayout->addStretch(0);
		filterLayout->addWidget(&withTagFilter);
		filterLayout->addStretch(0);
		filterLayout->addWidget(&withoutTagFilter);
		filterLayout->addStretch(0);
	}

	HLayout * filter1Layout = 0;
	if(state == LayoutState::Medium){
		filter1Layout = new HLayout;
		filter1Layout->addWidget(&wareFilter);
		filter1Layout->addStretch(0);
		filter1Layout->addWidget(&partnerFilter);
		filter1Layout->addStretch(0);
	}

	HLayout * filter2Layout = 0;
	if(state == LayoutState::Medium){
		filter2Layout = new HLayout;
		filter2Layout->addWidget(&withTagFilter);
		filter2Layout->addStretch(0);
		filter2Layout->addWidget(&withoutTagFilter);
		filter2Layout->addStretch(0);
	}

	HLayout * dateLayout = 0;
	if(state == LayoutState::Wide){
		dateLayout = new HLayout;
		dateLayout->addWidget(&startDate, 4);
		dateLayout->addStretch(1);
		dateLayout->addWidget(&endDate, 4);
	}

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	if(state == LayoutState::Wide){
		mainLayout->addStretch(0);
		mainLayout->addLayout(dateLayout);
	} else {
		mainLayout->addStretch(0);
		mainLayout->addWidget(&startDate);
		mainLayout->addStretch(0);
		mainLayout->addWidget(&endDate);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&stockOptions);
	if(state == LayoutState::Wide){
		mainLayout->addStretch(0);
		mainLayout->addLayout(filterLayout);
	}
	if(state == LayoutState::Medium){
		mainLayout->addStretch(0);
		mainLayout->addLayout(filter1Layout);
	}
	if(state == LayoutState::Narrow){
		mainLayout->addStretch(0);
		mainLayout->addWidget(&wareFilter);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&wareSelector);
	if(state == LayoutState::Narrow){
		mainLayout->addStretch(0);
		mainLayout->addWidget(&partnerFilter);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerSelector);
	if(state == LayoutState::Medium){
		mainLayout->addStretch(0);
		mainLayout->addLayout(filter2Layout);
	}
	if(state == LayoutState::Narrow){
		mainLayout->addStretch(0);
		mainLayout->addWidget(&withTagFilter);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&tagOptions);
	if(!test){
		mainLayout->addWidget(&tagsWidget);
		mainLayout->addStretch(0);
	}
	if(state == LayoutState::Narrow){
		mainLayout->addStretch(0);
		mainLayout->addWidget(&withoutTagFilter);
	}
	if(!test){
		mainLayout->addWidget(&withoutTagsWidget);
		mainLayout->addStretch(0);
	}

	setLayout(mainLayout);
	updateGeometry();
}

void ItemQueryOptionsView::relayout()
{
	LayoutState newState = LayoutState::Wide;
	nameEditor.wideLayout();
	startDate.wideLayout();
	endDate.wideLayout();
	wareSelector.wideLayout();
	partnerSelector.wideLayout();
	stockOptions.wideLayout();
	tagOptions.wideLayout();
	wareFilter.label.setText(tr(TidWareFilter));
	applyLayout(newState, true);

	if(width() < sizeHint().width()){
		newState = LayoutState::Medium;
		nameEditor.wideLayout();
		startDate.wideLayout();
		endDate.wideLayout();
		wareSelector.wideLayout();
		partnerSelector.wideLayout();
		stockOptions.mediumLayout();
		tagOptions.mediumLayout();
		wareFilter.label.setText(trShort(TidWareFilter));
		applyLayout(newState, true);
	}
	if(width() < sizeHint().width()){
		newState = LayoutState::Narrow;
		nameEditor.narrowLayout();
		startDate.narrowLayout();
		endDate.narrowLayout();
		wareSelector.narrowLayout();
		partnerSelector.narrowLayout();
		stockOptions.narrowLayout();
		tagOptions.narrowLayout();
		wareFilter.label.setText(trShort(TidWareFilter));
		applyLayout(newState, true);
	}

	applyLayout(newState);
	updateToolButtonStates();
}

void ItemQueryOptionsView::saveSlotSpec()
{
}

void ItemQueryOptionsView::saveSlot()
{
	mapFromGui();

	ItemQueryModel & qm = itemQueryModel(dbname);
	if(qm.set.has(query.name)) {
		if(qm.set.query(query.name) != query)
			qm.update(qm.index(query.name), query);
	} else
		qm.addNew(query);
	toolBar.setInfo(tr(TidQuerySavedInfo));
	updateToolButtonStates();
}

void ItemQueryOptionsView::delSlot()
{
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a query"),
			tr("Shall we delete this query: ") + query.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes){
		ItemQueryModel & qm = itemQueryModel(dbname);
		if(qm.set.has(query.name))
			qm.del(qm.index(query.name));
		query = ItemQuery();
		query.endDate = DateTime::now();
		mapToGui();
	}
}

void ItemQueryOptionsView::querySlot()
{
	mapFromGui();

	ItemQueryModel & qm = itemQueryModel(dbname);
	if(!qm.set.has(query.name))
		query.name = "";
	else if(qm.set.query(query.name) != query)
		query.name = "";

	accept();
}

void ItemQueryOptionsView::resetSlot()
{
	mapToGui();
}

void ItemQueryOptionsView::backSlot()
{
	reject();
}

void ItemQueryOptionsView::querySelectedSlot()
{
	ItemQueryModel & qm = itemQueryModel(dbname);
	Text name(nameEditor.text());
	if(!qm.set.has(name))
		return;
	query = qm.set.query(name);
	mapToGui();
}

void ItemQueryOptionsView::layoutContentChangeSlot()
{
	updateToolButtonStates();
	relayout();
}

void ItemQueryOptionsView::updateToolButtonStates()
{
	bool modified = !(
			query.name == nameEditor.text() &&
			query.startDate == startDate.edit.dateTime() &&
//			query.endDate == endDate.edit.dateTime() &&

			((0<query.wares.size()) ==(wareFilter.box.checkState()==Qt::Checked)) &&
			((0<query.partners.size())==(partnerFilter.box.checkState()==Qt::Checked))&&
			((0<query.withTags.size())==(withTagFilter.box.checkState()==Qt::Checked))&&
			((0<query.withoutTags.size())==(withoutTagFilter.box.checkState()==Qt::Checked)) &&

			query.withTags == tagsWidget.selectedTags() &&
			query.withoutTags == withoutTagsWidget.selectedTags()
			);
	if(!modified)
		if(query.wares.size() && (wareFilter.box.checkState()==Qt::Checked))
			if(query.wares.queryAt(0).ware != wareSelector.box.currentText())
				modified = true;
	if(!modified)
		if(query.partners.size() && (partnerFilter.box.checkState()==Qt::Checked))
			if(query.partners.queryAt(0).partner != partnerSelector.box.currentText())
				modified = true;
	if(!modified)
		if(query.withTags.size() && (withTagFilter.box.checkState()==Qt::Checked))
			if(query.withTags != tagsWidget.selectedTags())
				modified = true;
	if(!modified)
		if(query.withoutTags.size() && (withoutTagFilter.box.checkState()==Qt::Checked))
			if(query.withoutTags != withoutTagsWidget.selectedTags())
				modified = true;

	if(!modified){
		switch(query.stockOption) {
			case ItemQueryStockOptions::AllItemChanges :
				if(stockOptions.group.checkedButton() != &stockOptAll)
					modified = true;
				break;
			case ItemQueryStockOptions::Gains :
				if(stockOptions.group.checkedButton() != &stockOptOnStock)
					modified = true;
				break;
			case ItemQueryStockOptions::Looses :
				if(stockOptions.group.checkedButton() != &stockOptUsedUp)
					modified = true;
				break;
			default:
				modified = true;
				break;
		}
	}

	if(!modified){
		if(withoutTagFilter.box.checkState()==Qt::Checked){
			switch(query.tagOption) {
				case (ItemQueryTagOptions::MatchAll) :
					if(tagOptions.group.checkedButton() != &tagOptAllMatch)
					modified = true;
					break;
				case ItemQueryTagOptions::MatchAny :
					if(tagOptions.group.checkedButton() != &tagOptAnyMatch)
						modified = true;
					break;
				default:
					modified = true;
					break;
			}
		} else if(query.tagOption != ItemQueryTagOptions::MatchAny)
			modified = true;
	}

	bool hasName(nameEditor.text().size());

	/* Lets adjust states */

	saveButton.setVisible(modified && hasName);
	delButton.setVisible(!modified && hasName);
	resetButton.setVisible(modified);

	wareSelector.setVisible(wareFilter.box.checkState()==Qt::Checked);
	partnerSelector.setVisible(partnerFilter.box.checkState()==Qt::Checked);
	tagOptions.setVisible(withTagFilter.box.checkState()==Qt::Checked);
	tagsWidget.setVisible(withTagFilter.box.checkState()==Qt::Checked);
	withoutTagsWidget.setVisible(withoutTagFilter.box.checkState()==Qt::Checked);

	if(modified)
		toolBar.clearInfo();

	toolBar.updateButtons();
}

