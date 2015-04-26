/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <butler_dataclasses.h>

@ForTypes{Item@
@Type@Set @Type@Set::fromDb(SqlConnection & sql, const @Type@Query & q, @Type@QueryStat & stat)
{
	@Type@Set list;
	SqlTransaction tr(sql);
	/* assemble command */
	csjp::Array<csjp::String> params(128); // FIXME lets make it autoresizing
	csjp::String cmd("SELECT"
			" MAX(item.upload_date) AS upload_date,"
			" MAX(item.name) AS name,"
			" MAX(item.unit) AS unit,"
			" MAX(item.type) AS type,"
			" MAX(item.brand) AS brand,"
			" MAX(item.quantity) AS quantity, "
			" MAX(item.price) AS price,"
			" MAX(item.currency) AS currency,"
			" MAX(item.account) AS account,"
			" MAX(item.partner) AS partner,"
			" MAX(item.inventory) AS inventory,"
			" MAX(item.comment) AS comment,"
			" MAX(item.inv_change_date) AS inv_change_date"
			" FROM item"
			" LEFT JOIN ware_tag ON item.name = ware_tag.ware"
			" LEFT JOIN partner ON item.partner = partner.name");

	csjp::String filter;

	if(q.stockOption == QueryStockOptions::Gains){
		if(filter.length)
			filter += " AND";
		filter += " 0 < quantity";
	} else if(q.stockOption == QueryStockOptions::Looses){
		if(filter.length)
			filter += " AND";
		filter += " quantity < 0";
	}

	if(filter.length)
		filter += " AND";
	filter += "? < inv_change_date";
	params.add(C_STR(q.startDate.isoUtcString()));

	if(filter.length)
		filter += " AND";
	filter += " inv_change_date < ?";
	params.add(C_STR(q.endDate.isoUtcString()));

	{
		unsigned i, s = q.withTags.size();
		csjp::String tcmd;
		if(filter.length)
			tcmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = ?";
			params.add(C_STR(q.withTags.queryAt(i).tag));
		}
		tcmd.append(")");
		if(0 < s)
			filter.append(tcmd);
	}

	{
		unsigned i, s = q.wares.size();
		csjp::String wcmd;
		if(filter.length)
			wcmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				wcmd += " OR";
			wcmd += " item.name = ?";
			params.add(C_STR(q.wares.queryAt(i).ware));
		}
		wcmd.append(")");
		if(0 < s)
			filter.append(wcmd);
	}

	{
		unsigned i, s = q.partners.size();
		csjp::String scmd;
		if(filter.length)
			scmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				scmd += " OR";
			scmd += " partners.name = ?";
			params.add(C_STR(q.partners.queryAt(i).partner));
		}
		scmd.append(")");
		if(0 < s)
			filter.append(scmd);
	}

	{
		unsigned i, s = q.withoutTags.size();
		csjp::String tcmd;
		if(filter.length)
			tcmd += " AND";
		tcmd += " item.name NOT IN(SELECT name FROM ware_tag WHERE ";
		for(i=0; i<s; i++){
			if(0 < i)
				tcmd += " OR";
			tcmd += " tag = ?";
			params.add(C_STR(q.withoutTags.queryAt(i).tag));
		}
		tcmd += " GROUP BY name)";
		if(0 < s)
			filter.append(tcmd);
	}

	if(filter.length){
		cmd += " WHERE ";
		cmd += filter;
	}

	cmd += " GROUP BY item.upload_date";

	if(q.tagOption == QueryTagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd << q.withTags.size();
	}

	SqlResult result = sql.exec(params, cmd);

	/* statistics */
	stat = @Type@QueryStat();
	Double sumPrice;
	Double sumQuantity;

	DBG("----- Reading all @Type@ from db:");
	for(auto & row : result){
		DBG("Next row");
		csjp::Object<@Type@> record;
		@For{TableField@
		record->@.Name@ <<= csjp::CString(row.value(@.Idx@));
		@}@

		/* statistics */
		stat.itemCount++;
		stat.sumQuantity += record->quantity;
		stat.sumPrice += record->price;
		if(DBL_EPSILON <= record->quantity && DBL_EPSILON <= record->price){
			sumQuantity += record->quantity;
			sumPrice += record->price;

			Double unitPrice(record->price / record->quantity);
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}

		list.add(record);
	}
	DBG("-----");

	stat.avgPrice = sumPrice / sumQuantity;
	if(stat.itemCount == 0){
		stat.cheapestUnitPrice.nan();
		stat.mostExpUnitPrice.nan();
	}

	tr.commit();

	return list;
}
@}ForTypes@

