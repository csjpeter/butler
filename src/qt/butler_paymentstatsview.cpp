/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_paymentstatsview.h"

SCC TidWindowTitle	= QT_TRANSLATE_NOOP("PaymentStatsView", "Statistics");
SCC TidItemCount	= QT_TRANSLATE_NOOP("PaymentStatsView", "Number of queried items : ");
SCC TidItemSumQuantity	= QT_TRANSLATE_NOOP("PaymentStatsView", "Summary of queried quantites : ");
SCC TidItemSumPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Summary of paid prices : ");
SCC TidAvgUnitPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Avergae unit price : ");
SCC TidMinUnitPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Minimal unit price : ");
SCC TidMaxUnitPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Maximal unit price : ");
SCC TidSqlQueryTime	= QT_TRANSLATE_NOOP("PaymentStatsView", "Time taken by the SQL query : ");

PaymentStatsView::PaymentStatsView(const QueryStat & stat, QWidget * parent) :
	PannView(parent),
	stat(stat)
{
	setWindowIcon(QIcon(Path::icon("statistics.png")));
	setWindowModality(Qt::ApplicationModal);

	setupView();
	retranslate();
	loadState();
}

void PaymentStatsView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
}

void PaymentStatsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PaymentStatsView::loadState()
{
	QString prefix("PaymentStatsView");
	PannView::loadState(prefix);
}

void PaymentStatsView::saveState()
{
	QString prefix("PaymentStatsView");
	PannView::saveState(prefix);
}

void PaymentStatsView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PaymentStatsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize() || !isVisible()))
		return;
	relayout();
}

void PaymentStatsView::mapToGui()
{
	itemCountValueLabel.setText(Config::locale.toString(stat.itemCount.val));
	itemSumQuantityValueLabel.setText(Config::locale.toString(stat.sumQuantity.val, 'f', 3));
	itemSumPriceValueLabel.setText(Config::locale.toString(stat.sumPrice.val, 'f', 2));
	avgUnitPriceValueLabel.setText(Config::locale.toString(stat.avgPrice.val, 'f', 2));
	minUnitPriceValueLabel.setText(Config::locale.toString(stat.cheapestUnitPrice.val, 'f', 2));
	maxUnitPriceValueLabel.setText(Config::locale.toString(stat.mostExpUnitPrice.val, 'f', 2));
	timeTakenBySqlQueryValueLabel.setText(Config::locale.toString(stat.queryTime.val, 'f', 3));
}

void PaymentStatsView::retranslate()
{
	setWindowTitle(tr(TidWindowTitle));

	itemCountLabel.setText(tr(TidItemCount));
	itemSumQuantityLabel.setText(tr(TidItemSumQuantity));
	itemSumPriceLabel.setText(tr(TidItemSumPrice));
	avgUnitPriceLabel.setText(tr(TidAvgUnitPrice));
	minUnitPriceLabel.setText(tr(TidMinUnitPrice));
	maxUnitPriceLabel.setText(tr(TidMaxUnitPrice));
	timeTakenBySqlQueryLabel.setText(tr(TidSqlQueryTime));

	relayout();
}

void PaymentStatsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;

	QGridLayout * gridLayout = new QGridLayout;

	gridLayout->addWidget(&itemCountLabel, 0, 0, Qt::AlignLeft);
	gridLayout->addWidget(&itemCountValueLabel, 0, 1, Qt::AlignRight);
	gridLayout->addWidget(&itemSumQuantityLabel, 1, 0, Qt::AlignLeft);
	gridLayout->addWidget(&itemSumQuantityValueLabel, 1, 1, Qt::AlignRight);
	gridLayout->addWidget(&itemSumPriceLabel, 2, 0, Qt::AlignLeft);
	gridLayout->addWidget(&itemSumPriceValueLabel, 2, 1, Qt::AlignRight);
	gridLayout->addWidget(&avgUnitPriceLabel, 3, 0, Qt::AlignLeft);
	gridLayout->addWidget(&avgUnitPriceValueLabel, 3, 1, Qt::AlignRight);
	gridLayout->addWidget(&minUnitPriceLabel, 4, 0, Qt::AlignLeft);
	gridLayout->addWidget(&minUnitPriceValueLabel, 4, 1, Qt::AlignRight);
	gridLayout->addWidget(&maxUnitPriceLabel, 5, 0, Qt::AlignLeft);
	gridLayout->addWidget(&maxUnitPriceValueLabel, 5, 1, Qt::AlignRight);
/*	gridLayout->addWidget(&timeTakenBySqlQueryLabel, 6, 0, Qt::AlignLeft);
	gridLayout->addWidget(&timeTakenBySqlQueryValueLabel, 6, 1, Qt::AlignRight);*/

	gridLayout->setColumnStretch(0, 3);
	gridLayout->setColumnStretch(1, -1);

	mainLayout->addLayout(gridLayout);
	mainLayout->addStretch(1);

	setLayout(mainLayout);
}

void PaymentStatsView::relayout()
{
	LayoutState newState = LayoutState::Wide;
	QSize newSize;

	switch(newState) {
		case LayoutState::Expanding :
		case LayoutState::Wide :
		case LayoutState::Medium :
			itemCountLabel.setWordWrap(false);
			itemSumQuantityLabel.setWordWrap(false);
			itemSumPriceLabel.setWordWrap(false);
			avgUnitPriceLabel.setWordWrap(false);
			minUnitPriceLabel.setWordWrap(false);
			maxUnitPriceLabel.setWordWrap(false);
			timeTakenBySqlQueryLabel.setWordWrap(false);
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case LayoutState::Narrow :
			itemCountLabel.setWordWrap(true);
			itemSumQuantityLabel.setWordWrap(true);
			itemSumPriceLabel.setWordWrap(true);
			avgUnitPriceLabel.setWordWrap(true);
			minUnitPriceLabel.setWordWrap(true);
			maxUnitPriceLabel.setWordWrap(true);
			timeTakenBySqlQueryLabel.setWordWrap(true);
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
			break;
	}
}
