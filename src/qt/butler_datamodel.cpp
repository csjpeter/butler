/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include <butler_datamodel.h>

SCC TidDatabaseDescriptorFieldName		= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Name");
SCC TidDatabaseDescriptorFieldDriver	= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Driver");
SCC TidDatabaseDescriptorFieldDatabaseName= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Database name");
SCC TidDatabaseDescriptorFieldUsername	= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Username");
SCC TidDatabaseDescriptorFieldPassword	= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Password");
SCC TidDatabaseDescriptorFieldSavePassword	= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Save password");
SCC TidDatabaseDescriptorFieldHost		= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Host");
SCC TidDatabaseDescriptorFieldPort		= QT_TRANSLATE_NOOP("DatabaseDescriptorModel", "Port");

@declare@ DatabaseDescriptor
@include@ datamodel.cpp

SCC TidTagFieldName			= QT_TRANSLATE_NOOP("TagModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagModel", "Description");
SCC TidTagFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidTagFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");

@declare@ Tag
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidWareTagFieldWare			= QT_TRANSLATE_NOOP("WareTagModel", "Ware");
SCC TidWareTagFieldTag			= QT_TRANSLATE_NOOP("WareTagModel", "Tag");
SCC TidWareTagFieldLastModified	= QT_TRANSLATE_NOOP("WareTagModel", "Last modification date");
SCC TidWareTagFieldDeleted		= QT_TRANSLATE_NOOP("WareTagModel", "Is deleted");

@declare@ WareTag
@include@ datamodel.cpp

SCC TidWareTypeFieldWare			= QT_TRANSLATE_NOOP("WareTypeModel", "Ware");
SCC TidWareTypeFieldType			= QT_TRANSLATE_NOOP("WareTypeModel", "Type");
SCC TidWareTypeFieldLastModified	= QT_TRANSLATE_NOOP("WareTypeModel", "Last modification date");
SCC TidWareTypeFieldDeleted			= QT_TRANSLATE_NOOP("WareTypeModel", "Is deleted");

@declare@ WareType
@include@ datamodel.cpp

SCC TidWareFieldName			= QT_TRANSLATE_NOOP("WareModel", "Name");
SCC TidWareFieldUnit			= QT_TRANSLATE_NOOP("WareModel", "Unit");
SCC TidWareFieldIcon			= QT_TRANSLATE_NOOP("WareModel", "Icon");
SCC TidWareFieldTypes			= QT_TRANSLATE_NOOP("WareModel", "Types");
SCC TidWareFieldTags			= QT_TRANSLATE_NOOP("WareModel", "Tags");
SCC TidWareFieldLastModified	= QT_TRANSLATE_NOOP("WareModel", "Last modification date");
SCC TidWareFieldDeleted			= QT_TRANSLATE_NOOP("WareModel", "Is deleted");

@declare@ Ware
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidCompanyFieldName			= QT_TRANSLATE_NOOP("CompaniesModel", "Name");
SCC TidCompanyFieldCountry		= QT_TRANSLATE_NOOP("CompaniesModel", "Country");
SCC TidCompanyFieldCity			= QT_TRANSLATE_NOOP("CompaniesModel", "City");
SCC TidCompanyFieldPostalCode	= QT_TRANSLATE_NOOP("CompaniesModel", "Postal code");
SCC TidCompanyFieldAddress		= QT_TRANSLATE_NOOP("CompaniesModel", "Address");
SCC TidCompanyFieldTaxId		= QT_TRANSLATE_NOOP("CompaniesModel", "Tax Id");
SCC TidCompanyFieldIcon			= QT_TRANSLATE_NOOP("CompaniesModel", "Icon");
SCC TidCompanyFieldLastModified	= QT_TRANSLATE_NOOP("CompaniesModel", "Last modification date");
SCC TidCompanyFieldDeleted		= QT_TRANSLATE_NOOP("CompaniesModel", "Is deleted");

@declare@ Company
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidBrandFieldName			= QT_TRANSLATE_NOOP("BrandModel", "Name");
SCC TidBrandFieldCompany		= QT_TRANSLATE_NOOP("BrandModel", "Company");
SCC TidBrandFieldLastModified	= QT_TRANSLATE_NOOP("BrandModel", "Last modification date");
SCC TidBrandFieldDeleted		= QT_TRANSLATE_NOOP("BrandModel", "Is deleted");

@declare@ Brand
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidInventoryFieldName			= QT_TRANSLATE_NOOP("InventoryModel", "Name");
SCC TidInventoryFieldComment		= QT_TRANSLATE_NOOP("InventoryModel", "Comment");
SCC TidInventoryFieldLastModified	= QT_TRANSLATE_NOOP("InventoryModel", "Last modification date");
SCC TidInventoryFieldDeleted		= QT_TRANSLATE_NOOP("InventoryModel", "Is deleted");

