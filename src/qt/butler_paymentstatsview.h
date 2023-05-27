/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_PAYMENTSTATSVIEW_H
#define BUTLER_PAYMENTSTATSVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>

class PaymentStatsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	PaymentStatsView(const PaymentQueryStat & stat, QWidget * parent = 0);
	virtual ~PaymentStatsView() {}

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
	const PaymentQueryStat & stat;

	Label paymentCountLabel;
	Label paymentCountValueLabel;
	Label itemSumPriceLabel;
	Label itemSumPriceValueLabel;
	Label avgPriceLabel;
	Label avgPriceValueLabel;
	Label minPriceLabel;
	Label minPriceValueLabel;
	Label maxPriceLabel;
	Label maxPriceValueLabel;
	Label timeTakenBySqlQueryLabel;
	Label timeTakenBySqlQueryValueLabel;
};

#endif

