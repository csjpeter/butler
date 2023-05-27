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
SCC TidPaymentFieldComment		= QT_TRANSLATE_NOOP("PaymentModel", "Comment");
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








TagModel::TagModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = TagSet::fromDb(sql);
}

void TagModel::query()
{
	ModelResetGuard g(this);
	dataSet = TagSet::fromDb(sql);
}

bool TagModel::queryFilter(const Tag & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<TagModel> TagModel::operationListeners;

Qt::ItemFlags TagModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Tag::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Tag::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant TagModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Tag::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Tag::Description :
				v <<= dataSet.queryAt(index.row()).description;
				break;
		case Tag::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Tag::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant TagModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Tag::Name :
			return QVariant(tr(TidTagFieldName));
			break;
		case Tag::Description :
			return QVariant(tr(TidTagFieldDescription));
			break;
		case Tag::LastModified :
			return QVariant(tr(TidTagFieldLastModified));
			break;
		case Tag::Deleted :
			return QVariant(tr(TidTagFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool TagModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Tag modified(dataSet.queryAt(row));

	switch(index.column()){
		case Tag::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Tag::Description :
			modified.description <<= value;
			update(row, modified);
			break;
				case Tag::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Tag::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool TagModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int TagModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int TagModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Tag::NumOfAllFields;
}

bool TagModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool TagModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void TagModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Tag::Fields>(column));
	dataSet.sort();
}

int TagModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Tag& TagModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void TagModel::addNew(Tag & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void TagModel::objInserted(SqlConnection & sql, const Tag & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void TagModel::objInsertListener(SqlConnection & sql, const Tag & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Tag(obj));
	}
}

void TagModel::update(int row, Tag & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void TagModel::objChange(SqlConnection & sql, const Tag & orig, const Tag & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void TagModel::objChangeListener(SqlConnection & sql, const Tag & orig, const Tag & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Tag(modified));
	}
}

void TagModel::del(int row)
{
	Tag modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void TagModel::objRemoved(SqlConnection & sql, const Tag & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void TagModel::objRemovedListener(SqlConnection & sql, const Tag & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



WareModel::WareModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = WareSet::fromDb(sql);
}

void WareModel::query()
{
	ModelResetGuard g(this);
	dataSet = WareSet::fromDb(sql);
}

bool WareModel::queryFilter(const Ware & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<WareModel> WareModel::operationListeners;

Qt::ItemFlags WareModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Ware::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Ware::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant WareModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Ware::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Ware::Unit :
				v <<= dataSet.queryAt(index.row()).unit;
				break;
		case Ware::Icon :
				v <<= dataSet.queryAt(index.row()).icon;
				break;
		/*case Ware::Types :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).types;
				return s;
			}
			break;*/
		/*case Ware::Tags :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).tags;
				return s;
			}
			break;*/
		case Ware::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Ware::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant WareModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Ware::Name :
			return QVariant(tr(TidWareFieldName));
			break;
		case Ware::Unit :
			return QVariant(tr(TidWareFieldUnit));
			break;
		case Ware::Icon :
			return QVariant(tr(TidWareFieldIcon));
			break;
		case Ware::Types :
			return QVariant(tr(TidWareFieldTypes));
			break;
		case Ware::Tags :
			return QVariant(tr(TidWareFieldTags));
			break;
		case Ware::LastModified :
			return QVariant(tr(TidWareFieldLastModified));
			break;
		case Ware::Deleted :
			return QVariant(tr(TidWareFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool WareModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Ware modified(dataSet.queryAt(row));

	switch(index.column()){
		case Ware::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Ware::Unit :
			modified.unit <<= value;
			update(row, modified);
			break;
				case Ware::Icon :
			modified.icon <<= value;
			update(row, modified);
			break;
				/*case Ware::Types :
			modified.setAsTypes(value.toString());
			update(row, modified);
			break;*/
				/*case Ware::Tags :
			modified.setAsTags(value.toString());
			update(row, modified);
			break;*/
				case Ware::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Ware::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool WareModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WareModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int WareModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Ware::NumOfAllFields;
}

bool WareModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WareModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WareModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Ware::Fields>(column));
	dataSet.sort();
}

int WareModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Ware& WareModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void WareModel::addNew(Ware & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void WareModel::objInserted(SqlConnection & sql, const Ware & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void WareModel::objInsertListener(SqlConnection & sql, const Ware & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Ware(obj));
	}
}

void WareModel::update(int row, Ware & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void WareModel::objChange(SqlConnection & sql, const Ware & orig, const Ware & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void WareModel::objChangeListener(SqlConnection & sql, const Ware & orig, const Ware & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Ware(modified));
	}
}

void WareModel::del(int row)
{
	Ware modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void WareModel::objRemoved(SqlConnection & sql, const Ware & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void WareModel::objRemovedListener(SqlConnection & sql, const Ware & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



CompanyModel::CompanyModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = CompanySet::fromDb(sql);
}

void CompanyModel::query()
{
	ModelResetGuard g(this);
	dataSet = CompanySet::fromDb(sql);
}

bool CompanyModel::queryFilter(const Company & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<CompanyModel> CompanyModel::operationListeners;

Qt::ItemFlags CompanyModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Company::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Company::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant CompanyModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Company::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Company::Country :
				v <<= dataSet.queryAt(index.row()).country;
				break;
		case Company::City :
				v <<= dataSet.queryAt(index.row()).city;
				break;
		case Company::PostalCode :
				v <<= dataSet.queryAt(index.row()).postalCode;
				break;
		case Company::Address :
				v <<= dataSet.queryAt(index.row()).address;
				break;
		case Company::TaxId :
				v <<= dataSet.queryAt(index.row()).taxId;
				break;
		case Company::Icon :
				v <<= dataSet.queryAt(index.row()).icon;
				break;
		case Company::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Company::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant CompanyModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Company::Name :
			return QVariant(tr(TidCompanyFieldName));
			break;
		case Company::Country :
			return QVariant(tr(TidCompanyFieldCountry));
			break;
		case Company::City :
			return QVariant(tr(TidCompanyFieldCity));
			break;
		case Company::PostalCode :
			return QVariant(tr(TidCompanyFieldPostalCode));
			break;
		case Company::Address :
			return QVariant(tr(TidCompanyFieldAddress));
			break;
		case Company::TaxId :
			return QVariant(tr(TidCompanyFieldTaxId));
			break;
		case Company::Icon :
			return QVariant(tr(TidCompanyFieldIcon));
			break;
		case Company::LastModified :
			return QVariant(tr(TidCompanyFieldLastModified));
			break;
		case Company::Deleted :
			return QVariant(tr(TidCompanyFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool CompanyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Company modified(dataSet.queryAt(row));

	switch(index.column()){
		case Company::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Company::Country :
			modified.country <<= value;
			update(row, modified);
			break;
				case Company::City :
			modified.city <<= value;
			update(row, modified);
			break;
				case Company::PostalCode :
			modified.postalCode <<= value;
			update(row, modified);
			break;
				case Company::Address :
			modified.address <<= value;
			update(row, modified);
			break;
				case Company::TaxId :
			modified.taxId <<= value;
			update(row, modified);
			break;
				case Company::Icon :
			modified.icon <<= value;
			update(row, modified);
			break;
				case Company::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Company::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool CompanyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int CompanyModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int CompanyModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Company::NumOfAllFields;
}

bool CompanyModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool CompanyModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void CompanyModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Company::Fields>(column));
	dataSet.sort();
}

int CompanyModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Company& CompanyModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void CompanyModel::addNew(Company & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void CompanyModel::objInserted(SqlConnection & sql, const Company & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void CompanyModel::objInsertListener(SqlConnection & sql, const Company & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Company(obj));
	}
}

void CompanyModel::update(int row, Company & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void CompanyModel::objChange(SqlConnection & sql, const Company & orig, const Company & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void CompanyModel::objChangeListener(SqlConnection & sql, const Company & orig, const Company & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Company(modified));
	}
}

void CompanyModel::del(int row)
{
	Company modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void CompanyModel::objRemoved(SqlConnection & sql, const Company & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void CompanyModel::objRemovedListener(SqlConnection & sql, const Company & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



BrandModel::BrandModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = BrandSet::fromDb(sql);
}

void BrandModel::query()
{
	ModelResetGuard g(this);
	dataSet = BrandSet::fromDb(sql);
}

bool BrandModel::queryFilter(const Brand & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<BrandModel> BrandModel::operationListeners;

Qt::ItemFlags BrandModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Brand::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Brand::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant BrandModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Brand::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Brand::Company :
				v <<= dataSet.queryAt(index.row()).company;
				break;
		case Brand::Icon :
				v <<= dataSet.queryAt(index.row()).icon;
				break;
		case Brand::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Brand::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant BrandModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Brand::Name :
			return QVariant(tr(TidBrandFieldName));
			break;
		case Brand::Company :
			return QVariant(tr(TidBrandFieldCompany));
			break;
		case Brand::Icon :
			return QVariant(tr(TidBrandFieldIcon));
			break;
		case Brand::LastModified :
			return QVariant(tr(TidBrandFieldLastModified));
			break;
		case Brand::Deleted :
			return QVariant(tr(TidBrandFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool BrandModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Brand modified(dataSet.queryAt(row));

	switch(index.column()){
		case Brand::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Brand::Company :
			modified.company <<= value;
			update(row, modified);
			break;
				case Brand::Icon :
			modified.icon <<= value;
			update(row, modified);
			break;
				case Brand::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Brand::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool BrandModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int BrandModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int BrandModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Brand::NumOfAllFields;
}

bool BrandModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool BrandModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void BrandModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Brand::Fields>(column));
	dataSet.sort();
}

int BrandModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Brand& BrandModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void BrandModel::addNew(Brand & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void BrandModel::objInserted(SqlConnection & sql, const Brand & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void BrandModel::objInsertListener(SqlConnection & sql, const Brand & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Brand(obj));
	}
}

void BrandModel::update(int row, Brand & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void BrandModel::objChange(SqlConnection & sql, const Brand & orig, const Brand & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void BrandModel::objChangeListener(SqlConnection & sql, const Brand & orig, const Brand & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Brand(modified));
	}
}

void BrandModel::del(int row)
{
	Brand modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void BrandModel::objRemoved(SqlConnection & sql, const Brand & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void BrandModel::objRemovedListener(SqlConnection & sql, const Brand & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



InventoryModel::InventoryModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = InventorySet::fromDb(sql);
}

void InventoryModel::query()
{
	ModelResetGuard g(this);
	dataSet = InventorySet::fromDb(sql);
}

bool InventoryModel::queryFilter(const Inventory & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<InventoryModel> InventoryModel::operationListeners;

Qt::ItemFlags InventoryModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Inventory::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Inventory::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant InventoryModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Inventory::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Inventory::Comment :
				v <<= dataSet.queryAt(index.row()).comment;
				break;
		case Inventory::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Inventory::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant InventoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Inventory::Name :
			return QVariant(tr(TidInventoryFieldName));
			break;
		case Inventory::Comment :
			return QVariant(tr(TidInventoryFieldComment));
			break;
		case Inventory::LastModified :
			return QVariant(tr(TidInventoryFieldLastModified));
			break;
		case Inventory::Deleted :
			return QVariant(tr(TidInventoryFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool InventoryModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Inventory modified(dataSet.queryAt(row));

	switch(index.column()){
		case Inventory::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Inventory::Comment :
			modified.comment <<= value;
			update(row, modified);
			break;
				case Inventory::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Inventory::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool InventoryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int InventoryModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int InventoryModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Inventory::NumOfAllFields;
}

bool InventoryModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool InventoryModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void InventoryModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Inventory::Fields>(column));
	dataSet.sort();
}

int InventoryModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Inventory& InventoryModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void InventoryModel::addNew(Inventory & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void InventoryModel::objInserted(SqlConnection & sql, const Inventory & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void InventoryModel::objInsertListener(SqlConnection & sql, const Inventory & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Inventory(obj));
	}
}

void InventoryModel::update(int row, Inventory & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void InventoryModel::objChange(SqlConnection & sql, const Inventory & orig, const Inventory & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void InventoryModel::objChangeListener(SqlConnection & sql, const Inventory & orig, const Inventory & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Inventory(modified));
	}
}

void InventoryModel::del(int row)
{
	Inventory modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void InventoryModel::objRemoved(SqlConnection & sql, const Inventory & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void InventoryModel::objRemovedListener(SqlConnection & sql, const Inventory & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



PartnerModel::PartnerModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = PartnerSet::fromDb(sql);
}

void PartnerModel::query()
{
	ModelResetGuard g(this);
	dataSet = PartnerSet::fromDb(sql);
}

bool PartnerModel::queryFilter(const Partner & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<PartnerModel> PartnerModel::operationListeners;

Qt::ItemFlags PartnerModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Partner::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Partner::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant PartnerModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Partner::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Partner::Country :
				v <<= dataSet.queryAt(index.row()).country;
				break;
		case Partner::City :
				v <<= dataSet.queryAt(index.row()).city;
				break;
		case Partner::PostalCode :
				v <<= dataSet.queryAt(index.row()).postalCode;
				break;
		case Partner::Address :
				v <<= dataSet.queryAt(index.row()).address;
				break;
		case Partner::Company :
				v <<= dataSet.queryAt(index.row()).company;
				break;
		case Partner::StoreName :
				v <<= dataSet.queryAt(index.row()).storeName;
				break;
		case Partner::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Partner::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant PartnerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Partner::Name :
			return QVariant(tr(TidPartnerFieldName));
			break;
		case Partner::Country :
			return QVariant(tr(TidPartnerFieldCountry));
			break;
		case Partner::City :
			return QVariant(tr(TidPartnerFieldCity));
			break;
		case Partner::PostalCode :
			return QVariant(tr(TidPartnerFieldPostalCode));
			break;
		case Partner::Address :
			return QVariant(tr(TidPartnerFieldAddress));
			break;
		case Partner::Company :
			return QVariant(tr(TidPartnerFieldCompany));
			break;
		case Partner::StoreName :
			return QVariant(tr(TidPartnerFieldStoreName));
			break;
		case Partner::LastModified :
			return QVariant(tr(TidPartnerFieldLastModified));
			break;
		case Partner::Deleted :
			return QVariant(tr(TidPartnerFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool PartnerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Partner modified(dataSet.queryAt(row));

	switch(index.column()){
		case Partner::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Partner::Country :
			modified.country <<= value;
			update(row, modified);
			break;
				case Partner::City :
			modified.city <<= value;
			update(row, modified);
			break;
				case Partner::PostalCode :
			modified.postalCode <<= value;
			update(row, modified);
			break;
				case Partner::Address :
			modified.address <<= value;
			update(row, modified);
			break;
				case Partner::Company :
			modified.company <<= value;
			update(row, modified);
			break;
				case Partner::StoreName :
			modified.storeName <<= value;
			update(row, modified);
			break;
				case Partner::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Partner::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool PartnerModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int PartnerModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int PartnerModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Partner::NumOfAllFields;
}

bool PartnerModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool PartnerModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void PartnerModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Partner::Fields>(column));
	dataSet.sort();
}

int PartnerModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Partner& PartnerModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void PartnerModel::addNew(Partner & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void PartnerModel::objInserted(SqlConnection & sql, const Partner & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void PartnerModel::objInsertListener(SqlConnection & sql, const Partner & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Partner(obj));
	}
}

void PartnerModel::update(int row, Partner & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void PartnerModel::objChange(SqlConnection & sql, const Partner & orig, const Partner & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void PartnerModel::objChangeListener(SqlConnection & sql, const Partner & orig, const Partner & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Partner(modified));
	}
}

void PartnerModel::del(int row)
{
	Partner modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void PartnerModel::objRemoved(SqlConnection & sql, const Partner & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void PartnerModel::objRemovedListener(SqlConnection & sql, const Partner & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



AccountModel::AccountModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = AccountSet::fromDb(sql);
}

void AccountModel::query()
{
	ModelResetGuard g(this);
	dataSet = AccountSet::fromDb(sql);
}

bool AccountModel::queryFilter(const Account & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<AccountModel> AccountModel::operationListeners;

Qt::ItemFlags AccountModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Account::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Account::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant AccountModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Account::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Account::Currency :
				v <<= dataSet.queryAt(index.row()).currency;
				break;
		case Account::BankOffice :
				v <<= dataSet.queryAt(index.row()).bankOffice;
				break;
		case Account::Iban :
				v <<= dataSet.queryAt(index.row()).iban;
				break;
		case Account::SwiftCode :
				v <<= dataSet.queryAt(index.row()).swiftCode;
				break;
		case Account::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Account::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant AccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Account::Name :
			return QVariant(tr(TidAccountFieldName));
			break;
		case Account::Currency :
			return QVariant(tr(TidAccountFieldCurrency));
			break;
		case Account::BankOffice :
			return QVariant(tr(TidAccountFieldBankOffice));
			break;
		case Account::Iban :
			return QVariant(tr(TidAccountFieldIban));
			break;
		case Account::SwiftCode :
			return QVariant(tr(TidAccountFieldSwiftCode));
			break;
		case Account::LastModified :
			return QVariant(tr(TidAccountFieldLastModified));
			break;
		case Account::Deleted :
			return QVariant(tr(TidAccountFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool AccountModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Account modified(dataSet.queryAt(row));

	switch(index.column()){
		case Account::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Account::Currency :
			modified.currency <<= value;
			update(row, modified);
			break;
				case Account::BankOffice :
			modified.bankOffice <<= value;
			update(row, modified);
			break;
				case Account::Iban :
			modified.iban <<= value;
			update(row, modified);
			break;
				case Account::SwiftCode :
			modified.swiftCode <<= value;
			update(row, modified);
			break;
				case Account::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Account::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool AccountModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int AccountModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int AccountModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Account::NumOfAllFields;
}

bool AccountModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool AccountModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void AccountModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Account::Fields>(column));
	dataSet.sort();
}

int AccountModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const Account& AccountModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void AccountModel::addNew(Account & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void AccountModel::objInserted(SqlConnection & sql, const Account & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void AccountModel::objInsertListener(SqlConnection & sql, const Account & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Account(obj));
	}
}

void AccountModel::update(int row, Account & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void AccountModel::objChange(SqlConnection & sql, const Account & orig, const Account & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void AccountModel::objChangeListener(SqlConnection & sql, const Account & orig, const Account & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Account(modified));
	}
}

void AccountModel::del(int row)
{
	Account modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void AccountModel::objRemoved(SqlConnection & sql, const Account & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void AccountModel::objRemovedListener(SqlConnection & sql, const Account & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



ItemQueryModel::ItemQueryModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = ItemQuerySet::fromDb(sql);
}

void ItemQueryModel::query()
{
	ModelResetGuard g(this);
	dataSet = ItemQuerySet::fromDb(sql);
}

bool ItemQueryModel::queryFilter(const ItemQuery & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<ItemQueryModel> ItemQueryModel::operationListeners;

Qt::ItemFlags ItemQueryModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < ItemQuery::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < ItemQuery::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemQueryModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case ItemQuery::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case ItemQuery::StartDate :
				v <<= dataSet.queryAt(index.row()).startDate;
				break;
		case ItemQuery::EndDate :
				v <<= dataSet.queryAt(index.row()).endDate;
				break;
		/*case ItemQuery::StockOption :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).stockOption;
				return s;
			}
			break;*/
		/*case ItemQuery::TagOption :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).tagOption;
				return s;
			}
			break;*/
		/*case ItemQuery::WithTags :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).withTags;
				return s;
			}
			break;*/
		/*case ItemQuery::WithoutTags :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).withoutTags;
				return s;
			}
			break;*/
		/*case ItemQuery::Wares :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).wares;
				return s;
			}
			break;*/
		/*case ItemQuery::Partners :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).partners;
				return s;
			}
			break;*/
		case ItemQuery::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case ItemQuery::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant ItemQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case ItemQuery::Name :
			return QVariant(tr(TidItemQueryFieldName));
			break;
		case ItemQuery::StartDate :
			return QVariant(tr(TidItemQueryFieldStartDate));
			break;
		case ItemQuery::EndDate :
			return QVariant(tr(TidItemQueryFieldEndDate));
			break;
		case ItemQuery::StockOption :
			return QVariant(tr(TidItemQueryFieldStockOption));
			break;
		case ItemQuery::TagOption :
			return QVariant(tr(TidItemQueryFieldTagOption));
			break;
		case ItemQuery::WithTags :
			return QVariant(tr(TidItemQueryFieldWithTags));
			break;
		case ItemQuery::WithoutTags :
			return QVariant(tr(TidItemQueryFieldWithoutTags));
			break;
		case ItemQuery::Wares :
			return QVariant(tr(TidItemQueryFieldWares));
			break;
		case ItemQuery::Partners :
			return QVariant(tr(TidItemQueryFieldPartners));
			break;
		case ItemQuery::LastModified :
			return QVariant(tr(TidItemQueryFieldLastModified));
			break;
		case ItemQuery::Deleted :
			return QVariant(tr(TidItemQueryFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemQueryModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	ItemQuery modified(dataSet.queryAt(row));

	switch(index.column()){
		case ItemQuery::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case ItemQuery::StartDate :
			modified.startDate <<= value;
			update(row, modified);
			break;
				case ItemQuery::EndDate :
			modified.endDate <<= value;
			update(row, modified);
			break;
				/*case ItemQuery::StockOption :
			modified.setAsStockOption(value.toString());
			update(row, modified);
			break;*/
				/*case ItemQuery::TagOption :
			modified.setAsTagOption(value.toString());
			update(row, modified);
			break;*/
				/*case ItemQuery::WithTags :
			modified.setAsWithTags(value.toString());
			update(row, modified);
			break;*/
				/*case ItemQuery::WithoutTags :
			modified.setAsWithoutTags(value.toString());
			update(row, modified);
			break;*/
				/*case ItemQuery::Wares :
			modified.setAsWares(value.toString());
			update(row, modified);
			break;*/
				/*case ItemQuery::Partners :
			modified.setAsPartners(value.toString());
			update(row, modified);
			break;*/
				case ItemQuery::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case ItemQuery::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemQueryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemQueryModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemQueryModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return ItemQuery::NumOfAllFields;
}

bool ItemQueryModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemQueryModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemQueryModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<ItemQuery::Fields>(column));
	dataSet.sort();
}

int ItemQueryModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const ItemQuery& ItemQueryModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemQueryModel::addNew(ItemQuery & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void ItemQueryModel::objInserted(SqlConnection & sql, const ItemQuery & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void ItemQueryModel::objInsertListener(SqlConnection & sql, const ItemQuery & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQuery(obj));
	}
}

void ItemQueryModel::update(int row, ItemQuery & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void ItemQueryModel::objChange(SqlConnection & sql, const ItemQuery & orig, const ItemQuery & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void ItemQueryModel::objChangeListener(SqlConnection & sql, const ItemQuery & orig, const ItemQuery & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQuery(modified));
	}
}

void ItemQueryModel::del(int row)
{
	ItemQuery modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void ItemQueryModel::objRemoved(SqlConnection & sql, const ItemQuery & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void ItemQueryModel::objRemovedListener(SqlConnection & sql, const ItemQuery & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



PaymentQueryModel::PaymentQueryModel(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = PaymentQuerySet::fromDb(sql);
}

void PaymentQueryModel::query()
{
	ModelResetGuard g(this);
	dataSet = PaymentQuerySet::fromDb(sql);
}

bool PaymentQueryModel::queryFilter(const PaymentQuery & modified)
{
	(void)(modified);
	return true;
}


csjp::RefArray<PaymentQueryModel> PaymentQueryModel::operationListeners;

Qt::ItemFlags PaymentQueryModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < PaymentQuery::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < PaymentQuery::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant PaymentQueryModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case PaymentQuery::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case PaymentQuery::StartDate :
				v <<= dataSet.queryAt(index.row()).startDate;
				break;
		case PaymentQuery::EndDate :
				v <<= dataSet.queryAt(index.row()).endDate;
				break;
		/*case PaymentQuery::Partners :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).partners;
				return s;
			}
			break;*/
		case PaymentQuery::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case PaymentQuery::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant PaymentQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case PaymentQuery::Name :
			return QVariant(tr(TidPaymentQueryFieldName));
			break;
		case PaymentQuery::StartDate :
			return QVariant(tr(TidPaymentQueryFieldStartDate));
			break;
		case PaymentQuery::EndDate :
			return QVariant(tr(TidPaymentQueryFieldEndDate));
			break;
		case PaymentQuery::Partners :
			return QVariant(tr(TidPaymentQueryFieldPartners));
			break;
		case PaymentQuery::LastModified :
			return QVariant(tr(TidPaymentQueryFieldLastModified));
			break;
		case PaymentQuery::Deleted :
			return QVariant(tr(TidPaymentQueryFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool PaymentQueryModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	PaymentQuery modified(dataSet.queryAt(row));

	switch(index.column()){
		case PaymentQuery::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case PaymentQuery::StartDate :
			modified.startDate <<= value;
			update(row, modified);
			break;
				case PaymentQuery::EndDate :
			modified.endDate <<= value;
			update(row, modified);
			break;
				/*case PaymentQuery::Partners :
			modified.setAsPartners(value.toString());
			update(row, modified);
			break;*/
				case PaymentQuery::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case PaymentQuery::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool PaymentQueryModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int PaymentQueryModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int PaymentQueryModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return PaymentQuery::NumOfAllFields;
}

bool PaymentQueryModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool PaymentQueryModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void PaymentQueryModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<PaymentQuery::Fields>(column));
	dataSet.sort();
}

int PaymentQueryModel::index(const Text & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}

const PaymentQuery& PaymentQueryModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void PaymentQueryModel::addNew(PaymentQuery & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void PaymentQueryModel::objInserted(SqlConnection & sql, const PaymentQuery & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void PaymentQueryModel::objInsertListener(SqlConnection & sql, const PaymentQuery & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.name)){
		int row = dataSet.index(obj.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new PaymentQuery(obj));
	}
}

void PaymentQueryModel::update(int row, PaymentQuery & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void PaymentQueryModel::objChange(SqlConnection & sql, const PaymentQuery & orig, const PaymentQuery & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void PaymentQueryModel::objChangeListener(SqlConnection & sql, const PaymentQuery & orig, const PaymentQuery & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.name)){
		int row = dataSet.index(orig.name);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new PaymentQuery(modified));
	}
}

void PaymentQueryModel::del(int row)
{
	PaymentQuery modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void PaymentQueryModel::objRemoved(SqlConnection & sql, const PaymentQuery & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void PaymentQueryModel::objRemovedListener(SqlConnection & sql, const PaymentQuery & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.name))
		return;

	int row = dataSet.index(r.name);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}





csjp::RefArray<ItemModel> ItemModel::operationListeners;

Qt::ItemFlags ItemModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Item::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Item::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Item::UploadDate :
				v <<= dataSet.queryAt(index.row()).uploadDate;
				break;
		case Item::Name :
				v <<= dataSet.queryAt(index.row()).name;
				break;
		case Item::Unit :
				v <<= dataSet.queryAt(index.row()).unit;
				break;
		case Item::Type :
				v <<= dataSet.queryAt(index.row()).type;
				break;
		case Item::Brand :
				v <<= dataSet.queryAt(index.row()).brand;
				break;
		case Item::Quantity :
				v <<= dataSet.queryAt(index.row()).quantity;
				break;
		case Item::Price :
				v <<= dataSet.queryAt(index.row()).price;
				break;
		case Item::Currency :
				v <<= dataSet.queryAt(index.row()).currency;
				break;
		case Item::Account :
				v <<= dataSet.queryAt(index.row()).account;
				break;
		case Item::Partner :
				v <<= dataSet.queryAt(index.row()).partner;
				break;
		case Item::Inventory :
				v <<= dataSet.queryAt(index.row()).inventory;
				break;
		case Item::Comment :
				v <<= dataSet.queryAt(index.row()).comment;
				break;
		case Item::InvChangeDate :
				v <<= dataSet.queryAt(index.row()).invChangeDate;
				break;
		case Item::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Item::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
		case Item::UnitPrice :
			return v;
			break;
		case Item::QuantityWithUnit :
			return v;
			break;
		default :
			return v;
	}

	return v;
}

QVariant ItemModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Item::UploadDate :
			return QVariant(tr(TidItemFieldUploadDate));
			break;
		case Item::Name :
			return QVariant(tr(TidItemFieldName));
			break;
		case Item::Unit :
			return QVariant(tr(TidItemFieldUnit));
			break;
		case Item::Type :
			return QVariant(tr(TidItemFieldType));
			break;
		case Item::Brand :
			return QVariant(tr(TidItemFieldBrand));
			break;
		case Item::Quantity :
			return QVariant(tr(TidItemFieldQuantity));
			break;
		case Item::Price :
			return QVariant(tr(TidItemFieldPrice));
			break;
		case Item::Currency :
			return QVariant(tr(TidItemFieldCurrency));
			break;
		case Item::Account :
			return QVariant(tr(TidItemFieldAccount));
			break;
		case Item::Partner :
			return QVariant(tr(TidItemFieldPartner));
			break;
		case Item::Inventory :
			return QVariant(tr(TidItemFieldInventory));
			break;
		case Item::Comment :
			return QVariant(tr(TidItemFieldComment));
			break;
		case Item::InvChangeDate :
			return QVariant(tr(TidItemFieldInvChangeDate));
			break;
		case Item::LastModified :
			return QVariant(tr(TidItemFieldLastModified));
			break;
		case Item::Deleted :
			return QVariant(tr(TidItemFieldDeleted));
			break;
		case Item::UnitPrice :
			return QVariant(tr(TidItemFieldUnitPrice));
			break;
		case Item::QuantityWithUnit :
			return QVariant(tr(TidItemFieldQuantityWithUnit));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Item modified(dataSet.queryAt(row));

	switch(index.column()){
		case Item::UploadDate :
			modified.uploadDate <<= value;
			update(row, modified);
			break;
				case Item::Name :
			modified.name <<= value;
			update(row, modified);
			break;
				case Item::Unit :
			modified.unit <<= value;
			update(row, modified);
			break;
				case Item::Type :
			modified.type <<= value;
			update(row, modified);
			break;
				case Item::Brand :
			modified.brand <<= value;
			update(row, modified);
			break;
				case Item::Quantity :
			modified.quantity <<= value;
			update(row, modified);
			break;
				case Item::Price :
			modified.price <<= value;
			update(row, modified);
			break;
				case Item::Currency :
			modified.currency <<= value;
			update(row, modified);
			break;
				case Item::Account :
			modified.account <<= value;
			update(row, modified);
			break;
				case Item::Partner :
			modified.partner <<= value;
			update(row, modified);
			break;
				case Item::Inventory :
			modified.inventory <<= value;
			update(row, modified);
			break;
				case Item::Comment :
			modified.comment <<= value;
			update(row, modified);
			break;
				case Item::InvChangeDate :
			modified.invChangeDate <<= value;
			update(row, modified);
			break;
				case Item::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Item::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		case Item::UnitPrice :
			break;
		case Item::QuantityWithUnit :
			break;
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Item::NumOfAllFields;
}

bool ItemModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Item::Fields>(column));
	dataSet.sort();
}

int ItemModel::index(const DateTime & uploadDate) const
{
	if(dataSet.has(uploadDate))
		return dataSet.index(uploadDate);
	else
		return -1;
}

const Item& ItemModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemModel::addNew(Item & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void ItemModel::objInserted(SqlConnection & sql, const Item & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void ItemModel::objInsertListener(SqlConnection & sql, const Item & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.uploadDate)){
		int row = dataSet.index(obj.uploadDate);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Item(obj));
	}
}

void ItemModel::update(int row, Item & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void ItemModel::objChange(SqlConnection & sql, const Item & orig, const Item & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void ItemModel::objChangeListener(SqlConnection & sql, const Item & orig, const Item & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.uploadDate)){
		int row = dataSet.index(orig.uploadDate);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Item(modified));
	}
}

void ItemModel::del(int row)
{
	Item modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void ItemModel::objRemoved(SqlConnection & sql, const Item & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void ItemModel::objRemovedListener(SqlConnection & sql, const Item & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.uploadDate))
		return;

	int row = dataSet.index(r.uploadDate);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}



csjp::RefArray<PaymentModel> PaymentModel::operationListeners;

Qt::ItemFlags PaymentModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < Payment::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < Payment::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant PaymentModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
	case Payment::UploadDate :
				v <<= dataSet.queryAt(index.row()).uploadDate;
				break;
		case Payment::Account :
				v <<= dataSet.queryAt(index.row()).account;
				break;
		case Payment::Partner :
				v <<= dataSet.queryAt(index.row()).partner;
				break;
		case Payment::Amount :
				v <<= dataSet.queryAt(index.row()).amount;
				break;
		case Payment::Subject :
				v <<= dataSet.queryAt(index.row()).subject;
				break;
		case Payment::PayDate :
				v <<= dataSet.queryAt(index.row()).payDate;
				break;
		case Payment::Comment :
				v <<= dataSet.queryAt(index.row()).comment;
				break;
		case Payment::LastModified :
				v <<= dataSet.queryAt(index.row()).lastModified;
				break;
		case Payment::Deleted :
				v <<= dataSet.queryAt(index.row()).deleted;
				break;
	
		default :
			return v;
	}

	return v;
}

QVariant PaymentModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Payment::UploadDate :
			return QVariant(tr(TidPaymentFieldUploadDate));
			break;
		case Payment::Account :
			return QVariant(tr(TidPaymentFieldAccount));
			break;
		case Payment::Partner :
			return QVariant(tr(TidPaymentFieldPartner));
			break;
		case Payment::Amount :
			return QVariant(tr(TidPaymentFieldAmount));
			break;
		case Payment::Subject :
			return QVariant(tr(TidPaymentFieldSubject));
			break;
		case Payment::PayDate :
			return QVariant(tr(TidPaymentFieldPayDate));
			break;
		case Payment::Comment :
			return QVariant(tr(TidPaymentFieldComment));
			break;
		case Payment::LastModified :
			return QVariant(tr(TidPaymentFieldLastModified));
			break;
		case Payment::Deleted :
			return QVariant(tr(TidPaymentFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool PaymentModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	Payment modified(dataSet.queryAt(row));

	switch(index.column()){
		case Payment::UploadDate :
			modified.uploadDate <<= value;
			update(row, modified);
			break;
				case Payment::Account :
			modified.account <<= value;
			update(row, modified);
			break;
				case Payment::Partner :
			modified.partner <<= value;
			update(row, modified);
			break;
				case Payment::Amount :
			modified.amount <<= value;
			update(row, modified);
			break;
				case Payment::Subject :
			modified.subject <<= value;
			update(row, modified);
			break;
				case Payment::PayDate :
			modified.payDate <<= value;
			update(row, modified);
			break;
				case Payment::Comment :
			modified.comment <<= value;
			update(row, modified);
			break;
				case Payment::LastModified :
			modified.lastModified <<= value;
			update(row, modified);
			break;
				case Payment::Deleted :
			modified.deleted <<= value;
			update(row, modified);
			break;
		
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool PaymentModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int PaymentModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int PaymentModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Payment::NumOfAllFields;
}

bool PaymentModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool PaymentModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void PaymentModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<Payment::Fields>(column));
	dataSet.sort();
}

int PaymentModel::index(const DateTime & uploadDate) const
{
	if(dataSet.has(uploadDate))
		return dataSet.index(uploadDate);
	else
		return -1;
}

const Payment& PaymentModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void PaymentModel::addNew(Payment & obj)
{
	obj.toDb(sql);
	objInserted(sql, obj);
}

void PaymentModel::objInserted(SqlConnection & sql, const Payment & obj)
{
	for(auto & i : operationListeners)
		i.objInsertListener(sql, obj);
}

void PaymentModel::objInsertListener(SqlConnection & sql, const Payment & obj)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(obj.uploadDate)){
		int row = dataSet.index(obj.uploadDate);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(obj)){
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Payment(obj));
	}
}

void PaymentModel::update(int row, Payment & modified)
{
	auto & orig = dataSet.queryAt(row);
	modified.toDb(sql, orig);
	objChange(sql, orig, modified);
}

void PaymentModel::objChange(SqlConnection & sql, const Payment & orig, const Payment & modified)
{
	for(auto & i : operationListeners)
		i.objChangeListener(sql, orig, modified);
}

void PaymentModel::objChangeListener(SqlConnection & sql, const Payment & orig, const Payment & modified)
{
	if(& sql != &(this->sql))
		return;

	if(dataSet.has(orig.uploadDate)){
		int row = dataSet.index(orig.uploadDate);
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	if(queryFilter(modified)) {
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new Payment(modified));
	}
}

void PaymentModel::del(int row)
{
	Payment modified(dataSet.queryAt(row));
	modified.deleted = true;
	modified.toDb(sql);

	auto & obj = dataSet.queryAt(row);
	objRemoved(sql, obj);
}

void PaymentModel::objRemoved(SqlConnection & sql, const Payment & r)
{
	for(auto & i : operationListeners)
		i.objRemovedListener(sql, r);
}

void PaymentModel::objRemovedListener(SqlConnection & sql, const Payment & r)
{
	if(& sql != &(this->sql))
		return;

	if(!dataSet.has(r.uploadDate))
		return;

	int row = dataSet.index(r.uploadDate);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}





Qt::ItemFlags DatabaseDescriptorModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < DatabaseDescriptor::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < DatabaseDescriptor::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant DatabaseDescriptorModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case DatabaseDescriptor::Name :
			v <<= dataSet.queryAt(index.row()).name;
			break;
				case DatabaseDescriptor::Driver :
			v <<= dataSet.queryAt(index.row()).driver;
			break;
				case DatabaseDescriptor::DatabaseName :
			v <<= dataSet.queryAt(index.row()).databaseName;
			break;
				case DatabaseDescriptor::Username :
			v <<= dataSet.queryAt(index.row()).username;
			break;
				case DatabaseDescriptor::Password :
			v <<= dataSet.queryAt(index.row()).password;
			break;
				case DatabaseDescriptor::SavePassword :
			v <<= dataSet.queryAt(index.row()).savePassword;
			break;
				case DatabaseDescriptor::Host :
			v <<= dataSet.queryAt(index.row()).host;
			break;
				case DatabaseDescriptor::Port :
			v <<= dataSet.queryAt(index.row()).port;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant DatabaseDescriptorModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case DatabaseDescriptor::Name :
			return QVariant(tr(TidDatabaseDescriptorFieldName));
			break;
		case DatabaseDescriptor::Driver :
			return QVariant(tr(TidDatabaseDescriptorFieldDriver));
			break;
		case DatabaseDescriptor::DatabaseName :
			return QVariant(tr(TidDatabaseDescriptorFieldDatabaseName));
			break;
		case DatabaseDescriptor::Username :
			return QVariant(tr(TidDatabaseDescriptorFieldUsername));
			break;
		case DatabaseDescriptor::Password :
			return QVariant(tr(TidDatabaseDescriptorFieldPassword));
			break;
		case DatabaseDescriptor::SavePassword :
			return QVariant(tr(TidDatabaseDescriptorFieldSavePassword));
			break;
		case DatabaseDescriptor::Host :
			return QVariant(tr(TidDatabaseDescriptorFieldHost));
			break;
		case DatabaseDescriptor::Port :
			return QVariant(tr(TidDatabaseDescriptorFieldPort));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool DatabaseDescriptorModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	DatabaseDescriptor modified(dataSet.queryAt(row));

	switch(index.column()){
	case DatabaseDescriptor::Name :
		modified.name <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::Driver :
		modified.driver <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::DatabaseName :
		modified.databaseName <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::Username :
		modified.username <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::Password :
		modified.password <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::SavePassword :
		modified.savePassword <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::Host :
		modified.host <<= value;
		update(row, modified);
		break;
		case DatabaseDescriptor::Port :
		modified.port <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool DatabaseDescriptorModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int DatabaseDescriptorModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int DatabaseDescriptorModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return DatabaseDescriptor::NumOfAllFields;
}

bool DatabaseDescriptorModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool DatabaseDescriptorModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void DatabaseDescriptorModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<DatabaseDescriptor::Fields>(column));
	dataSet.sort();
}

int DatabaseDescriptorModel::index(const csjp::String & name) const
{
	if(dataSet.has(name))
		return dataSet.index(name);
	else
		return -1;
}
const DatabaseDescriptor& DatabaseDescriptorModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void DatabaseDescriptorModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void DatabaseDescriptorModel::addNew(DatabaseDescriptor & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new DatabaseDescriptor(obj));
}

void DatabaseDescriptorModel::update(int row, DatabaseDescriptor & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new DatabaseDescriptor(modified));
	}
}



Qt::ItemFlags WareTypeModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < WareType::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < WareType::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant WareTypeModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case WareType::Ware :
			v <<= dataSet.queryAt(index.row()).ware;
			break;
				case WareType::Type :
			v <<= dataSet.queryAt(index.row()).type;
			break;
				case WareType::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case WareType::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant WareTypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case WareType::Ware :
			return QVariant(tr(TidWareTypeFieldWare));
			break;
		case WareType::Type :
			return QVariant(tr(TidWareTypeFieldType));
			break;
		case WareType::LastModified :
			return QVariant(tr(TidWareTypeFieldLastModified));
			break;
		case WareType::Deleted :
			return QVariant(tr(TidWareTypeFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool WareTypeModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	WareType modified(dataSet.queryAt(row));

	switch(index.column()){
	case WareType::Ware :
		modified.ware <<= value;
		update(row, modified);
		break;
		case WareType::Type :
		modified.type <<= value;
		update(row, modified);
		break;
		case WareType::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case WareType::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool WareTypeModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WareTypeModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int WareTypeModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return WareType::NumOfAllFields;
}

bool WareTypeModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WareTypeModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WareTypeModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<WareType::Fields>(column));
	dataSet.sort();
}

const WareType& WareTypeModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void WareTypeModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void WareTypeModel::addNew(WareType & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new WareType(obj));
}

void WareTypeModel::update(int row, WareType & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new WareType(modified));
	}
}



Qt::ItemFlags WareTagModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < WareTag::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < WareTag::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant WareTagModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case WareTag::Ware :
			v <<= dataSet.queryAt(index.row()).ware;
			break;
				case WareTag::Tag :
			v <<= dataSet.queryAt(index.row()).tag;
			break;
				case WareTag::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case WareTag::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant WareTagModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case WareTag::Ware :
			return QVariant(tr(TidWareTagFieldWare));
			break;
		case WareTag::Tag :
			return QVariant(tr(TidWareTagFieldTag));
			break;
		case WareTag::LastModified :
			return QVariant(tr(TidWareTagFieldLastModified));
			break;
		case WareTag::Deleted :
			return QVariant(tr(TidWareTagFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool WareTagModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	WareTag modified(dataSet.queryAt(row));

	switch(index.column()){
	case WareTag::Ware :
		modified.ware <<= value;
		update(row, modified);
		break;
		case WareTag::Tag :
		modified.tag <<= value;
		update(row, modified);
		break;
		case WareTag::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case WareTag::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool WareTagModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int WareTagModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int WareTagModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return WareTag::NumOfAllFields;
}

bool WareTagModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool WareTagModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void WareTagModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<WareTag::Fields>(column));
	dataSet.sort();
}

const WareTag& WareTagModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void WareTagModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void WareTagModel::addNew(WareTag & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new WareTag(obj));
}

void WareTagModel::update(int row, WareTag & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new WareTag(modified));
	}
}



Qt::ItemFlags ItemQueryWithTagModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWithTag::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWithTag::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemQueryWithTagModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case ItemQueryWithTag::Query :
			v <<= dataSet.queryAt(index.row()).query;
			break;
				case ItemQueryWithTag::Tag :
			v <<= dataSet.queryAt(index.row()).tag;
			break;
				case ItemQueryWithTag::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case ItemQueryWithTag::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant ItemQueryWithTagModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case ItemQueryWithTag::Query :
			return QVariant(tr(TidItemQueryWithTagFieldQuery));
			break;
		case ItemQueryWithTag::Tag :
			return QVariant(tr(TidItemQueryWithTagFieldTag));
			break;
		case ItemQueryWithTag::LastModified :
			return QVariant(tr(TidItemQueryWithTagFieldLastModified));
			break;
		case ItemQueryWithTag::Deleted :
			return QVariant(tr(TidItemQueryWithTagFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemQueryWithTagModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	ItemQueryWithTag modified(dataSet.queryAt(row));

	switch(index.column()){
	case ItemQueryWithTag::Query :
		modified.query <<= value;
		update(row, modified);
		break;
		case ItemQueryWithTag::Tag :
		modified.tag <<= value;
		update(row, modified);
		break;
		case ItemQueryWithTag::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case ItemQueryWithTag::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemQueryWithTagModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemQueryWithTagModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemQueryWithTagModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return ItemQueryWithTag::NumOfAllFields;
}

bool ItemQueryWithTagModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemQueryWithTagModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemQueryWithTagModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<ItemQueryWithTag::Fields>(column));
	dataSet.sort();
}

const ItemQueryWithTag& ItemQueryWithTagModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemQueryWithTagModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void ItemQueryWithTagModel::addNew(ItemQueryWithTag & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new ItemQueryWithTag(obj));
}

void ItemQueryWithTagModel::update(int row, ItemQueryWithTag & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQueryWithTag(modified));
	}
}



