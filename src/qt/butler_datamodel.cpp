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

SCC TidTagFieldName			= QT_TRANSLATE_NOOP("TagModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagModel", "Description");
SCC TidTagFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidTagFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");

SCC TidWareTagFieldWare			= QT_TRANSLATE_NOOP("WareTagModel", "Ware");
SCC TidWareTagFieldTag			= QT_TRANSLATE_NOOP("WareTagModel", "Tag");
SCC TidWareTagFieldLastModified	= QT_TRANSLATE_NOOP("WareTagModel", "Last modification date");
SCC TidWareTagFieldDeleted		= QT_TRANSLATE_NOOP("WareTagModel", "Is deleted");

SCC TidWareTypeFieldWare			= QT_TRANSLATE_NOOP("WareTypeModel", "Ware");
SCC TidWareTypeFieldType			= QT_TRANSLATE_NOOP("WareTypeModel", "Type");
SCC TidWareTypeFieldLastModified	= QT_TRANSLATE_NOOP("WareTypeModel", "Last modification date");
SCC TidWareTypeFieldDeleted			= QT_TRANSLATE_NOOP("WareTypeModel", "Is deleted");

SCC TidWareFieldName			= QT_TRANSLATE_NOOP("WareModel", "Name");
SCC TidWareFieldUnit			= QT_TRANSLATE_NOOP("WareModel", "Unit");
SCC TidWareFieldIcon			= QT_TRANSLATE_NOOP("WareModel", "Icon");
SCC TidWareFieldTypes			= QT_TRANSLATE_NOOP("WareModel", "Types");
SCC TidWareFieldTags			= QT_TRANSLATE_NOOP("WareModel", "Tags");
SCC TidWareFieldLastModified	= QT_TRANSLATE_NOOP("WareModel", "Last modification date");
SCC TidWareFieldDeleted			= QT_TRANSLATE_NOOP("WareModel", "Is deleted");

SCC TidCompanyFieldName			= QT_TRANSLATE_NOOP("CompanyModel", "Name");
SCC TidCompanyFieldCountry		= QT_TRANSLATE_NOOP("CompanyModel", "Country");
SCC TidCompanyFieldCity			= QT_TRANSLATE_NOOP("CompanyModel", "City");
SCC TidCompanyFieldPostalCode	= QT_TRANSLATE_NOOP("CompanyModel", "Postal code");
SCC TidCompanyFieldAddress		= QT_TRANSLATE_NOOP("CompanyModel", "Address");
SCC TidCompanyFieldTaxId		= QT_TRANSLATE_NOOP("CompanyModel", "Tax Id");
SCC TidCompanyFieldIcon			= QT_TRANSLATE_NOOP("CompanyModel", "Icon");
SCC TidCompanyFieldLastModified	= QT_TRANSLATE_NOOP("CompanyModel", "Last modification date");
SCC TidCompanyFieldDeleted		= QT_TRANSLATE_NOOP("CompanyModel", "Is deleted");

SCC TidBrandFieldName			= QT_TRANSLATE_NOOP("BrandModel", "Name");
SCC TidBrandFieldCompany		= QT_TRANSLATE_NOOP("BrandModel", "Company");
SCC TidBrandFieldIcon			= QT_TRANSLATE_NOOP("BrandModel", "Icon");
SCC TidBrandFieldLastModified	= QT_TRANSLATE_NOOP("BrandModel", "Last modification date");
SCC TidBrandFieldDeleted		= QT_TRANSLATE_NOOP("BrandModel", "Is deleted");

SCC TidInventoryFieldName			= QT_TRANSLATE_NOOP("InventoryModel", "Name");
SCC TidInventoryFieldComment		= QT_TRANSLATE_NOOP("InventoryModel", "Comment");
SCC TidInventoryFieldLastModified	= QT_TRANSLATE_NOOP("InventoryModel", "Last modification date");
SCC TidInventoryFieldDeleted		= QT_TRANSLATE_NOOP("InventoryModel", "Is deleted");

