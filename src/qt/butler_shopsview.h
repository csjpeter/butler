/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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