Qt::ItemFlags ItemQueryWithoutTagModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWithoutTag::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWithoutTag::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemQueryWithoutTagModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case ItemQueryWithoutTag::Query :
			v <<= dataSet.queryAt(index.row()).query;
			break;
				case ItemQueryWithoutTag::Tag :
			v <<= dataSet.queryAt(index.row()).tag;
			break;
				case ItemQueryWithoutTag::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case ItemQueryWithoutTag::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant ItemQueryWithoutTagModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case ItemQueryWithoutTag::Query :
			return QVariant(tr(TidItemQueryWithoutTagFieldQuery));
			break;
		case ItemQueryWithoutTag::Tag :
			return QVariant(tr(TidItemQueryWithoutTagFieldTag));
			break;
		case ItemQueryWithoutTag::LastModified :
			return QVariant(tr(TidItemQueryWithoutTagFieldLastModified));
			break;
		case ItemQueryWithoutTag::Deleted :
			return QVariant(tr(TidItemQueryWithoutTagFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemQueryWithoutTagModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	ItemQueryWithoutTag modified(dataSet.queryAt(row));

	switch(index.column()){
	case ItemQueryWithoutTag::Query :
		modified.query <<= value;
		update(row, modified);
		break;
		case ItemQueryWithoutTag::Tag :
		modified.tag <<= value;
		update(row, modified);
		break;
		case ItemQueryWithoutTag::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case ItemQueryWithoutTag::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemQueryWithoutTagModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemQueryWithoutTagModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemQueryWithoutTagModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return ItemQueryWithoutTag::NumOfAllFields;
}

bool ItemQueryWithoutTagModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemQueryWithoutTagModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemQueryWithoutTagModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<ItemQueryWithoutTag::Fields>(column));
	dataSet.sort();
}

