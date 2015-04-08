/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATAMODEL_H
#define BUTLER_DATAMODEL_H

#include <csjp_reference_container.h>

#include <butler_conversions.h>
#include <butler_abstract_table_model.h>
#include <butler_config.h>


@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,Payment,Query@
class @Type@Model : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }
@}ForTypes@


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
		auto & item = dataSet.queryAt(row);
		double unitPrice = 0;
		//if(isnormal(item.quantity)) /*FIXME*/
		unitPrice = item.price / item.quantity;
		return QVariant(unitPrice);
	}
	QVariant dataQuantityWithUnit(int row) const
	{
		auto & item = dataSet.queryAt(row);
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
inline bool operator==(const ItemModel & a, const ItemModel & b) { return &a == &b; }


@ForTypes{DatabaseDescriptor,WareType,WareTag,QueryWithTag,QueryWithoutTag,QueryWare,QueryPartner@
class @Type@Model : public AbstractTableModel
{
@include@ datamodel.h
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }
@}ForTypes@


#endif