SCC TidPartnerFieldName			= QT_TRANSLATE_NOOP("PartnerModel", "Name");
SCC TidPartnerFieldCountry		= QT_TRANSLATE_NOOP("PartnerModel", "Country");
SCC TidPartnerFieldCity			= QT_TRANSLATE_NOOP("PartnerModel", "City");
SCC TidPartnerFieldPostalCode	= QT_TRANSLATE_NOOP("PartnerModel", "Postal code");
SCC TidPartnerFieldAddress		= QT_TRANSLATE_NOOP("PartnerModel", "Address");
SCC TidPartnerFieldCompany		= QT_TRANSLATE_NOOP("PartnerModel", "Company");
SCC TidPartnerFieldStoreName	= QT_TRANSLATE_NOOP("PartnerModel", "Store/Agent");
SCC TidPartnerFieldLastModified	= QT_TRANSLATE_NOOP("PartnerModel", "Last modification date");
SCC TidPartnerFieldDeleted		= QT_TRANSLATE_NOOP("PartnerModel", "Is deleted");

SCC TidAccountFieldName			= QT_TRANSLATE_NOOP("AccountModel", "Name");
SCC TidAccountFieldCurrency		= QT_TRANSLATE_NOOP("AccountModel", "Currency");
SCC TidAccountFieldBankOffice	= QT_TRANSLATE_NOOP("AccountModel", "Bank Office");
SCC TidAccountFieldIban			= QT_TRANSLATE_NOOP("AccountModel", "IBAN");
SCC TidAccountFieldSwiftCode	= QT_TRANSLATE_NOOP("AccountModel", "Swift code");
SCC TidAccountFieldLastModified	= QT_TRANSLATE_NOOP("AccountModel", "Last modification date");
SCC TidAccountFieldDeleted		= QT_TRANSLATE_NOOP("AccountModel", "Is deleted");

SCC TidPaymentFieldUploadDate	= QT_TRANSLATE_NOOP("PaymentModel", "Upload date");
SCC TidPaymentFieldAccount		= QT_TRANSLATE_NOOP("PaymentModel", "Account");
SCC TidPaymentFieldPartner		= QT_TRANSLATE_NOOP("PaymentModel", "Partner");
SCC TidPaymentFieldAmount		= QT_TRANSLATE_NOOP("PaymentModel", "Amount");
SCC TidPaymentFieldSubject		= QT_TRANSLATE_NOOP("PaymentModel", "Subject");
SCC TidPaymentFieldPayDate		= QT_TRANSLATE_NOOP("PaymentModel", "Payment date");
SCC TidPaymentFieldLastModified	= QT_TRANSLATE_NOOP("PaymentModel", "Last modification date");
SCC TidPaymentFieldDeleted		= QT_TRANSLATE_NOOP("PaymentModel", "Is deleted");

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

SCC TidItemQueryWithTagFieldQuery		= QT_TRANSLATE_NOOP("ItemQueryWithTagModel", "Query");
SCC TidItemQueryWithTagFieldTag			= QT_TRANSLATE_NOOP("ItemQueryWithTagModel", "Tag");
SCC TidItemQueryWithTagFieldLastModified= QT_TRANSLATE_NOOP("ItemQueryWithTagModel", "Last modification date");
SCC TidItemQueryWithTagFieldDeleted		= QT_TRANSLATE_NOOP("ItemQueryWithTagModel", "Is deleted");

SCC TidItemQueryWithoutTagFieldQuery		= QT_TRANSLATE_NOOP("ItemQueryWithoutTagModel", "Query");
SCC TidItemQueryWithoutTagFieldTag			= QT_TRANSLATE_NOOP("ItemQueryWithoutTagModel", "Tag");
SCC TidItemQueryWithoutTagFieldLastModified	= QT_TRANSLATE_NOOP("ItemQueryWithoutTagModel", "Last modification date");
SCC TidItemQueryWithoutTagFieldDeleted		= QT_TRANSLATE_NOOP("ItemQueryWithoutTagModel", "Is deleted");

SCC TidItemQueryWareFieldQuery			= QT_TRANSLATE_NOOP("ItemQueryWareModel", "Query");
SCC TidItemQueryWareFieldWare			= QT_TRANSLATE_NOOP("ItemQueryWareModel", "Ware");
SCC TidItemQueryWareFieldLastModified	= QT_TRANSLATE_NOOP("ItemQueryWareModel", "Last modification date");
SCC TidItemQueryWareFieldDeleted		= QT_TRANSLATE_NOOP("ItemQueryWareModel", "Is deleted");

