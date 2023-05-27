/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_LISTVIEWS_H
#define BUTLER_LISTVIEWS_H

#include <butler_pannview.h>
#include <butler_databases.h>






/*forwards*/
class NewCompanyView;
class EditCompanyView;

class CompanysView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	CompanysView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~CompanysView();

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
	void newCompany();
	void editCompany();
	void delCompany();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	CompanyModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditCompanyView * newCompanyView;
	EditCompanyView * editCompanyView;
};



/*forwards*/
class NewPartnerView;
class EditPartnerView;

class PartnersView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	PartnersView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~PartnersView();

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
	void newPartner();
	void editPartner();
	void delPartner();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	PartnerModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditPartnerView * newPartnerView;
	EditPartnerView * editPartnerView;
};



/*forwards*/
class NewAccountView;
class EditAccountView;

class AccountsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	AccountsView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~AccountsView();

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
	void newAccount();
	void editAccount();
	void delAccount();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	AccountModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditAccountView * newAccountView;
	EditAccountView * editAccountView;
};



/*forwards*/
class NewWareView;
class EditWareView;

class WaresView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	WaresView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~WaresView();

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
	void newWare();
	void editWare();
	void delWare();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	WareModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditWareView * newWareView;
	EditWareView * editWareView;
};



/*forwards*/
class NewTagView;
class EditTagView;

class TagsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	TagsView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~TagsView();

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
	void newTag();
	void editTag();
	void delTag();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	TagModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditTagView * newTagView;
	EditTagView * editTagView;
};



/*forwards*/
class NewBrandView;
class EditBrandView;

class BrandsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	BrandsView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~BrandsView();

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
	void newBrand();
	void editBrand();
	void delBrand();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	BrandModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditBrandView * newBrandView;
	EditBrandView * editBrandView;
};



/*forwards*/
class NewInventoryView;
class EditInventoryView;

class InventorysView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	InventorysView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~InventorysView();

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
	const csjp::String dbname;
private:
	InventoryModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditInventoryView * newInventoryView;
	EditInventoryView * editInventoryView;
};



#endif


