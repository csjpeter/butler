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
TagModel & tagModel(const csjp::String & dbname);
WareModel & wareModel(const csjp::String & dbname);
CompanyModel & companyModel(const csjp::String & dbname);
BrandModel & brandModel(const csjp::String & dbname);
InventoryModel & inventoryModel(const csjp::String & dbname);
PartnerModel & partnersModel(const csjp::String & dbname);
AccountModel & accountModel(const csjp::String & dbname);
QueryModel & queryModel(const csjp::String & dbname);
//ShoppingModel & shoppingModel(const csjp::String & dbname);
csjp::Object<ItemModel> itemModel(const csjp::String & dbname);

#endif
