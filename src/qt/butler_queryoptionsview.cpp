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

QueryOptionsView::QueryOptionsView(const QString & dbname, Query &query, QWidget *parent) :
	QDialog(parent),
	dbname(dbname),
	query(query)
{
	QGridLayout *gridLayout = new QGridLayout();
	setLayout(gridLayout);

	QLabel *label = NULL;

	label = new QLabel(tr("Name :"), this);
	gridLayout->addWidget(label, 0, 0, 1, 1);
	nameEditor = new QLineEdit(this);
	gridLayout->addWidget(nameEditor, 0, 1, 1, 3);

	label = new QLabel(tr("Start date :"), this);
	gridLayout->addWidget(label, 1, 0, 1, 1);
	startDate = new QDateTimeEdit(this);
	startDate->setCalendarPopup(true);
	startDate->setDisplayFormat(Config::instance().dateTimeFormat());
	gridLayout->addWidget(startDate, 1, 1, 1, 3);

	label = new QLabel(tr("End date :"), this);
	gridLayout->addWidget(label, 2, 0, 1, 1);
	endDate = new QDateTimeEdit(this);
	endDate->setCalendarPopup(true);
	endDate->setDisplayFormat(Config::instance().dateTimeFormat());
	gridLayout->addWidget(endDate, 2, 1, 1, 3);

	label = new QLabel(tr("Wares (any) :"), this);
	gridLayout->addWidget(label, 3, 0, 1, 1);
	wareFilter = new QCheckBox;
	wareFilter->setTristate(false);
	gridLayout->addWidget(wareFilter, 3, 1, 1, 1);
	wareBox = new QComboBox;
	wareBox->setModel(&waresModel(dbname));
	wareBox->setModelColumn(WaresModel::Name);
	gridLayout->addWidget(wareBox, 3, 2, 1, 2);

	label = new QLabel(tr("Shops (any) :"), this);
	gridLayout->addWidget(label, 4, 0, 1, 1);
	shopFilter = new QCheckBox;
	shopFilter->setTristate(false);
	gridLayout->addWidget(shopFilter, 4, 1, 1, 1);
	shopBox = new QComboBox;
	shopBox->setModel(&shopsModel(dbname));
	shopBox->setModelColumn(Shop::Name);
	gridLayout->addWidget(shopBox, 4, 2, 1, 2);

	label = new QLabel(tr("With these tags :"));
	gridLayout->addWidget(label, 5, 0, 1, 4);
	tagOptions = new QButtonGroup();
	tagOptAllMatch = new QRadioButton;
	tagOptAllMatch->setText(tr("all selected tag need to match"));
	tagOptions->addButton(tagOptAllMatch);
	gridLayout->addWidget(tagOptAllMatch, 6, 1, 1, 2);
	tagOptAnyMatch = new QRadioButton;
	tagOptAnyMatch->setText(tr("any selected tag enough to match"));
	tagOptions->addButton(tagOptAnyMatch);
	gridLayout->addWidget(tagOptAnyMatch, 7, 1, 1, 2);
	tagsSelector = new TagWidget(dbname, this);
	gridLayout->addWidget(tagsSelector, 8, 0, 1, 4);

	selectAllButton = new QPushButton;
	selectAllButton->setAutoDefault(false);
	connect(selectAllButton, SIGNAL(clicked(bool)),
			this, SLOT(selectAllClickedSlot(bool)));
	selectAllButton->setText(tr("Select all tags"));
	gridLayout->addWidget(selectAllButton, 9, 1, 1, 1);

	selectNoneButton = new QPushButton;
	selectNoneButton->setAutoDefault(false);
	connect(selectNoneButton, SIGNAL(clicked(bool)),
			this, SLOT(selectNoneClickedSlot(bool)));
	selectNoneButton->setText(tr("Deselect all tags"));
	gridLayout->addWidget(selectNoneButton, 9, 2, 1, 1);

	label = new QLabel(tr("Without these tags :"));
	gridLayout->addWidget(label, 10, 0, 1, 4);
	withoutTagsSelector = new TagWidget(dbname, this);
	gridLayout->addWidget(withoutTagsSelector, 11, 0, 1, 4);

	label = new QLabel(tr("Stock option :"));
	gridLayout->addWidget(label, 12, 0, 1, 4);
	stockOptions = new QButtonGroup();
	stockOptAll = new QRadioButton;
	stockOptAll->setText(tr("all bought items"));
	stockOptions->addButton(stockOptAll);
	gridLayout->addWidget(stockOptAll, 13, 1, 1, 2);
	stockOptOnStock = new QRadioButton;
	stockOptOnStock->setText(tr("items on stock"));
	stockOptions->addButton(stockOptOnStock);
	gridLayout->addWidget(stockOptOnStock, 14, 1, 1, 2);
	stockOptUsedUp = new QRadioButton;
	stockOptUsedUp->setText(tr("items used up"));
	stockOptions->addButton(stockOptUsedUp);
	gridLayout->addWidget(stockOptUsedUp, 15, 1, 1, 2);

	okButton = new QPushButton;
	okButton->setAutoDefault(false);
	okButton->setDefault(true);
	connect(okButton, SIGNAL(clicked(bool)), this, SLOT(okClickedSlot(bool)));
	okButton->setText(tr("Ok"));
	gridLayout->addWidget(okButton, 16, 3, 1, 1);

	/* restore last state */
	loadState();
}

