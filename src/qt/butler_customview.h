/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMVIEW_H
#define BUTLER_CUSTOMVIEW_H

#include <butler_widgets.h>

class AccountingView;
class EditItemView;
class QueryOptionsView;
class EditWareView;

class CustomView : public PannView
{
private:
	Q_OBJECT

public:
	CustomView(const QString & dbname, bool selfDestruct = false, QWidget *parent = 0);
	~CustomView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void retranslate();
	void applyLayout();
	void relayout();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	
//	void updateStatistics();

private slots:
	void editItem();
	void delItem();
	void openAccountingView();
	void finishedEditItem(int);
	void filterItems();
	void filterAcceptedSlot();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void editWare();
	void finishedEditWare(int);
	void shoppingItem();

private:
	const QString & dbname;
	csjp::Object<CustomModel> model;
	bool selfDestruct; /* For additionally opened (non-first) custom view. */

	ToolBar toolBar;
	ToolButton addButton;
	ToolButton editButton;
	ToolButton delButton;
	ToolButton shoppigButton;
	ToolButton filterButton;
	ToolButton statsButton;

	TableView tableView;
/*
	QLabel *itemCountLabel;
	QLabel *itemSumQuantityLabel;
	QLabel *itemSumPriceLabel;
	QLabel *avgUnitPriceLabel;
	QLabel *minUnitPriceLabel;
	QLabel *maxUnitPriceLabel;
*/
	AccountingView *accountingView;
	EditItemView *editItemView;
	QueryOptionsView *queryOptsView;
	EditWareView *editWareView;
};

#endif
