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

csjp::Object<ItemModel> itemModel(const csjp::String & dbname);
csjp::Object<PaymentModel> paymentModel(const csjp::String & dbname);


TagModel & tagModel(const csjp::String & dbname);

WareModel & wareModel(const csjp::String & dbname);

CompanyModel & companyModel(const csjp::String & dbname);

BrandModel & brandModel(const csjp::String & dbname);

InventoryModel & inventoryModel(const csjp::String & dbname);

PartnerModel & partnerModel(const csjp::String & dbname);

AccountModel & accountModel(const csjp::String & dbname);

ItemQueryModel & itemQueryModel(const csjp::String & dbname);

PaymentQueryModel & paymentQueryModel(const csjp::String & dbname);


#endif

