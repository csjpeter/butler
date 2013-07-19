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
class EditItemView;
class CustomView;
class PartnersView;
class CompanyView;
class WaresView;
class TagsView;
class DatabasesView;
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
	void openEditItemView();
	void openCustomView();
	void openPartnersView();
	void openCompanyView();
	void openWaresView();
	void openTagsView();
	void openDatabasesView();
	void openQueryOptionsView();
	void openInfoView();

private:
	QString dbname;
	csjp::Object<CustomModel> model;

	ToolButton todoButton;
	ToolButton shoppingButton;
	ToolButton newItemButton;
	ToolButton analiticsButton;
	ToolButton partnersButton;
	ToolButton companyButton;
	ToolButton wareButton;
	ToolButton tagButton;
	ToolButton databasesButton;
	ToolButton infoButton;
	ToolButton quitButton;

//	TodoView *todoView;
	ShoppingView *shoppingView;
	EditItemView *newItemView;
	CustomView *customView;
	PartnersView *partnersView;
	CompanyView *companyView;
	WaresView *waresView;
	TagsView *tagsView;
	DatabasesView *databasesView;
	QueryOptionsView *queryOptionsView;
	InfoView *infoView;
};

#endif
