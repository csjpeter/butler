/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_queryoptionsview.h"
#include "butler_tagwidget.h"
#include "butler_waresmodel.h"
#include "butler_shopsmodel.h"

SCC TidContext = "QueryOptionsView";

SCC TidEditQueryWindowTitle = QT_TRANSLATE_NOOP("QueryOptionsView", "Select and edit query");

SCC TidQueryButton = QT_TRANSLATE_NOOP("QueryOptionsView", "Query");
SCC TidSaveButton = QT_TRANSLATE_NOOP("QueryOptionsView", "Save");
SCC TidResetButton = QT_TRANSLATE_NOOP("QueryOptionsView", "Reset");
SCC TidDelButton = QT_TRANSLATE_NOOP("QueryOptionsView", "Delete");

SCC TidQuerySelector = QT_TRANSLATE_NOOP("QueryOptionsView", "Query name:");
SCC TidShopFilter = QT_TRANSLATE_NOOP("QueryOptionsView", "Filter by partner:");
SCC TidShopSelector = QT_TRANSLATE_NOOP("QueryOptionsView", "Business partner:");
SCC TidWareFilter = QT_TRANSLATE_NOOP("QueryOptionsView", "Filter by ware:");
SCC TidWareSelector = QT_TRANSLATE_NOOP("QueryOptionsView", "Common ware name:");

SCC TidFromDateTimeEditor = QT_TRANSLATE_NOOP("QueryOptionsView", "From:");
SCC TidTillDateTimeEditor = QT_TRANSLATE_NOOP("QueryOptionsView", "Till:");

SCC TidWithTagFilter = QT_TRANSLATE_NOOP("QueryOptionsView", "Filter by tags to have");
//SCC TidWithTags = QT_TRANSLATE_NOOP("QueryOptionsView", "With some or all of these tags:");
SCC TidWithoutTagFilter = QT_TRANSLATE_NOOP("QueryOptionsView", "Filter by tags not to have");
SCC TidWithoutTags = QT_TRANSLATE_NOOP("QueryOptionsView", "Without any of these tags:");

SCC TidStockOptions = QT_TRANSLATE_NOOP("QueryOptionsView", "Stock option:");
SCC TidStockOptAllRadioButton = QT_TRANSLATE_NOOP("QueryOptionsView", "all items");
SCC TidStockOptOnStockRadioButton = QT_TRANSLATE_NOOP("QueryOptionsView", "items on stock");
SCC TidStockOptUsedUpRadioButton = QT_TRANSLATE_NOOP("QueryOptionsView", "items used up");

SCC TidTagOptions = QT_TRANSLATE_NOOP("QueryOptionsView", "Tags need to match with");
SCC TidTagOptAllMatchRadioButton = QT_TRANSLATE_NOOP("QueryOptionsView", "all selected tags");
SCC TidTagOptAnyMatchRadioButton = QT_TRANSLATE_NOOP("QueryOptionsView", "any selected tags");

SCC TidQuerySavedInfo = QT_TRANSLATE_NOOP("QueryOptionsView", "Query is saved.");

