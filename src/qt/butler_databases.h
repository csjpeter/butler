/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASES_H
#define BUTLER_DATABASES_H

#include <csjp_exception.h>
#include <csjp_object.h>

#include <butler_datamodel.h>

void loadDatabaseConfigs();
void saveDatabaseConfigs();

DatabaseDescriptorModel & databasesModel();
TagModel & tagModel(const QString & dbname);
WareModel & wareModel(const QString & dbname);
CompanyModel & companyModel(const QString & dbname);
BrandModel & brandModel(const QString & dbname);
InventoryModel & inventoryModel(const QString & dbname);
PartnerModel & partnersModel(const QString & dbname);
AccountModel & accountModel(const QString & dbname);
QueryModel & queryModel(const QString & dbname);
//ShoppingModel & shoppingModel(const QString & dbname);
csjp::Object<ItemModel> itemModel(const QString & dbname);

#endif
