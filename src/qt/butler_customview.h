/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CUSTOMVIEW_H
#define BUTLER_CUSTOMVIEW_H

#include <butler_pannview.h>

class AccountingView;
class EditItemView;
class QueryOptionsView;
class EditWareView;
class StatsView;

class CustomView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	CustomView(const QString & dbname, QWidget *parent = 0);
	virtual ~CustomView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void retranslate();
	void applyLayout();
	void relayout();

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
	void updateToolButtonStates();
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

private:
	const QString & dbname;
	csjp::Object<CustomModel> model;

	ToolButton editButton;
	ToolButton delButton;
	ToolButton shoppingButton;
	ToolButton statsButton;
	ToolButton refreshButton;
	ToolButton filterButton;

	TableView tableView;

	AccountingView *accountingView;
	AccountingView *editItemView;
	QueryOptionsView *queryOptsView;
	EditWareView *editWareView;
	StatsView *statsView;
};

#endif