QueryOptionsView::QueryOptionsView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	toolBar(this),
	queryButton(TidQueryButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Enter)),
	saveButton(TidSaveButton, TidContext, QKeySequence(QKeySequence::Save)),
	delButton(TidDelButton, TidContext, QKeySequence(QKeySequence::Delete)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	nameEditor(&queriesModel(dbname), Query::Name),
	wareSelector(&waresModel(dbname), Ware::Name),
	shopSelector(&shopsModel(dbname), Shop::Name),
	tagsWidget(dbname),
	withoutTagsWidget(dbname)
{
	setWindowModality(Qt::ApplicationModal);

	stockOptions.group.addButton(&stockOptAll);
	stockOptions.group.addButton(&stockOptOnStock);
	stockOptions.group.addButton(&stockOptUsedUp);

	tagOptions.group.addButton(&tagOptAllMatch);
	tagOptions.group.addButton(&tagOptAnyMatch);

	toolBar.addToolWidget(queryButton);
	toolBar.addToolWidget(saveButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(resetButton);
	toolBar.relayout();
	setToolBar(&toolBar);

	connect(&queryButton, SIGNAL(clicked()), this, SLOT(queryClickedSlot()));
	connect(&saveButton, SIGNAL(clicked()), this, SLOT(saveClickedSlot()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delClickedSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetClickedSlot()));

	connect(&wareFilter.box, SIGNAL(stateChanged(int)), this, SLOT(layoutContentChangeSlot()));
	connect(&shopFilter.box, SIGNAL(stateChanged(int)), this, SLOT(layoutContentChangeSlot()));
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
	connect(&shopSelector.box, SIGNAL(currentIndexChanged(const QString &)),
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

	loadState();
	retranslate();
}

QueryOptionsView::~QueryOptionsView()
{
}

void QueryOptionsView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
	relayout();
}

void QueryOptionsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void QueryOptionsView::loadState()
{
	QString prefix("QueryOptionsView");
	PannView::loadState(prefix);
}

void QueryOptionsView::saveState()
{
	QString prefix("QueryOptionsView");
	PannView::saveState(prefix);
}

void QueryOptionsView::mapToGui()
{
	nameEditor.setText(query.name);
	startDate.edit.setDateTime(query.startDate);
	endDate.edit.setDateTime(query.endDate);

	if(query.wares.size()){
		WaresModel &wm = waresModel(dbname);
		int row = wm.index(query.wares.queryAt(0));
		if(0 <= row){
			wareFilter.box.blockSignals(true);
			wareFilter.box.setChecked(true);
			wareFilter.box.blockSignals(false);
			wareSelector.box.setCurrentIndex(row);
		}
	} else
		wareFilter.box.setChecked(false);

	if(query.shops.size()){
		ShopsModel &sm = shopsModel(dbname);
		int row = sm.index(query.shops.queryAt(0));
		if(0 <= row){
			shopFilter.box.blockSignals(true);
			shopFilter.box.setChecked(true);
			shopFilter.box.blockSignals(false);
			shopSelector.box.setCurrentIndex(row);
		}
	} else {
		shopFilter.box.blockSignals(true);
		shopFilter.box.setChecked(false);
		shopFilter.box.blockSignals(false);
	}

	if(query.stockOption == Query::StockOptions::AllBoughtItem)
		stockOptAll.setChecked(true);
	else if(query.stockOption == Query::StockOptions::ItemsOnStock)
		stockOptOnStock.setChecked(true);
	else if(query.stockOption == Query::StockOptions::ItemsUsedUp)
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
	if(query.tagOption == Query::TagOptions::MatchAll)
		tagOptAllMatch.setChecked(true);
	else if(query.tagOption == Query::TagOptions::MatchAny)
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

void QueryOptionsView::mapFromGui()
{
	query.name = nameEditor.text();
	query.startDate = startDate.edit.dateTime();
	query.endDate = endDate.edit.dateTime();

	query.wares.clear();
	if(wareFilter.box.isChecked()){
		int i = wareSelector.box.currentIndex();
		WaresModel &wm = waresModel(dbname);
		if(0 <= i && i < wm.rowCount())
			query.wares.add(new QString(wm.ware(i).name.trimmed()));
	}
	
	query.shops.clear();
	if(shopFilter.box.isChecked()){
		int i = shopSelector.box.currentIndex();
		ShopsModel &sm = shopsModel(dbname);
		if(0 <= i && i < sm.rowCount())
			query.shops.add(new QString(sm.shop(i).name.trimmed()));
	}

	if(stockOptions.group.checkedButton() == &stockOptAll)
		query.stockOption = Query::StockOptions::AllBoughtItem;
	else if(stockOptions.group.checkedButton() == &stockOptOnStock)
		query.stockOption = Query::StockOptions::ItemsOnStock;
	else if(stockOptions.group.checkedButton() == &stockOptUsedUp)
		query.stockOption = Query::StockOptions::ItemsUsedUp;

	if(withTagFilter.box.isChecked()){
		if(tagOptions.group.checkedButton() == &tagOptAllMatch)
			query.tagOption = Query::TagOptions::MatchAll;
		else if(tagOptions.group.checkedButton() == &tagOptAnyMatch)
			query.tagOption = Query::TagOptions::MatchAny;
		query.withTags = tagsWidget.selectedTags();
	} else {
		query.tagOption = Query::TagOptions::MatchAny;
		query.withTags.clear();
	}

	if(withoutTagFilter.box.isChecked()){
		query.withoutTags = withoutTagsWidget.selectedTags();
	} else {
		query.withoutTags.clear();
	}
}

void QueryOptionsView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void QueryOptionsView::resizeEvent(QResizeEvent *event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	updateGeometry();
	relayout();
}

void QueryOptionsView::retranslate()
{
	setWindowTitle(tr(TidEditQueryWindowTitle));

	nameEditor.label.setText(tr(TidQuerySelector));

	startDate.label.setText(tr(TidFromDateTimeEditor));
	endDate.label.setText(tr(TidTillDateTimeEditor));

	wareSelector.label.setText(tr(TidWareSelector));

	shopFilter.label.setText(tr(TidShopFilter));
	shopSelector.label.setText(tr(TidShopSelector));

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

void QueryOptionsView::applyLayout(LayoutState state, bool test)
{
	delete layout();

	HLayout * filterLayout = 0;
	if(state == LayoutState::Wide){
		filterLayout = new HLayout;
		filterLayout->addWidget(&wareFilter);
		filterLayout->addStretch(0);
		filterLayout->addWidget(&shopFilter);
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
		filter1Layout->addWidget(&shopFilter);
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
		mainLayout->addWidget(&shopFilter);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&shopSelector);
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

void QueryOptionsView::relayout()
{
	LayoutState newState = LayoutState::Wide;
	nameEditor.wideLayout();
	startDate.wideLayout();
	endDate.wideLayout();
	wareSelector.wideLayout();
	shopSelector.wideLayout();
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
		shopSelector.wideLayout();
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
		shopSelector.narrowLayout();
		stockOptions.narrowLayout();
		tagOptions.narrowLayout();
		wareFilter.label.setText(trShort(TidWareFilter));
		applyLayout(newState, true);
	}

	applyLayout(newState);
	updateToolButtonStates();
}

void QueryOptionsView::saveClickedSlot()
{
	mapFromGui();

	QueriesModel & qm = queriesModel(dbname);
	if(qm.querySet().has(query.name)) {
		if(qm.querySet().query(query.name) != query)
			qm.update(qm.index(query.name), query);
	} else
		qm.addNew(query);
	toolBar.setInfo(tr(TidQuerySavedInfo));
	updateToolButtonStates();
}

void QueryOptionsView::delClickedSlot()
{
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a query"),
			tr("Shall we delete this query: ") + query.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes){
		QueriesModel & qm = queriesModel(dbname);
		if(qm.querySet().has(query.name))
			qm.del(qm.index(query.name));
		query = Query();
		mapToGui();
	}
}

void QueryOptionsView::queryClickedSlot()
{
	mapFromGui();

	QueriesModel & qm = queriesModel(dbname);
	if(!qm.querySet().has(query.name))
		query.name = "";
	else if(qm.querySet().query(query.name) != query)
		query.name = "";

	accept();
}

void QueryOptionsView::resetClickedSlot()
{
	mapToGui();
}

void QueryOptionsView::backClickedSlot()
{
	reject();
}

void QueryOptionsView::querySelectedSlot()
{
	QueriesModel & qm = queriesModel(dbname);
	if(!qm.querySet().has(nameEditor.text()))
		return;
	query = qm.querySet().query(nameEditor.text());
	mapToGui();
}

void QueryOptionsView::layoutContentChangeSlot()
{
	updateToolButtonStates();
	relayout();
}

void QueryOptionsView::updateToolButtonStates()
{
	bool modified = !(
			query.name == nameEditor.text() &&
			query.startDate == startDate.edit.dateTime() &&
//			query.endDate == endDate.edit.dateTime() &&

			((bool)(query.wares.size()) ==(wareFilter.box.checkState()==Qt::Checked)) &&
			((bool)(query.shops.size()) ==(shopFilter.box.checkState()==Qt::Checked)) &&
			((bool)(query.withTags.size()) ==(withTagFilter.box.checkState()==Qt::Checked)) &&
			((bool)(query.withoutTags.size()) ==(withoutTagFilter.box.checkState()==Qt::Checked)) &&

			query.withTags == tagsWidget.selectedTags() &&
			query.withoutTags == withoutTagsWidget.selectedTags()
			);
	if(!modified)
		if(query.wares.size() && (wareFilter.box.checkState()==Qt::Checked))
			if(query.wares.queryAt(0) != wareSelector.box.currentText())
				modified = true;
	if(!modified)
		if(query.shops.size() && (shopFilter.box.checkState()==Qt::Checked))
			if(query.shops.queryAt(0) != shopSelector.box.currentText())
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
			case Query::StockOptions::AllBoughtItem :
				if(stockOptions.group.checkedButton() != &stockOptAll)
					modified = true;
				break;
			case Query::StockOptions::ItemsOnStock :
				if(stockOptions.group.checkedButton() != &stockOptOnStock)
					modified = true;
				break;
			case Query::StockOptions::ItemsUsedUp :
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
				case (Query::TagOptions::MatchAll) :
					if(tagOptions.group.checkedButton() != &tagOptAllMatch)
					modified = true;
					break;
				case Query::TagOptions::MatchAny :
					if(tagOptions.group.checkedButton() != &tagOptAnyMatch)
						modified = true;
					break;
				default:
					modified = true;
					break;
			}
		} else if(query.tagOption != Query::TagOptions::MatchAny)
			modified = true;
	}

	bool hasName(nameEditor.text().size());

	/* Lets adjust states */

	saveButton.setVisible(modified && hasName);
	delButton.setVisible(!modified && hasName);
	resetButton.setVisible(modified);

	wareSelector.setVisible(wareFilter.box.checkState()==Qt::Checked);
	shopSelector.setVisible(shopFilter.box.checkState()==Qt::Checked);
	tagOptions.setVisible(withTagFilter.box.checkState()==Qt::Checked);
	tagsWidget.setVisible(withTagFilter.box.checkState()==Qt::Checked);
	withoutTagsWidget.setVisible(withoutTagFilter.box.checkState()==Qt::Checked);

	if(modified)
		toolBar.clearInfo();

	toolBar.updateButtons();
}