const ItemQueryWithoutTag& ItemQueryWithoutTagModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemQueryWithoutTagModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void ItemQueryWithoutTagModel::addNew(ItemQueryWithoutTag & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new ItemQueryWithoutTag(obj));
}

void ItemQueryWithoutTagModel::update(int row, ItemQueryWithoutTag & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQueryWithoutTag(modified));
	}
}



Qt::ItemFlags ItemQueryWareModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWare::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < ItemQueryWare::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemQueryWareModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case ItemQueryWare::Query :
			v <<= dataSet.queryAt(index.row()).query;
			break;
				case ItemQueryWare::Ware :
			v <<= dataSet.queryAt(index.row()).ware;
			break;
				case ItemQueryWare::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case ItemQueryWare::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant ItemQueryWareModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case ItemQueryWare::Query :
			return QVariant(tr(TidItemQueryWareFieldQuery));
			break;
		case ItemQueryWare::Ware :
			return QVariant(tr(TidItemQueryWareFieldWare));
			break;
		case ItemQueryWare::LastModified :
			return QVariant(tr(TidItemQueryWareFieldLastModified));
			break;
		case ItemQueryWare::Deleted :
			return QVariant(tr(TidItemQueryWareFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemQueryWareModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	ItemQueryWare modified(dataSet.queryAt(row));

	switch(index.column()){
	case ItemQueryWare::Query :
		modified.query <<= value;
		update(row, modified);
		break;
		case ItemQueryWare::Ware :
		modified.ware <<= value;
		update(row, modified);
		break;
		case ItemQueryWare::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case ItemQueryWare::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemQueryWareModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemQueryWareModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemQueryWareModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return ItemQueryWare::NumOfAllFields;
}

bool ItemQueryWareModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemQueryWareModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemQueryWareModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<ItemQueryWare::Fields>(column));
	dataSet.sort();
}

