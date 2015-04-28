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
SCC TidItemSumPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Summary of paid prices : ");
SCC TidAvgPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Avergae unit price : ");
SCC TidMinPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Minimal unit price : ");
SCC TidMaxPrice	= QT_TRANSLATE_NOOP("PaymentStatsView", "Maximal unit price : ");
SCC TidSqlQueryTime	= QT_TRANSLATE_NOOP("PaymentStatsView", "Time taken by the SQL query : ");

PaymentStatsView::PaymentStatsView(const PaymentQueryStat & stat, QWidget * parent) :
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
	paymentCountValueLabel.setText(Config::locale.toString(stat.paymentCount.val));
	itemSumPriceValueLabel.setText(Config::locale.toString(stat.sumPrice.val, 'f', 2));
	avgPriceValueLabel.setText(Config::locale.toString(stat.avgPrice.val, 'f', 2));
	minPriceValueLabel.setText(Config::locale.toString(stat.cheapestPrice.val, 'f', 2));
	maxPriceValueLabel.setText(Config::locale.toString(stat.mostExpPrice.val, 'f', 2));
	timeTakenBySqlQueryValueLabel.setText(Config::locale.toString(stat.queryTime.val, 'f', 3));
}

void PaymentStatsView::retranslate()
{
	setWindowTitle(tr(TidWindowTitle));

	paymentCountLabel.setText(tr(TidItemCount));
	itemSumPriceLabel.setText(tr(TidItemSumPrice));
	avgPriceLabel.setText(tr(TidAvgPrice));
	minPriceLabel.setText(tr(TidMinPrice));
	maxPriceLabel.setText(tr(TidMaxPrice));
	timeTakenBySqlQueryLabel.setText(tr(TidSqlQueryTime));

	relayout();
}

void PaymentStatsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;

	QGridLayout * gridLayout = new QGridLayout;

	gridLayout->addWidget(&paymentCountLabel, 0, 0, Qt::AlignLeft);
	gridLayout->addWidget(&paymentCountValueLabel, 0, 1, Qt::AlignRight);
	gridLayout->addWidget(&itemSumPriceLabel, 2, 0, Qt::AlignLeft);
	gridLayout->addWidget(&itemSumPriceValueLabel, 2, 1, Qt::AlignRight);
	gridLayout->addWidget(&avgPriceLabel, 3, 0, Qt::AlignLeft);
	gridLayout->addWidget(&avgPriceValueLabel, 3, 1, Qt::AlignRight);
	gridLayout->addWidget(&minPriceLabel, 4, 0, Qt::AlignLeft);
	gridLayout->addWidget(&minPriceValueLabel, 4, 1, Qt::AlignRight);
	gridLayout->addWidget(&maxPriceLabel, 5, 0, Qt::AlignLeft);
	gridLayout->addWidget(&maxPriceValueLabel, 5, 1, Qt::AlignRight);
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
			paymentCountLabel.setWordWrap(false);
			itemSumPriceLabel.setWordWrap(false);
			avgPriceLabel.setWordWrap(false);
			minPriceLabel.setWordWrap(false);
			maxPriceLabel.setWordWrap(false);
			timeTakenBySqlQueryLabel.setWordWrap(false);
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
		case LayoutState::Narrow :
			paymentCountLabel.setWordWrap(true);
			itemSumPriceLabel.setWordWrap(true);
			avgPriceLabel.setWordWrap(true);
			minPriceLabel.setWordWrap(true);
			maxPriceLabel.setWordWrap(true);
			timeTakenBySqlQueryLabel.setWordWrap(true);
			applyLayout();
			if(sizeHint().width() <= width())
				break;
			// falling back to a smaller size
			break;
	}
}
