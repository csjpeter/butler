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
class AccountingView;
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
	MY_Q_OBJECT

public:
	MainView(const QString & dbname, QWidget *parent = 0);
	virtual ~MainView();

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

private slots:
	void activateSavedActiveWindow();
	void openTodoView();
	void openShoppingView();
	void openAccountingView();
	void openCustomView();
	void openShopsView();
	void openWaresView();
	void openTagsView();
	void openQueryOptionsView();
	void openInfoView();

private:
	QString dbname;
	csjp::Object<CustomModel> model;

	ToolButton todoButton;
	ToolButton shoppingButton;
	ToolButton accountingButton;
	ToolButton analiticsButton;
	ToolButton partnersButton;
	ToolButton wareButton;
	ToolButton tagButton;
	ToolButton infoButton;
	ToolButton quitButton;

//	TodoView *todoView;
	ShoppingView *shoppingView;
	AccountingView *accountingView;
	CustomView *customView;
	ShopsView *shopsView;
	WaresView *waresView;
	TagsView *tagsView;
	QueryOptionsView *queryOptionsView;
	InfoView *infoView;
};

#endif