const ItemQueryWare& ItemQueryWareModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemQueryWareModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void ItemQueryWareModel::addNew(ItemQueryWare & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new ItemQueryWare(obj));
}

void ItemQueryWareModel::update(int row, ItemQueryWare & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQueryWare(modified));
	}
}



Qt::ItemFlags ItemQueryPartnerModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < ItemQueryPartner::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < ItemQueryPartner::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant ItemQueryPartnerModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case ItemQueryPartner::Query :
			v <<= dataSet.queryAt(index.row()).query;
			break;
				case ItemQueryPartner::Partner :
			v <<= dataSet.queryAt(index.row()).partner;
			break;
				case ItemQueryPartner::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case ItemQueryPartner::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant ItemQueryPartnerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case ItemQueryPartner::Query :
			return QVariant(tr(TidItemQueryPartnerFieldQuery));
			break;
		case ItemQueryPartner::Partner :
			return QVariant(tr(TidItemQueryPartnerFieldPartner));
			break;
		case ItemQueryPartner::LastModified :
			return QVariant(tr(TidItemQueryPartnerFieldLastModified));
			break;
		case ItemQueryPartner::Deleted :
			return QVariant(tr(TidItemQueryPartnerFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemQueryPartnerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	ItemQueryPartner modified(dataSet.queryAt(row));

	switch(index.column()){
	case ItemQueryPartner::Query :
		modified.query <<= value;
		update(row, modified);
		break;
		case ItemQueryPartner::Partner :
		modified.partner <<= value;
		update(row, modified);
		break;
		case ItemQueryPartner::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case ItemQueryPartner::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool ItemQueryPartnerModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemQueryPartnerModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int ItemQueryPartnerModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return ItemQueryPartner::NumOfAllFields;
}

bool ItemQueryPartnerModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemQueryPartnerModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemQueryPartnerModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<ItemQueryPartner::Fields>(column));
	dataSet.sort();
}

