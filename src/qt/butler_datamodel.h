/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATAMODEL_H
#define BUTLER_DATAMODEL_H

#include <csjp_reference_container.h>

#include <butler_abstract_table_model.h>
#include <butler_dbclasses.h>
#include <butler_config.h>

@declare@ Tag
class TagModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ WareTag
class WareTagModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ WareType
class WareTypeModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ Ware
class WareModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Company
class CompanyModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Brand
class BrandModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Inventory
class InventoryModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Partner
class PartnerModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Account
class AccountModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Payment
class PaymentModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};

@declare@ Item
class ItemModel : public AbstractTableModel
{
@include@ dbdatamodel.h
public:
	ItemModel(SqlConnection & sql, const WareModel & wmodel);
	const WareModel & wmodel;
private:
	QVariant dataUnitPrice(int row) const
	{
		Item & item = dataSet.queryAt(row);
		double unitPrice = 0;
		//if(isnormal(item.quantity)) /*FIXME*/
		unitPrice = item.price / item.quantity;
		return QVariant(unitPrice);
	}
	QVariant dataQuantityWithUnit(int row) const
	{
		Item & item = dataSet.queryAt(row);
		int i = wmodel.index(item.name);
		QString val;
		val.setNum(item.quantity, 'g', 3);
		if(i != -1) {
			const Ware & w = wmodel.data(i);
			val += " ";
			val += w.unit;
		}
		return QVariant(val);
	}
public:
	Query opts;
	QueryStat stat;
};

@declare@ QueryWithTag
class QueryWithTagModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ QueryWithoutTag
class QueryWithoutTagModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ QueryWare
class QueryWareModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ QueryPartner
class QueryPartnerModel : public AbstractTableModel
{
@include@ datamodel.h
};

@declare@ Query
class QueryModel : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};


#endif
