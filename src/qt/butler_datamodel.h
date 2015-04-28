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


@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
class @Type@Model : public AbstractTableModel
{
@include@ dbdatamodel.h
@include@ dbdatamodel_spec.h
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }
@}ForTypes@


@declare@ Item
class @Type@Model : public AbstractTableModel
{
@include@ dbdatamodel.h
public:
	@Type@Model(SqlConnection & sql, const WareModel & wmodel);
	const WareModel & wmodel;
private:
/*	QVariant dataUnitPrice(int row) const
	{
		auto & item = dataSet.queryAt(row);
		Double unitPrice(item.price / item.quantity);
		return QVariant(unitPrice.val);
	}
	QVariant dataQuantityWithUnit(int row) const
	{
		auto & item = dataSet.queryAt(row);
		int i = wmodel.index(item.name);
		QString val;
		val.setNum(item.quantity.val, 'g', 3);
		if(i != -1) {
			const Ware & w = wmodel.data(i);
			val += " ";
			val += w.unit;
		}
		return QVariant(val);
	}*/
public:
	@Type@Query opts;
	@Type@QueryStat stat;
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }

@declare@ Payment
class @Type@Model : public AbstractTableModel
{
@include@ dbdatamodel.h
public:
	@Type@Model(SqlConnection & sql);
public:
	@Type@Query opts;
	@Type@QueryStat stat;
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }


@ForTypes{DatabaseDescriptor,WareType,WareTag,ItemQueryWithTag,ItemQueryWithoutTag,ItemQueryWare
				ItemQueryPartner,PaymentQueryPartner@
class @Type@Model : public AbstractTableModel
{
@include@ datamodel.h
};
inline bool operator==(const @Type@Model & a, const @Type@Model & b) { return &a == &b; }
@}ForTypes@


#endif