SCC TidItemQueryPartnerFieldQuery		= QT_TRANSLATE_NOOP("ItemQueryPartnerModel", "Query");
SCC TidItemQueryPartnerFieldPartner		= QT_TRANSLATE_NOOP("ItemQueryPartnerModel", "Partner");
SCC TidItemQueryPartnerFieldLastModified= QT_TRANSLATE_NOOP("ItemQueryPartnerModel", "Last modification date");
SCC TidItemQueryPartnerFieldDeleted		= QT_TRANSLATE_NOOP("ItemQueryPartnerModel", "Is deleted");

SCC TidItemQueryFieldName			= QT_TRANSLATE_NOOP("ItemQueryModel", "Name");
SCC TidItemQueryFieldStartDate		= QT_TRANSLATE_NOOP("ItemQueryModel", "From date");
SCC TidItemQueryFieldEndDate		= QT_TRANSLATE_NOOP("ItemQueryModel", "Till date");
SCC TidItemQueryFieldStockOption	= QT_TRANSLATE_NOOP("ItemQueryModel", "Stock option");
SCC TidItemQueryFieldTagOption		= QT_TRANSLATE_NOOP("ItemQueryModel", "Tag option");
SCC TidItemQueryFieldWithTags		= QT_TRANSLATE_NOOP("ItemQueryModel", "With tags");
SCC TidItemQueryFieldWithoutTags	= QT_TRANSLATE_NOOP("ItemQueryModel", "Without tags");
SCC TidItemQueryFieldWares			= QT_TRANSLATE_NOOP("ItemQueryModel", "Wares");
SCC TidItemQueryFieldPartners		= QT_TRANSLATE_NOOP("ItemQueryModel", "Partners");
SCC TidItemQueryFieldLastModified	= QT_TRANSLATE_NOOP("ItemQueryModel", "Last modification date");
SCC TidItemQueryFieldDeleted		= QT_TRANSLATE_NOOP("ItemQueryModel", "Is deleted");

SCC TidPaymentQueryFieldName			= QT_TRANSLATE_NOOP("PaymentQueryModel", "Name");
SCC TidPaymentQueryFieldStartDate		= QT_TRANSLATE_NOOP("PaymentQueryModel", "From date");
SCC TidPaymentQueryFieldEndDate		= QT_TRANSLATE_NOOP("PaymentQueryModel", "Till date");
SCC TidPaymentQueryFieldPartners		= QT_TRANSLATE_NOOP("PaymentQueryModel", "Partners");
SCC TidPaymentQueryFieldLastModified	= QT_TRANSLATE_NOOP("PaymentQueryModel", "Last modification date");
SCC TidPaymentQueryFieldDeleted		= QT_TRANSLATE_NOOP("PaymentQueryModel", "Is deleted");

SCC TidPaymentQueryPartnerFieldQuery		= QT_TRANSLATE_NOOP("PaymentQueryPartnerModel", "Query");
SCC TidPaymentQueryPartnerFieldPartner		= QT_TRANSLATE_NOOP("PaymentQueryPartnerModel", "Partner");
SCC TidPaymentQueryPartnerFieldLastModified= QT_TRANSLATE_NOOP("PaymentQueryPartnerModel", "Last modification date");
SCC TidPaymentQueryPartnerFieldDeleted		= QT_TRANSLATE_NOOP("PaymentQueryPartnerModel", "Is deleted");







@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp
@}ForTypes@

@ForTypes{Item,Payment@
@include@ dbdatamodel.cpp
@}ForTypes@

@ForTypes{DatabaseDescriptor,WareType,WareTag,ItemQueryWithTag,ItemQueryWithoutTag,ItemQueryWare
			ItemQueryPartner,PaymentQueryPartner@
@include@ datamodel.cpp
@}ForTypes@


ItemModel::ItemModel(SqlConnection & sql, const WareModel & wmodel) :
	sql(sql),
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
	dataSet = @Type@Set::fromDb(sql, opts, stat);
}

bool ItemModel::queryFilter(const Item & modified)
{
	(void)(modified);
	return true;
}

