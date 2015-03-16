/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include <butler_datamodel.h>

SCC TidTagFieldName			= QT_TRANSLATE_NOOP("TagModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagModel", "Description");
SCC TidTagFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidTagFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");

@declare@ Tag
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ WareTag
@include@ datamodel.cpp

@declare@ WareType
@include@ datamodel.cpp

SCC TidWareFieldName = QT_TRANSLATE_NOOP("WaresModel", "Name");
SCC TidWareFieldUnit = QT_TRANSLATE_NOOP("WaresModel", "Unit");
SCC TidWareFieldIcon = QT_TRANSLATE_NOOP("WaresModel", "Icon");
SCC TidWareFieldTypes = QT_TRANSLATE_NOOP("WaresModel", "Types");
SCC TidWareFieldTags = QT_TRANSLATE_NOOP("WaresModel", "Tags");
SCC TidWareFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidWareFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");

@declare@ Ware
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Company
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Brand
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Inventory
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Partner
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Account
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

@declare@ Payment
@include@ dbdatamodel_spec.cpp
@include@ dbdatamodel.cpp

SCC TidItemFieldUploadDate	= QT_TRANSLATE_NOOP("ItemsModel", "Upload date");
SCC TidItemFieldName		= QT_TRANSLATE_NOOP("ItemsModel", "Common name");
SCC TidItemFieldUnit		= QT_TRANSLATE_NOOP("ItemsModel", "Unit");
SCC TidItemFieldType		= QT_TRANSLATE_NOOP("ItemsModel", "Type");
SCC TidItemFieldBrand		= QT_TRANSLATE_NOOP("ItemsModel", "Brand");
SCC TidItemFieldQuantity	= QT_TRANSLATE_NOOP("ItemsModel", "Quantity");
SCC TidItemFieldPrice		= QT_TRANSLATE_NOOP("ItemsModel", "Price");
SCC TidItemFieldCurrency	= QT_TRANSLATE_NOOP("ItemsModel", "Currency");
SCC TidItemFieldAccount		= QT_TRANSLATE_NOOP("ItemsModel", "Account");
SCC TidItemFieldPartner		= QT_TRANSLATE_NOOP("ItemsModel", "Partner");
SCC TidItemFieldInventory	= QT_TRANSLATE_NOOP("ItemsModel", "Inventory");
SCC TidItemFieldComment		= QT_TRANSLATE_NOOP("ItemsModel", "Comment");
SCC TidItemFieldInvChangeDate	= QT_TRANSLATE_NOOP("ItemsModel", "Change date");
SCC TidItemFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidItemFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");
SCC TidItemFieldUnitPrice	= QT_TRANSLATE_NOOP("ItemsModel", "Unit price");
SCC TidItemFieldQuantityWithUnit	= QT_TRANSLATE_NOOP("ItemsModel", "Quantity");

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

@declare@ QueryWithTag
@include@ datamodel.cpp

@declare@ QueryWithoutTag
@include@ datamodel.cpp

@declare@ QueryWare
@include@ datamodel.cpp

@declare@ QueryPartner
@include@ datamodel.cpp

@declare@ Query
@include@ datamodel.cpp


