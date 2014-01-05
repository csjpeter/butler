/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MAINVIEW_H
#define BUTLER_MAINVIEW_H

#include <butler_pannview.h>

/*forwards*/
class ShoppingView;
class EditItemView;
class CustomView;
class CompanyView;
class PartnersView;
class AccountsView;
class WaresView;
class TagsView;
class BrandsView;
class InventoriesView;
class DatabasesView;
class QueryOptionsView;
class InfoView;

class MainView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	MainView(QWidget *parent = 0);
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
	void activeDbChanged();
	//void openTodoView();
	//void openShoppingView();
	void openEditItemView();
	void openCustomView();
	void openCompanyView();
	void openPartnersView();
	void openAccountsView();
	void openWaresView();
	void openTagsView();
	void openBrandsView();
	void openInventoriesView();
	void openDatabasesView();
	void openQueryOptionsView();
	void openInfoView();

private:
	InfoLabel infoLabel;

	ToolButton todoButton;
	ToolButton shoppingButton;
	ToolButton newItemButton;
	ToolButton analiticsButton;
	ToolButton companyButton;
	ToolButton partnersButton;
	ToolButton accountsButton;
	ToolButton wareButton;
	ToolButton tagButton;
	ToolButton brandButton;
	ToolButton inventoryButton;
	ToolButton databasesButton;
	ToolButton infoButton;
	ToolButton quitButton;

//	TodoView *todoView;
	ShoppingView *shoppingView;
	EditItemView *newItemView;
	CustomView *customView;
	CompanyView *companyView;
	PartnersView *partnersView;
	AccountsView *accountsView;
	WaresView *waresView;
	TagsView *tagsView;
	BrandsView *brandsView;
	InventoriesView *inventoriesView;
	DatabasesView *databasesView;
	QueryOptionsView *queryOptionsView;
	InfoView *infoView;
};

#endif