QueryOptionsView::~QueryOptionsView()
{
}

void QueryOptionsView::showEvent(QShowEvent *event)
{
	tagsModel(dbname).query();
	
	mapToGui();

	QWidget::showEvent(event);
}

void QueryOptionsView::closeEvent(QCloseEvent *event)
{
	saveState();

	QWidget::closeEvent(event);
}

void QueryOptionsView::loadState()
{
	QSettings settings(this);
	QPoint pos = settings.value("queryoptionsview/position", QPoint()).toPoint();
	QSize size = settings.value("queryoptionsview/size", QSize()).toSize();
	if(size.isValid())
		resize(size);
	move(pos);
}

void QueryOptionsView::saveState()
{
	QSettings settings(this);
	settings.setValue("queryoptionsview/position", pos());
	settings.setValue("queryoptionsview/size", size());
}

void QueryOptionsView::mapToGui()
{
	nameEditor->setText(query.name);
	startDate->setDateTime(query.startDate);
	endDate->setDateTime(query.endDate);


/* FIXME *///		waresEditor->setText(stringSetToString(query.wares));
/* FIXME *///		shopsEditor->setText(stringSetToString(query.shops));
		
		
	tagsSelector->setTags(query.withTags);
	withoutTagsSelector->setTags(query.withoutTags);

	if(query.stockOption == Query::ALL_BOUGHT_ITEM)
		stockOptAll->setChecked(true);
	else if(query.stockOption == Query::ITEMS_ON_STOCK)
		stockOptOnStock->setChecked(true);
	else if(query.stockOption == Query::ITEMS_USED_UP)
		stockOptUsedUp->setChecked(true);

	if(query.tagOption == Query::MATCH_ALL_TAGS)
		tagOptAllMatch->setChecked(true);
	else if(query.tagOption == Query::MATCH_ANY_TAGS)
		tagOptAnyMatch->setChecked(true);
}

void QueryOptionsView::mapFromGui()
{
	query.name = nameEditor->text();
	query.startDate = startDate->dateTime();
	query.endDate = endDate->dateTime();

	query.wares.clear();
	if(wareFilter->isChecked()){
		int i = wareBox->currentIndex();
		WaresModel &wm = waresModel(dbname);
		if(0 <= i && i < wm.rowCount())
			query.wares.add(new QString(wm.ware(i).name.trimmed()));
	}
	
	query.shops.clear();
	if(shopFilter->isChecked()){
		int i = shopBox->currentIndex();
		ShopsModel &sm = shopsModel(dbname);
		if(0 <= i && i < sm.rowCount())
			query.shops.add(new QString(sm.shop(i).name.trimmed()));
	}

	tagsSelector->getTags(query.withTags);
	withoutTagsSelector->getTags(query.withoutTags);

	if(stockOptions->checkedButton() == stockOptAll)
		query.stockOption = Query::ALL_BOUGHT_ITEM;
	else if(stockOptions->checkedButton() == stockOptOnStock)
		query.stockOption = Query::ITEMS_ON_STOCK;
	else if(stockOptions->checkedButton() == stockOptUsedUp)
		query.stockOption = Query::ITEMS_USED_UP;

	if(tagOptions->checkedButton() == tagOptAllMatch)
		query.tagOption = Query::MATCH_ALL_TAGS;
	else if(tagOptions->checkedButton() == tagOptAnyMatch)
		query.tagOption = Query::MATCH_ANY_TAGS;
}

void QueryOptionsView::okClickedSlot(bool)
{
	mapFromGui();

	accept();
}
/* Might be usefull later when we want to support multiple items in shop or ware set.
QString QueryOptionsView::stringSetToString(
		const csjp::OwnerContainer<QString> &strSet)
{
	QString result("");

	int s = strSet.size();
	if(s == 0)
		return result;

	result = strSet.queryAt(0);
	int i;
	for(i=1; i<s; i++){
		result += ", ";
		result += strSet.queryAt(i);
	}

	return result;
}
*/

void QueryOptionsView::selectAllClickedSlot(bool)
{
	tagsSelector->selectAll();
}

void QueryOptionsView::selectNoneClickedSlot(bool)
{
	tagsSelector->deselectAll();
}