const ItemQueryPartner& ItemQueryPartnerModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void ItemQueryPartnerModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void ItemQueryPartnerModel::addNew(ItemQueryPartner & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new ItemQueryPartner(obj));
}

void ItemQueryPartnerModel::update(int row, ItemQueryPartner & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new ItemQueryPartner(modified));
	}
}



Qt::ItemFlags PaymentQueryPartnerModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < PaymentQueryPartner::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < PaymentQueryPartner::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant PaymentQueryPartnerModel::data(const QModelIndex & index, int role) const 
{
	QVariant v;

	if(!index.isValid())
		return v;

	if(index.parent().isValid())
		return v;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return v;

	if((int)dataSet.size() <= index.row())
		return v;

	switch(index.column()){
		case PaymentQueryPartner::Query :
			v <<= dataSet.queryAt(index.row()).query;
			break;
				case PaymentQueryPartner::Partner :
			v <<= dataSet.queryAt(index.row()).partner;
			break;
				case PaymentQueryPartner::LastModified :
			v <<= dataSet.queryAt(index.row()).lastModified;
			break;
				case PaymentQueryPartner::Deleted :
			v <<= dataSet.queryAt(index.row()).deleted;
			break;
		
		default :
			return v;
	}

	return v;
}

QVariant PaymentQueryPartnerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case PaymentQueryPartner::Query :
			return QVariant(tr(TidPaymentQueryPartnerFieldQuery));
			break;
		case PaymentQueryPartner::Partner :
			return QVariant(tr(TidPaymentQueryPartnerFieldPartner));
			break;
		case PaymentQueryPartner::LastModified :
			return QVariant(tr(TidPaymentQueryPartnerFieldLastModified));
			break;
		case PaymentQueryPartner::Deleted :
			return QVariant(tr(TidPaymentQueryPartnerFieldDeleted));
			break;

		default :
			return QVariant();
	}

	return QVariant();
}

