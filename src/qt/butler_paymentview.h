/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_PAYMENTVIEW_H
#define BUTLER_PAYMENTVIEW_H

#include <butler_pannview.h>

class EditPaymentView;
class PaymentQueryOptionsView;
class EditWareView;
class PaymentStatsView;

class PaymentView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	PaymentView(const csjp::String & dbname, QWidget *parent = 0);
	virtual ~PaymentView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void retranslate();
	void applyLayout();
	void relayout();
	void updateToolButtonStates();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);

private slots:
	void editPayment();
	void delPayment();
	void refreshPayments();
	void filterPayments();
	void applyNewFilter();
	void statsPayments();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	csjp::Object<PaymentModel> model;

	ToolButton editButton;
	ToolButton delButton;
	ToolButton statsButton;
	ToolButton refreshButton;
	ToolButton filterButton;

	TableView tableView;

	EditPaymentView *editPaymentView;
	PaymentQueryOptionsView *queryOptsView;
	PaymentStatsView *statsView;
};

#endif

