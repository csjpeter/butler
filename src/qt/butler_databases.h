/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASES_H
#define BUTLER_DATABASES_H

#include <csjp_exception.h>
#include <csjp_object.h>

#include <butler_datamodel.h>

#include <butler_tagsmodel.h>
#include <butler_brandsmodel.h>
#include <butler_waresmodel.h>
#include <butler_companiesmodel.h>
#include <butler_brandsmodel.h>
#include <butler_inventoriesmodel.h>
#include <butler_accountsmodel.h>
#include <butler_partnersmodel.h>
#include <butler_queriesmodel.h>
#include <butler_itemsmodel.h>

void loadDatabaseConfigs();
void saveDatabaseConfigs();

TagModel & tagModel(const QString & dbname);
WareModel & wareModel(const QString & dbname);
CompaniesModel & companiesModel(const QString & dbname);
BrandsModel & brandsModel(const QString & dbname);
InventoriesModel & inventoriesModel(const QString & dbname);
AccountsModel & accountsModel(const QString & dbname);
PartnersModel & partnersModel(const QString & dbname);
QueriesModel & queriesModel(const QString & dbname);
//ShoppingModel & shoppingModel(const QString & dbname);
csjp::Object<ItemModel> itemModel(const QString & dbname);

#endif
