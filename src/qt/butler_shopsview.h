/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPSVIEW_H
#define BUTLER_SHOPSVIEW_H

#include <QWidget>

#include <butler_query.h>

#include "butler_shopsmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;

namespace Butler {

class NewShopView;
class EditShopView;

class ShopsView : public QWidget
{
private:
	Q_OBJECT

public:
	ShopsView(QWidget *parent = 0);
	~ShopsView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

private slots:
	void newShop();
	void finishedNewShop(int);
	void editShop();
	void finishedEditShop(int);
	void delShop();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	ShopsModel  &model;

	QTableView *queryView;
	QSqlTableModel *queryTable;

	QToolBar *actionTB;

	QToolButton *newTBtn;
	QToolButton *editTBtn;
	QToolButton *delTBtn;

	QAction *newAct;
	QAction *editAct;
	QAction *delAct;

	NewShopView *newShopView;
	EditShopView *editShopView;
};

}

#endif


