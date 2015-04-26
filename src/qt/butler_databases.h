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

@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
@Type@Model & @type@Model(const csjp::String & dbname);
@}ForTypes@

#endif