@ForTypes{Payment@
@Type@Set @Type@Set::fromDb(SqlConnection & sql, const @Type@Query & q, @Type@QueryStat & stat)
{
	@Type@Set list;
	SqlTransaction tr(sql);
	/* assemble command */
	csjp::Array<csjp::String> params(128); // FIXME lets make it autoresizing
	csjp::String cmd("SELECT"
			" MAX(payment.upload_date) AS upload_date,"
			" MAX(payment.name) AS name,"
			" MAX(payment.price) AS price,"
			" MAX(payment.currency) AS currency,"
			" MAX(payment.account) AS account,"
			" MAX(payment.partner) AS partner,"
			" MAX(payment.comment) AS comment,"
			" MAX(payment.inv_change_date) AS inv_change_date"
			" FROM payment"
			" LEFT JOIN ware_tag ON payment.name = ware_tag.ware"
			" LEFT JOIN partner ON payment.partner = partner.name");

	csjp::String filter;

	if(filter.length)
		filter += " AND";
	filter += "? < inv_change_date";
	params.add(C_STR(q.startDate.isoUtcString()));

	if(filter.length)
		filter += " AND";
	filter += " inv_change_date < ?";
	params.add(C_STR(q.endDate.isoUtcString()));

	{
		unsigned i, s = q.partners.size();
		csjp::String scmd;
		if(filter.length)
			scmd += " AND ( ";
		for(i=0; i<s; i++){
			if(0 < i)
				scmd += " OR";
			scmd += " partners.name = ?";
			params.add(C_STR(q.partners.queryAt(i).partner));
		}
		scmd.append(")");
		if(0 < s)
			filter.append(scmd);
	}

	if(filter.length){
		cmd += " WHERE ";
		cmd += filter;
	}

	cmd += " GROUP BY payment.upload_date";

	SqlResult result = sql.exec(params, cmd);

	/* statistics */
	stat = @Type@QueryStat();
	Double sumPrice;
	Double sumQuantity;

	DBG("----- Reading all @Type@ from db:");
	for(auto & row : result){
		DBG("Next row");
		csjp::Object<@Type@> record;
		@For{TableField@
		record->@.Name@ <<= csjp::CString(row.value(@.Idx@));
		@}@

		/* statistics */
		stat.paymentCount++;
		stat.sumPrice += record->amount;
		if(DBL_EPSILON <= record->amount){
			sumPrice += record->amount;

			if(record->amount < stat.cheapestPrice)
				stat.cheapestPrice = record->amount;
			if(stat.mostExpPrice < record->amount)
				stat.mostExpPrice = record->amount;
		}

		list.add(record);
	}
	DBG("-----");

	stat.avgPrice = sumPrice / sumQuantity;
	if(stat.paymentCount == 0){
		stat.cheapestPrice.nan();
		stat.mostExpPrice.nan();
	}

	tr.commit();

	return list;
}
@}ForTypes@

@Define{dbquery@
	@Type@Set @Type@Set::fromDb(
			SqlConnection & sql@For{LinkField@, const @.Type@ & _@.Name@@}@)
	{
		@Type@Set list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT @For{TableField@@.colName@, @-@@.colName@@}@ "
				"FROM @TableName@ "
				@IfHasLinkField{@
				" WHERE @For{LinkField@@.colName@ = ?, @-@@.colName@ = ?@}@"
				@For{LinkField@, _@.Name@@}@
				@}IfHasLinkField@
				);

		DBG("----- Reading all @Type@ from db:");
		for(auto & row : result){
			csjp::Object<@Type@> record;
			@For{TableField@
			record->@.Name@ <<= csjp::CString(row.value(@.Idx@));
			@}@
			DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		@For{SetField@
		for(auto& item : list)
			const_cast<@Type@ &>(item).@.Name@ = @.SetSubType@Set::fromDb(sql@For{KeyField@, item.@.Name@@}@);
		@}@

		return list;
	}
@}Define@

@Define{todb@
	@Type@ @Type@::fromDb(SqlConnection & sql@For{KeyField@, const @.Type@ & _@.Name@@}@)
	{
		SqlResult result=sql.exec("SELECT @For{TableField@@.colName@, @-@@.colName@@}@ "
				"FROM @TableName@ WHERE @For{KeyField@@.colName@ = ?, @-@@.colName@ = ?@}@"
				@For{KeyField@, _@.Name@@}@
				);

		@Type@ ret;
		@For{TableField@
		ret.@.Name@ <<= csjp::CString(result.value(@.Idx@));
		@}@

		@For{SetField@
			ret.@.Name@ = @.SetSubType@Set::fromDb(sql@For{KeyField@, _@.Name@@}@);
		@}@

		return ret;
	}

	void @Type@::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	@Type@ orig;
			try{
			orig = @Type@::fromDb(SqlConnection & sql@For{KeyField@, const @.Type@ & _@.Name@@}@);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO @TableName@ (@For{TableField@@.colName@, @-@@.colName@@}@) VALUES ("
				"@For{TableField@?, @-@?@}@)"
				@For{TableField@, @.Name@@}@);
		@For{SetField@
		for(auto& item : @.Name@)
			item.toDb(sql);
		@}@
		tr.commit();
	}

	void @Type@::toDb(SqlConnection & sql, const @Type@ & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE @TableName@ SET @For{TableField@@.colName@ = ?, @-@@.colName@ = ?@}@ "
				"WHERE @For{KeyField@@.colName@ = ? AND @-@@.colName@ = ?@}@"
				@For{TableField@, @.Name@@}@
				@For{KeyField@, orig.@.Name@@}@);

		@For{SetField@
		for(auto& item : @.Name@){
			if(orig.@.Name@.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.@.Name@){
			if(@.Name@.has(item)){
				auto & modified = @.Name@.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			@.SetSubType@ modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
		@}@

		tr.commit();
	}
@}Define@

@ForTypes{Tag,WareType,WareTag,Ware
		Company,Brand,Inventory,Partner,Account,
		ItemQueryWithTag,ItemQueryWithoutTag,ItemQueryWare,ItemQueryPartner
		ItemQuery,PaymentQueryPartner,PaymentQuery@
@include@ todb

@include@ dbquery
@}ForTypes@

@ForTypes{Item,Payment@
@include@ todb
@}ForTypes@


