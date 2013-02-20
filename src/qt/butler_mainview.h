/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MAINVIEW_H
#define BUTLER_MAINVIEW_H

#include <QWidget>

#include <butler_query.h>

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

class ShoppingView;
class StockView;
class CustomView;
class ShopsView;
class TagsView;
class WaresView;
class QueryOptionsView;

class MainView : public QWidget
{
private:
	Q_OBJECT

public:
	static MainView& instance();

protected:
	MainView(QWidget *parent = 0);
	~MainView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

private slots:
	void openShoppingView();
	void openStockView();
	void openCustomView();
	void openShopsView();
	void openTagsView();
	void openWaresView();
	void openQueryOptionsView();

	void about();

private:
	QToolBar *actionTB;

	QToolButton *shoppingTBtn;
	QToolButton *stockTBtn;
	QToolButton *customTBtn;
	QToolButton *shopsTBtn;
	QToolButton *tagsTBtn;
	QToolButton *waresTBtn;
//	QToolButton *queryOptionsTBtn;

	QAction *shoppingAct;
	QAction *stockAct;
	QAction *customAct;
	QAction *shopsAct;
	QAction *tagsAct;
	QAction *waresAct;
//	QAction *queryOptionsAct;

	ShoppingView *shoppingView;
	StockView *stockView;
	CustomView *customView;
	ShopsView *shopsView;
	TagsView *tagsView;
	WaresView *waresView;
	QueryOptionsView *queryOptionsView;
};

}

#endif


