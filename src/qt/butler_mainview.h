/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MAINVIEW_H
#define BUTLER_MAINVIEW_H

#include <QWidget>

#include <butler_query.h>
#include <butler_databases.h>

/*forwards*/
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;

class ShoppingView;
class StockView;
class CustomView;
class ShopsView;
class TagsView;
class WaresView;
class QueryOptionsView;
class InfoView;

class MainView : public QWidget
{
private:
	Q_OBJECT

public:
	MainView(const QString & dbname, QWidget *parent = 0);
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
	void openInfoView();

	void about();
	void quit();

private:
	QString dbname;

	ShoppingView *shoppingView;
	StockView *stockView;
	CustomView *customView;
	ShopsView *shopsView;
	TagsView *tagsView;
	WaresView *waresView;
	QueryOptionsView *queryOptionsView;
	InfoView *infoView;
};

#endif
