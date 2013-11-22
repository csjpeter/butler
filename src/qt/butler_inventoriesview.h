/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_INVENTORIESVIEW_H
#define BUTLER_INVENTORIESVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class EditInventoryView;

class InventoriesView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	InventoriesView(const QString & dbname, QWidget * parent = 0);
	virtual ~InventoriesView();

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
	void newInventory();
	void editInventory();
	void delInventory();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const QString dbname;
private:
	InventoriesModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditInventoryView * newInventoryView;
	EditInventoryView * editInventoryView;
};

#endif
