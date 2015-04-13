/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_config.h"
#include "butler_listviews.h"

SCC TidCompanyContext = "CompanyView";

SCC TidCompanysWindowTitle = QT_TRANSLATE_NOOP("CompanyView", "Companies");
SCC TidCompanyAddButton = QT_TRANSLATE_NOOP("CompanyView", "Add new company");
SCC TidCompanyEditButton = QT_TRANSLATE_NOOP("CompanyView", "Edit company");
SCC TidCompanyDelButton = QT_TRANSLATE_NOOP("CompanyView", "Delete company");
SCC TidCompanyRefreshButton = QT_TRANSLATE_NOOP("CompanyView", "Refresh company list");

SCC TidPartnerContext = "PartnersView";

SCC TidPartnersWindowTitle = QT_TRANSLATE_NOOP("PartnersView", "Business partners");
SCC TidPartnerAddButton = QT_TRANSLATE_NOOP("PartnersView", "Add new partner");
SCC TidPartnerEditButton = QT_TRANSLATE_NOOP("PartnersView", "Edit partner");
SCC TidPartnerDelButton = QT_TRANSLATE_NOOP("PartnersView", "Delete partner");
SCC TidPartnerRefreshButton = QT_TRANSLATE_NOOP("PartnersView", "Refresh partner list");

SCC TidAccountContext = "AccountsView";

SCC TidAccountsWindowTitle = QT_TRANSLATE_NOOP("AccountsView", "Business accounts");
SCC TidAccountAddButton = QT_TRANSLATE_NOOP("AccountsView", "Add new account");
SCC TidAccountEditButton = QT_TRANSLATE_NOOP("AccountsView", "Edit account");
SCC TidAccountDelButton = QT_TRANSLATE_NOOP("AccountsView", "Delete account");
SCC TidAccountRefreshButton = QT_TRANSLATE_NOOP("AccountsView", "Refresh account list");

SCC TidWareContext = "WaresView";

SCC TidWaresWindowTitle = QT_TRANSLATE_NOOP("WaresView", "Ware/Service list");
SCC TidWareAddButton = QT_TRANSLATE_NOOP("WaresView", "Add new ware or service");
SCC TidWareEditButton = QT_TRANSLATE_NOOP("WaresView", "Edit ware or service");
SCC TidWareDelButton = QT_TRANSLATE_NOOP("WaresView", "Delete ware or service");
SCC TidWareRefreshButton = QT_TRANSLATE_NOOP("WaresView", "Refresh ware/service list");

SCC TidTagContext = "TagsView";

SCC TidTagsWindowTitle = QT_TRANSLATE_NOOP("TagsView", "Tag list");
SCC TidTagAddButton = QT_TRANSLATE_NOOP("TagsView", "Add new tag");
SCC TidTagEditButton = QT_TRANSLATE_NOOP("TagsView", "Edit tag");
SCC TidTagDelButton = QT_TRANSLATE_NOOP("TagsView", "Delete tag");
SCC TidTagRefreshButton = QT_TRANSLATE_NOOP("TagsView", "Refresh tag list");

SCC TidBrandContext = "BrandsView";

SCC TidBrandsWindowTitle = QT_TRANSLATE_NOOP("BrandsView", "Brand list");
SCC TidBrandAddButton = QT_TRANSLATE_NOOP("BrandsView", "Add new brand");
SCC TidBrandEditButton = QT_TRANSLATE_NOOP("BrandsView", "Edit brand");
SCC TidBrandDelButton = QT_TRANSLATE_NOOP("BrandsView", "Delete brand");
SCC TidBrandRefreshButton = QT_TRANSLATE_NOOP("BrandsView", "Refresh brand list");

SCC TidInventoryContext = "InventoriesView";

SCC TidInventorysWindowTitle = QT_TRANSLATE_NOOP("InventoriesView", "Inventory list");
SCC TidInventoryAddButton = QT_TRANSLATE_NOOP("InventoriesView", "Add new inventory");
SCC TidInventoryEditButton = QT_TRANSLATE_NOOP("InventoriesView", "Edit inventory");
SCC TidInventoryDelButton = QT_TRANSLATE_NOOP("InventoriesView", "Delete inventory");
SCC TidInventoryRefreshButton = QT_TRANSLATE_NOOP("InventoriesView", "Refresh inventory list");

@include@ views.cpp

@ForTypes{Company,Partner,Account,Ware,Tag,Brand,Inventory@
@include@ simpleListViewImplementation
@}ForTypes@