@declare@ Inventory
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidPartnerFieldName			= QT_TRANSLATE_NOOP("PartnerModel", "Name");
SCC TidPartnerFieldCountry		= QT_TRANSLATE_NOOP("PartnerModel", "Country");
SCC TidPartnerFieldCity			= QT_TRANSLATE_NOOP("PartnerModel", "City");
SCC TidPartnerFieldPostalCode	= QT_TRANSLATE_NOOP("PartnerModel", "Postal code");
SCC TidPartnerFieldAddress		= QT_TRANSLATE_NOOP("PartnerModel", "Address");
SCC TidPartnerFieldCompany		= QT_TRANSLATE_NOOP("PartnerModel", "Company");
SCC TidPartnerFieldStoreName	= QT_TRANSLATE_NOOP("PartnerModel", "Store/Agent");
SCC TidPartnerFieldLastModified	= QT_TRANSLATE_NOOP("PartnerModel", "Last modification date");
SCC TidPartnerFieldDeleted		= QT_TRANSLATE_NOOP("PartnerModel", "Is deleted");

@declare@ Partner
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidAccountFieldName			= QT_TRANSLATE_NOOP("AccountModel", "Name");
SCC TidAccountFieldCurrency		= QT_TRANSLATE_NOOP("AccountModel", "Currency");
SCC TidAccountFieldBankOffice	= QT_TRANSLATE_NOOP("AccountModel", "Bank Office");
SCC TidAccountFieldIban			= QT_TRANSLATE_NOOP("AccountModel", "IBAN");
SCC TidAccountFieldSwiftCode	= QT_TRANSLATE_NOOP("AccountModel", "Swift code");
SCC TidAccountFieldLastModified	= QT_TRANSLATE_NOOP("AccountModel", "Last modification date");
SCC TidAccountFieldDeleted		= QT_TRANSLATE_NOOP("AccountModel", "Is deleted");

@declare@ Account
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidPaymentFieldUploadDate	= QT_TRANSLATE_NOOP("PaymentModel", "Upload date");
SCC TidPaymentFieldAccount		= QT_TRANSLATE_NOOP("PaymentModel", "Account");
SCC TidPaymentFieldPartner		= QT_TRANSLATE_NOOP("PaymentModel", "Partner");
SCC TidPaymentFieldAmount		= QT_TRANSLATE_NOOP("PaymentModel", "Amount");
SCC TidPaymentFieldSubject		= QT_TRANSLATE_NOOP("PaymentModel", "Subject");
SCC TidPaymentFieldPayDate		= QT_TRANSLATE_NOOP("PaymentModel", "Payment date");
SCC TidPaymentFieldLastModified	= QT_TRANSLATE_NOOP("PaymentModel", "Last modification date");
SCC TidPaymentFieldDeleted		= QT_TRANSLATE_NOOP("PaymentModel", "Is deleted");

@declare@ Payment
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidItemFieldUploadDate		= QT_TRANSLATE_NOOP("ItemModel", "Upload date");
SCC TidItemFieldName			= QT_TRANSLATE_NOOP("ItemModel", "Common name");
SCC TidItemFieldUnit			= QT_TRANSLATE_NOOP("ItemModel", "Unit");
SCC TidItemFieldType			= QT_TRANSLATE_NOOP("ItemModel", "Type");
SCC TidItemFieldBrand			= QT_TRANSLATE_NOOP("ItemModel", "Brand");
SCC TidItemFieldQuantity		= QT_TRANSLATE_NOOP("ItemModel", "Quantity");
SCC TidItemFieldPrice			= QT_TRANSLATE_NOOP("ItemModel", "Price");
SCC TidItemFieldCurrency		= QT_TRANSLATE_NOOP("ItemModel", "Currency");
SCC TidItemFieldAccount			= QT_TRANSLATE_NOOP("ItemModel", "Account");
SCC TidItemFieldPartner			= QT_TRANSLATE_NOOP("ItemModel", "Partner");
SCC TidItemFieldInventory		= QT_TRANSLATE_NOOP("ItemModel", "Inventory");
SCC TidItemFieldComment			= QT_TRANSLATE_NOOP("ItemModel", "Comment");
SCC TidItemFieldInvChangeDate	= QT_TRANSLATE_NOOP("ItemModel", "Change date");
SCC TidItemFieldLastModified	= QT_TRANSLATE_NOOP("ItemModel", "Last modification date");
SCC TidItemFieldDeleted			= QT_TRANSLATE_NOOP("ItemModel", "Is deleted");
SCC TidItemFieldUnitPrice		= QT_TRANSLATE_NOOP("ItemModel", "Unit price");
SCC TidItemFieldQuantityWithUnit= QT_TRANSLATE_NOOP("ItemModel", "Quantity");