bool PaymentQueryPartnerModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	PaymentQueryPartner modified(dataSet.queryAt(row));

	switch(index.column()){
	case PaymentQueryPartner::Query :
		modified.query <<= value;
		update(row, modified);
		break;
		case PaymentQueryPartner::Partner :
		modified.partner <<= value;
		update(row, modified);
		break;
		case PaymentQueryPartner::LastModified :
		modified.lastModified <<= value;
		update(row, modified);
		break;
		case PaymentQueryPartner::Deleted :
		modified.deleted <<= value;
		update(row, modified);
		break;
	
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool PaymentQueryPartnerModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int PaymentQueryPartnerModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int PaymentQueryPartnerModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return PaymentQueryPartner::NumOfAllFields;
}

bool PaymentQueryPartnerModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool PaymentQueryPartnerModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void PaymentQueryPartnerModel::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<PaymentQueryPartner::Fields>(column));
	dataSet.sort();
}

const PaymentQueryPartner& PaymentQueryPartnerModel::data(int row) const
{
	return dataSet.queryAt(row);
}

void PaymentQueryPartnerModel::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void PaymentQueryPartnerModel::addNew(PaymentQueryPartner & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new PaymentQueryPartner(obj));
}

void PaymentQueryPartnerModel::update(int row, PaymentQueryPartner & modified)
{
	{
		ModelRemoveGuard g(this, QModelIndex(), row, row);
		dataSet.removeAt(row);
	}
	{
		ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
		dataSet.add(new PaymentQueryPartner(modified));
	}
}





ItemModel::ItemModel(SqlConnection & sql, const WareModel & wmodel) :
	sql(sql),
	set(dataSet),
	wmodel(wmodel)
{
	operationListeners.add(*this);
	opts.endDate = DateTime::now();
	opts.tagOption = ItemQueryTagOptions::MatchAny;
}

void ItemModel::query()
{
	ModelResetGuard g(this);
	dataSet = ItemSet::fromDb(sql, opts, stat);
}

bool ItemModel::queryFilter(const Item & modified)
{
	(void)(modified);
	return true;
}

PaymentModel::PaymentModel(SqlConnection & sql) :
	sql(sql),
	set(dataSet)
{
	operationListeners.add(*this);
	opts.endDate = DateTime::now();
}

void PaymentModel::query()
{
	ModelResetGuard g(this);
	dataSet = PaymentSet::fromDb(sql, opts, stat);
}

bool PaymentModel::queryFilter(const Payment & modified)
{
	(void)(modified);
	return true;
}


