/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>
#include <math.h>

#include <QtGui>

#include "butler_config.h"
#include "butler_statsview.h"

StatsView::StatsView(const QueryStat & stat, QWidget * parent) :
	PannView(parent),
	stat(stat),
	backButton(QKeySequence(Qt::ALT + Qt::Key_Escape))
{
	setWindowIcon(QIcon(Path::icon("statistics.png")));
	setWindowModality(Qt::ApplicationModal);
	
	connect(&backButton, SIGNAL(clicked()), this, SLOT(reject()));

	retranslate();
	loadState();
}

void StatsView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
}

void StatsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void StatsView::loadState()
{
	QString prefix("StatsView");
	PannView::loadState(prefix);
}

void StatsView::saveState()
{
	QString prefix("StatsView");
	PannView::saveState(prefix);
}
void StatsView::mapToGui()
{
	itemCountValueLabel.setText(Config::locale.toString(stat.itemCount));
	itemSumQuantityValueLabel.setText(Config::locale.toString(stat.sumQuantity, 'f', 3));
	itemSumPriceValueLabel.setText(Config::locale.toString(stat.sumPrice, 'f', 2));
	avgUnitPriceValueLabel.setText(Config::locale.toString(stat.avgPrice, 'f', 2));
	minUnitPriceValueLabel.setText(Config::locale.toString(stat.cheapestUnitPrice, 'f', 2));
	maxUnitPriceValueLabel.setText(Config::locale.toString(stat.mostExpUnitPrice, 'f', 2));
	timeTakenBySqlQueryValueLabel.setText(Config::locale.toString(stat.queryTime, 'f', 3));
}

void StatsView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void StatsView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize() || !isVisible()))
		return;
	relayout();
}

void StatsView::retranslate()
{
	setWindowTitle(qtTrId(TidStatsWindowTitle));
	backButton.setText(qtTrId(TidBackButtonLabel));

	itemCountLabel      .setText(qtTrId(TidStatsItemCountLabel      ));
	itemSumQuantityLabel.setText(qtTrId(TidStatsItemSumQuantityLabel));
	itemSumPriceLabel   .setText(qtTrId(TidStatsItemSumPriceLabel   ));
	avgUnitPriceLabel   .setText(qtTrId(TidStatsAvgUnitPriceLabel   ));
	minUnitPriceLabel   .setText(qtTrId(TidStatsMinUnitPriceLabel   ));
	maxUnitPriceLabel   .setText(qtTrId(TidStatsMaxUnitPriceLabel   ));
	timeTakenBySqlQueryLabel.setText(qtTrId(TidStatsTimeTakenBySqlQueryLabel));

	relayout();
}

void StatsView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;

	HLayout * toolLayout = new HLayout;
	toolLayout->addStretch(0);
	toolLayout->addWidget(&backButton);

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

	mainLayout->addLayout(toolLayout);
	mainLayout->addLayout(gridLayout);
	mainLayout->addStretch(1);

	setLayout(mainLayout);
}

void StatsView::relayout()
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