@declare@ Item
@include@ dbdatamodel.cpp

ItemModel::ItemModel(SqlConnection & sql, const WareModel & wmodel) :
	sql(sql),
	db(sql),
	set(dataSet),
	wmodel(wmodel)
{
	operationListeners.add(*this);
	opts.endDate = DateTime::now();
	opts.tagOption = QueryTagOptions::MatchAny;
}

void ItemModel::query()
{
	ModelResetGuard g(this);
	db.query(opts, stat, dataSet);
}

bool ItemModel::queryFilter(const Item & modified)
{
	(void)(modified);
	return true;
}

SCC TidQueryWithTagFieldQuery		= QT_TRANSLATE_NOOP("QueryWithTagModel", "Query");
SCC TidQueryWithTagFieldTag			= QT_TRANSLATE_NOOP("QueryWithTagModel", "Tag");
SCC TidQueryWithTagFieldLastModified= QT_TRANSLATE_NOOP("QueryWithTagModel", "Last modification date");
SCC TidQueryWithTagFieldDeleted		= QT_TRANSLATE_NOOP("QueryWithTagModel", "Is deleted");

@declare@ QueryWithTag
@include@ datamodel.cpp

SCC TidQueryWithoutTagFieldQuery			= QT_TRANSLATE_NOOP("QueryWithoutTagModel", "Query");
SCC TidQueryWithoutTagFieldTag			= QT_TRANSLATE_NOOP("QueryWithoutTagModel", "Tag");
SCC TidQueryWithoutTagFieldLastModified	= QT_TRANSLATE_NOOP("QueryWithoutTagModel", "Last modification date");
SCC TidQueryWithoutTagFieldDeleted		= QT_TRANSLATE_NOOP("QueryWithoutTagModel", "Is deleted");

@declare@ QueryWithoutTag
@include@ datamodel.cpp

SCC TidQueryWareFieldQuery			= QT_TRANSLATE_NOOP("QueryWareModel", "Query");
SCC TidQueryWareFieldWare			= QT_TRANSLATE_NOOP("QueryWareModel", "Ware");
SCC TidQueryWareFieldLastModified	= QT_TRANSLATE_NOOP("QueryWareModel", "Last modification date");
SCC TidQueryWareFieldDeleted		= QT_TRANSLATE_NOOP("QueryWareModel", "Is deleted");

@declare@ QueryWare
@include@ datamodel.cpp

SCC TidQueryPartnerFieldQuery		= QT_TRANSLATE_NOOP("QueryPartnerModel", "Query");
SCC TidQueryPartnerFieldPartner		= QT_TRANSLATE_NOOP("QueryPartnerModel", "Partner");
SCC TidQueryPartnerFieldLastModified= QT_TRANSLATE_NOOP("QueryPartnerModel", "Last modification date");
SCC TidQueryPartnerFieldDeleted		= QT_TRANSLATE_NOOP("QueryPartnerModel", "Is deleted");

@declare@ QueryPartner
@include@ datamodel.cpp

SCC TidQueryFieldName			= QT_TRANSLATE_NOOP("QueryModel", "Name");
SCC TidQueryFieldStartDate		= QT_TRANSLATE_NOOP("QueryModel", "From date");
SCC TidQueryFieldEndDate		= QT_TRANSLATE_NOOP("QueryModel", "Till date");
SCC TidQueryFieldStockOption	= QT_TRANSLATE_NOOP("QueryModel", "Stock option");
SCC TidQueryFieldTagOption		= QT_TRANSLATE_NOOP("QueryModel", "Tag option");
SCC TidQueryFieldWithTags		= QT_TRANSLATE_NOOP("QueryModel", "With tags");
SCC TidQueryFieldWithoutTags	= QT_TRANSLATE_NOOP("QueryModel", "Without tags");
SCC TidQueryFieldWares			= QT_TRANSLATE_NOOP("QueryModel", "Wares");
SCC TidQueryFieldPartners		= QT_TRANSLATE_NOOP("QueryModel", "Partners");
SCC TidQueryFieldLastModified	= QT_TRANSLATE_NOOP("QueryModel", "Last modification date");
SCC TidQueryFieldDeleted		= QT_TRANSLATE_NOOP("QueryModel", "Is deleted");

@declare@ Query
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp


