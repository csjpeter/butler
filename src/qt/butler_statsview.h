/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_STATSVIEW_H
#define BUTLER_STATSVIEW_H

#include <butler_item.h>
#include <butler_query.h>
#include <butler_pannview.h>

class StatsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	StatsView(const QueryStat & stat, QWidget * parent = 0);
	virtual ~StatsView() {}

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);

private slots:
	void retranslate();
	void applyLayout();
	void relayout();

private:
	const QueryStat & stat;

	Label itemCountLabel;
	Label itemCountValueLabel;
	Label itemSumQuantityLabel;
	Label itemSumQuantityValueLabel;
	Label itemSumPriceLabel;
	Label itemSumPriceValueLabel;
	Label avgUnitPriceLabel;
	Label avgUnitPriceValueLabel;
	Label minUnitPriceLabel;
	Label minUnitPriceValueLabel;
	Label maxUnitPriceLabel;
	Label maxUnitPriceValueLabel;
	Label timeTakenBySqlQueryLabel;
	Label timeTakenBySqlQueryValueLabel;
};

#endif
