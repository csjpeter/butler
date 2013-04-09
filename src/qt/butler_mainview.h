/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MAINVIEW_H
#define BUTLER_MAINVIEW_H

#include <butler_pannview.h>
#include <butler_query.h>
#include <butler_databases.h>

/*forwards*/
class ShoppingView;
class CustomView;
class ShopsView;
class TagsView;
class WaresView;
class QueryOptionsView;
class InfoView;

class MainView : public PannView
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

	QShortcut shortcut;

	ShoppingView *shoppingView;
	ShopsView *shopsView;
	TagsView *tagsView;
	WaresView *waresView;
	CustomView *customView;
	QueryOptionsView *queryOptionsView;
	InfoView *infoView;
};

#endif
