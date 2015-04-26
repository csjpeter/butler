/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_PAYMENTVIEW_H
#define BUTLER_PAYMENTVIEW_H

#include <butler_pannview.h>

class EditItemView;
class EditItemView;
class QueryOptionsView;
class EditWareView;
class StatsView;

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
	void editItem();
	void delItem();
	void shoppingItem();
	void refreshItems();
	void editWare();
	void filterItems();
	void applyNewFilter();
	void statsItems();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	csjp::Object<ItemModel> model;

	ToolButton editButton;
	ToolButton delButton;
	ToolButton shoppingButton;
	ToolButton statsButton;
	ToolButton refreshButton;
	ToolButton filterButton;

	TableView tableView;

	EditItemView *editItemView;
	QueryOptionsView *queryOptsView;
	EditWareView *editWareView;
	StatsView *statsView;
};

#endif
