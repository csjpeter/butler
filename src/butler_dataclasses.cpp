/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <butler_dataclasses.h>


ItemSet ItemSet::fromDb(SqlConnection & sql, const ItemQuery & q, ItemQueryStat & stat)
{
	ItemSet list;
	SqlTransaction tr(sql);
	/* assemble command */
	csjp::Array<csjp::String> params;
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

	if(q.stockOption == ItemQueryStockOptions::Gains){
		if(filter.length)
			filter += " AND";
		filter += " 0 < quantity";
	} else if(q.stockOption == ItemQueryStockOptions::Looses){
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

	if(q.tagOption == ItemQueryTagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd << q.withTags.size();
	}

	SqlResult result = sql.exec(params, cmd);

	/* statistics */
	stat = ItemQueryStat();
	Double sumPrice;
	Double sumQuantity;

	DBG("----- Reading all Item from db:");
	for(auto & row : result){
		DBG("Next row");
		csjp::Object<Item> record;
		record->uploadDate <<= csjp::CString(row.value(0));
				record->name <<= csjp::CString(row.value(1));
				record->unit <<= csjp::CString(row.value(2));
				record->type <<= csjp::CString(row.value(3));
				record->brand <<= csjp::CString(row.value(4));
				record->quantity <<= csjp::CString(row.value(5));
				record->price <<= csjp::CString(row.value(6));
				record->currency <<= csjp::CString(row.value(7));
				record->account <<= csjp::CString(row.value(8));
				record->partner <<= csjp::CString(row.value(9));
				record->inventory <<= csjp::CString(row.value(10));
				record->comment <<= csjp::CString(row.value(11));
				record->invChangeDate <<= csjp::CString(row.value(12));
				record->lastModified <<= csjp::CString(row.value(13));
				record->deleted <<= csjp::CString(row.value(14));
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



PaymentSet PaymentSet::fromDb(SqlConnection & sql, const PaymentQuery & q, PaymentQueryStat & stat)
{
	PaymentSet list;
	SqlTransaction tr(sql);
	/* assemble command */
	csjp::Array<csjp::String> params;
	csjp::String cmd("SELECT"
			" MAX(payment.upload_date) AS upload_date,"
			" MAX(payment.amount) AS price,"
			" MAX(payment.account) AS account,"
			" MAX(payment.partner) AS partner,"
			" MAX(payment.comment) AS comment,"
			" MAX(payment.pay_date) AS pay_date"
			" FROM payment"
			" LEFT JOIN partner ON payment.partner = partner.name");

	csjp::String filter;

	if(filter.length)
		filter += " AND";
	filter += "? < pay_date";
	params.add(C_STR(q.startDate.isoUtcString()));

	if(filter.length)
		filter += " AND";
	filter += " pay_date < ?";
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
	stat = PaymentQueryStat();
	Double sumPrice;
	Double sumQuantity;

	DBG("----- Reading all Payment from db:");
	for(auto & row : result){
		DBG("Next row");
		csjp::Object<Payment> record;
		record->uploadDate <<= csjp::CString(row.value(0));
				record->account <<= csjp::CString(row.value(1));
				record->partner <<= csjp::CString(row.value(2));
				record->amount <<= csjp::CString(row.value(3));
				record->subject <<= csjp::CString(row.value(4));
				record->payDate <<= csjp::CString(row.value(5));
				record->comment <<= csjp::CString(row.value(6));
				record->lastModified <<= csjp::CString(row.value(7));
				record->deleted <<= csjp::CString(row.value(8));
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








	Tag Tag::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, description, last_modified, deleted "
				"FROM tag WHERE name = ?"
, _name				);

		Tag ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.description <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void Tag::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Tag orig;
			try{
			orig = Tag::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO tag (name, description, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, name, description, lastModified, deleted);
		
		tr.commit();
	}

	void Tag::toDb(SqlConnection & sql, const Tag & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE tag SET name = ?, description = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, description, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	TagSet TagSet::fromDb(
			SqlConnection & sql)
	{
		TagSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, description, last_modified, deleted "
				"FROM tag "
								);

		DBG("----- Reading all Tag from db:");
		for(auto & row : result){
			csjp::Object<Tag> record;
			record->name <<= csjp::CString(row.value(0));
						record->description <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	WareType WareType::fromDb(SqlConnection & sql, const Text & _ware, const Text & _type)
	{
		SqlResult result=sql.exec("SELECT ware, type, last_modified, deleted "
				"FROM ware_type WHERE ware = ?, type = ?"
, _ware, _type				);

		WareType ret;
		ret.ware <<= csjp::CString(result.value(0));
				ret.type <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void WareType::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	WareType orig;
			try{
			orig = WareType::fromDb(SqlConnection & sql, const Text & _ware, const Text & _type);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO ware_type (ware, type, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, ware, type, lastModified, deleted);
		
		tr.commit();
	}

	void WareType::toDb(SqlConnection & sql, const WareType & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE ware_type SET ware = ?, type = ?, last_modified = ?, deleted = ? "
				"WHERE ware = ? AND type = ?"
, ware, type, lastModified, deleted, orig.ware, orig.type);

		

		tr.commit();
	}



	WareTypeSet WareTypeSet::fromDb(
			SqlConnection & sql, const Text & _ware)
	{
		WareTypeSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT ware, type, last_modified, deleted "
				"FROM ware_type "
								" WHERE ware = ?"
, _ware								);

		DBG("----- Reading all WareType from db:");
		for(auto & row : result){
			csjp::Object<WareType> record;
			record->ware <<= csjp::CString(row.value(0));
						record->type <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	WareTag WareTag::fromDb(SqlConnection & sql, const Text & _ware, const Text & _tag)
	{
		SqlResult result=sql.exec("SELECT ware, tag, last_modified, deleted "
				"FROM ware_tag WHERE ware = ?, tag = ?"
, _ware, _tag				);

		WareTag ret;
		ret.ware <<= csjp::CString(result.value(0));
				ret.tag <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void WareTag::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	WareTag orig;
			try{
			orig = WareTag::fromDb(SqlConnection & sql, const Text & _ware, const Text & _tag);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO ware_tag (ware, tag, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, ware, tag, lastModified, deleted);
		
		tr.commit();
	}

	void WareTag::toDb(SqlConnection & sql, const WareTag & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE ware_tag SET ware = ?, tag = ?, last_modified = ?, deleted = ? "
				"WHERE ware = ? AND tag = ?"
, ware, tag, lastModified, deleted, orig.ware, orig.tag);

		

		tr.commit();
	}



	WareTagSet WareTagSet::fromDb(
			SqlConnection & sql, const Text & _ware)
	{
		WareTagSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT ware, tag, last_modified, deleted "
				"FROM ware_tag "
								" WHERE ware = ?"
, _ware								);

		DBG("----- Reading all WareTag from db:");
		for(auto & row : result){
			csjp::Object<WareTag> record;
			record->ware <<= csjp::CString(row.value(0));
						record->tag <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	Ware Ware::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, unit, icon, last_modified, deleted "
				"FROM ware WHERE name = ?"
, _name				);

		Ware ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.unit <<= csjp::CString(result.value(1));
				ret.icon <<= csjp::CString(result.value(2));
				ret.lastModified <<= csjp::CString(result.value(3));
				ret.deleted <<= csjp::CString(result.value(4));
			ret.types = WareTypeSet::fromDb(sql, _name);
					ret.tags = WareTagSet::fromDb(sql, _name);
				return ret;
	}

	void Ware::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Ware orig;
			try{
			orig = Ware::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO ware (name, unit, icon, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?)"
, name, unit, icon, lastModified, deleted);
		for(auto& item : types)
			item.toDb(sql);
				for(auto& item : tags)
			item.toDb(sql);
				tr.commit();
	}

	void Ware::toDb(SqlConnection & sql, const Ware & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE ware SET name = ?, unit = ?, icon = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, unit, icon, lastModified, deleted, orig.name);

		for(auto& item : types){
			if(orig.types.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.types){
			if(types.has(item)){
				auto & modified = types.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			WareType modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				for(auto& item : tags){
			if(orig.tags.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.tags){
			if(tags.has(item)){
				auto & modified = tags.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			WareTag modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				tr.commit();
	}



	WareSet WareSet::fromDb(
			SqlConnection & sql)
	{
		WareSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, unit, icon, last_modified, deleted "
				"FROM ware "
								);

		DBG("----- Reading all Ware from db:");
		for(auto & row : result){
			csjp::Object<Ware> record;
			record->name <<= csjp::CString(row.value(0));
						record->unit <<= csjp::CString(row.value(1));
						record->icon <<= csjp::CString(row.value(2));
						record->lastModified <<= csjp::CString(row.value(3));
						record->deleted <<= csjp::CString(row.value(4));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		for(auto& item : list)
			const_cast<Ware &>(item).types = WareTypeSet::fromDb(sql, item.name);
				for(auto& item : list)
			const_cast<Ware &>(item).tags = WareTagSet::fromDb(sql, item.name);
				return list;
	}



	Company Company::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, country, city, postal_code, address, tax_id, icon, last_modified, deleted "
				"FROM company WHERE name = ?"
, _name				);

		Company ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.country <<= csjp::CString(result.value(1));
				ret.city <<= csjp::CString(result.value(2));
				ret.postalCode <<= csjp::CString(result.value(3));
				ret.address <<= csjp::CString(result.value(4));
				ret.taxId <<= csjp::CString(result.value(5));
				ret.icon <<= csjp::CString(result.value(6));
				ret.lastModified <<= csjp::CString(result.value(7));
				ret.deleted <<= csjp::CString(result.value(8));
				

		return ret;
	}

	void Company::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Company orig;
			try{
			orig = Company::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO company (name, country, city, postal_code, address, tax_id, icon, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?, ?, ?)"
, name, country, city, postalCode, address, taxId, icon, lastModified, deleted);
		
		tr.commit();
	}

	void Company::toDb(SqlConnection & sql, const Company & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE company SET name = ?, country = ?, city = ?, postal_code = ?, address = ?, tax_id = ?, icon = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, country, city, postalCode, address, taxId, icon, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	CompanySet CompanySet::fromDb(
			SqlConnection & sql)
	{
		CompanySet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, country, city, postal_code, address, tax_id, icon, last_modified, deleted "
				"FROM company "
								);

		DBG("----- Reading all Company from db:");
		for(auto & row : result){
			csjp::Object<Company> record;
			record->name <<= csjp::CString(row.value(0));
						record->country <<= csjp::CString(row.value(1));
						record->city <<= csjp::CString(row.value(2));
						record->postalCode <<= csjp::CString(row.value(3));
						record->address <<= csjp::CString(row.value(4));
						record->taxId <<= csjp::CString(row.value(5));
						record->icon <<= csjp::CString(row.value(6));
						record->lastModified <<= csjp::CString(row.value(7));
						record->deleted <<= csjp::CString(row.value(8));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	Brand Brand::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, company, icon, last_modified, deleted "
				"FROM brand WHERE name = ?"
, _name				);

		Brand ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.company <<= csjp::CString(result.value(1));
				ret.icon <<= csjp::CString(result.value(2));
				ret.lastModified <<= csjp::CString(result.value(3));
				ret.deleted <<= csjp::CString(result.value(4));
				

		return ret;
	}

	void Brand::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Brand orig;
			try{
			orig = Brand::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO brand (name, company, icon, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?)"
, name, company, icon, lastModified, deleted);
		
		tr.commit();
	}

	void Brand::toDb(SqlConnection & sql, const Brand & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE brand SET name = ?, company = ?, icon = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, company, icon, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	BrandSet BrandSet::fromDb(
			SqlConnection & sql)
	{
		BrandSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, company, icon, last_modified, deleted "
				"FROM brand "
								);

		DBG("----- Reading all Brand from db:");
		for(auto & row : result){
			csjp::Object<Brand> record;
			record->name <<= csjp::CString(row.value(0));
						record->company <<= csjp::CString(row.value(1));
						record->icon <<= csjp::CString(row.value(2));
						record->lastModified <<= csjp::CString(row.value(3));
						record->deleted <<= csjp::CString(row.value(4));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	Inventory Inventory::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, comment, last_modified, deleted "
				"FROM inventory WHERE name = ?"
, _name				);

		Inventory ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.comment <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void Inventory::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Inventory orig;
			try{
			orig = Inventory::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO inventory (name, comment, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, name, comment, lastModified, deleted);
		
		tr.commit();
	}

	void Inventory::toDb(SqlConnection & sql, const Inventory & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE inventory SET name = ?, comment = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, comment, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	InventorySet InventorySet::fromDb(
			SqlConnection & sql)
	{
		InventorySet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, comment, last_modified, deleted "
				"FROM inventory "
								);

		DBG("----- Reading all Inventory from db:");
		for(auto & row : result){
			csjp::Object<Inventory> record;
			record->name <<= csjp::CString(row.value(0));
						record->comment <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	Partner Partner::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, country, city, postal_code, address, company, store_name, last_modified, deleted "
				"FROM partner WHERE name = ?"
, _name				);

		Partner ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.country <<= csjp::CString(result.value(1));
				ret.city <<= csjp::CString(result.value(2));
				ret.postalCode <<= csjp::CString(result.value(3));
				ret.address <<= csjp::CString(result.value(4));
				ret.company <<= csjp::CString(result.value(5));
				ret.storeName <<= csjp::CString(result.value(6));
				ret.lastModified <<= csjp::CString(result.value(7));
				ret.deleted <<= csjp::CString(result.value(8));
				

		return ret;
	}

	void Partner::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Partner orig;
			try{
			orig = Partner::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO partner (name, country, city, postal_code, address, company, store_name, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?, ?, ?)"
, name, country, city, postalCode, address, company, storeName, lastModified, deleted);
		
		tr.commit();
	}

	void Partner::toDb(SqlConnection & sql, const Partner & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE partner SET name = ?, country = ?, city = ?, postal_code = ?, address = ?, company = ?, store_name = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, country, city, postalCode, address, company, storeName, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	PartnerSet PartnerSet::fromDb(
			SqlConnection & sql)
	{
		PartnerSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, country, city, postal_code, address, company, store_name, last_modified, deleted "
				"FROM partner "
								);

		DBG("----- Reading all Partner from db:");
		for(auto & row : result){
			csjp::Object<Partner> record;
			record->name <<= csjp::CString(row.value(0));
						record->country <<= csjp::CString(row.value(1));
						record->city <<= csjp::CString(row.value(2));
						record->postalCode <<= csjp::CString(row.value(3));
						record->address <<= csjp::CString(row.value(4));
						record->company <<= csjp::CString(row.value(5));
						record->storeName <<= csjp::CString(row.value(6));
						record->lastModified <<= csjp::CString(row.value(7));
						record->deleted <<= csjp::CString(row.value(8));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	Account Account::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, currency, bank_office, iban, swift_code, last_modified, deleted "
				"FROM account WHERE name = ?"
, _name				);

		Account ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.currency <<= csjp::CString(result.value(1));
				ret.bankOffice <<= csjp::CString(result.value(2));
				ret.iban <<= csjp::CString(result.value(3));
				ret.swiftCode <<= csjp::CString(result.value(4));
				ret.lastModified <<= csjp::CString(result.value(5));
				ret.deleted <<= csjp::CString(result.value(6));
				

		return ret;
	}

	void Account::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Account orig;
			try{
			orig = Account::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO account (name, currency, bank_office, iban, swift_code, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?)"
, name, currency, bankOffice, iban, swiftCode, lastModified, deleted);
		
		tr.commit();
	}

	void Account::toDb(SqlConnection & sql, const Account & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE account SET name = ?, currency = ?, bank_office = ?, iban = ?, swift_code = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, currency, bankOffice, iban, swiftCode, lastModified, deleted, orig.name);

		

		tr.commit();
	}



	AccountSet AccountSet::fromDb(
			SqlConnection & sql)
	{
		AccountSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, currency, bank_office, iban, swift_code, last_modified, deleted "
				"FROM account "
								);

		DBG("----- Reading all Account from db:");
		for(auto & row : result){
			csjp::Object<Account> record;
			record->name <<= csjp::CString(row.value(0));
						record->currency <<= csjp::CString(row.value(1));
						record->bankOffice <<= csjp::CString(row.value(2));
						record->iban <<= csjp::CString(row.value(3));
						record->swiftCode <<= csjp::CString(row.value(4));
						record->lastModified <<= csjp::CString(row.value(5));
						record->deleted <<= csjp::CString(row.value(6));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	ItemQueryWithTag ItemQueryWithTag::fromDb(SqlConnection & sql, const Text & _query, const Text & _tag)
	{
		SqlResult result=sql.exec("SELECT query, tag, last_modified, deleted "
				"FROM item_query_with_tag WHERE query = ?, tag = ?"
, _query, _tag				);

		ItemQueryWithTag ret;
		ret.query <<= csjp::CString(result.value(0));
				ret.tag <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void ItemQueryWithTag::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	ItemQueryWithTag orig;
			try{
			orig = ItemQueryWithTag::fromDb(SqlConnection & sql, const Text & _query, const Text & _tag);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item_query_with_tag (query, tag, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, query, tag, lastModified, deleted);
		
		tr.commit();
	}

	void ItemQueryWithTag::toDb(SqlConnection & sql, const ItemQueryWithTag & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item_query_with_tag SET query = ?, tag = ?, last_modified = ?, deleted = ? "
				"WHERE query = ? AND tag = ?"
, query, tag, lastModified, deleted, orig.query, orig.tag);

		

		tr.commit();
	}



	ItemQueryWithTagSet ItemQueryWithTagSet::fromDb(
			SqlConnection & sql, const Text & _query)
	{
		ItemQueryWithTagSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT query, tag, last_modified, deleted "
				"FROM item_query_with_tag "
								" WHERE query = ?"
, _query								);

		DBG("----- Reading all ItemQueryWithTag from db:");
		for(auto & row : result){
			csjp::Object<ItemQueryWithTag> record;
			record->query <<= csjp::CString(row.value(0));
						record->tag <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	ItemQueryWithoutTag ItemQueryWithoutTag::fromDb(SqlConnection & sql, const Text & _query, const Text & _tag)
	{
		SqlResult result=sql.exec("SELECT query, tag, last_modified, deleted "
				"FROM item_query_without_tag WHERE query = ?, tag = ?"
, _query, _tag				);

		ItemQueryWithoutTag ret;
		ret.query <<= csjp::CString(result.value(0));
				ret.tag <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void ItemQueryWithoutTag::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	ItemQueryWithoutTag orig;
			try{
			orig = ItemQueryWithoutTag::fromDb(SqlConnection & sql, const Text & _query, const Text & _tag);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item_query_without_tag (query, tag, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, query, tag, lastModified, deleted);
		
		tr.commit();
	}

	void ItemQueryWithoutTag::toDb(SqlConnection & sql, const ItemQueryWithoutTag & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item_query_without_tag SET query = ?, tag = ?, last_modified = ?, deleted = ? "
				"WHERE query = ? AND tag = ?"
, query, tag, lastModified, deleted, orig.query, orig.tag);

		

		tr.commit();
	}



	ItemQueryWithoutTagSet ItemQueryWithoutTagSet::fromDb(
			SqlConnection & sql, const Text & _query)
	{
		ItemQueryWithoutTagSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT query, tag, last_modified, deleted "
				"FROM item_query_without_tag "
								" WHERE query = ?"
, _query								);

		DBG("----- Reading all ItemQueryWithoutTag from db:");
		for(auto & row : result){
			csjp::Object<ItemQueryWithoutTag> record;
			record->query <<= csjp::CString(row.value(0));
						record->tag <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	ItemQueryWare ItemQueryWare::fromDb(SqlConnection & sql, const Text & _query, const Text & _ware)
	{
		SqlResult result=sql.exec("SELECT query, ware, last_modified, deleted "
				"FROM item_query_ware WHERE query = ?, ware = ?"
, _query, _ware				);

		ItemQueryWare ret;
		ret.query <<= csjp::CString(result.value(0));
				ret.ware <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void ItemQueryWare::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	ItemQueryWare orig;
			try{
			orig = ItemQueryWare::fromDb(SqlConnection & sql, const Text & _query, const Text & _ware);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item_query_ware (query, ware, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, query, ware, lastModified, deleted);
		
		tr.commit();
	}

	void ItemQueryWare::toDb(SqlConnection & sql, const ItemQueryWare & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item_query_ware SET query = ?, ware = ?, last_modified = ?, deleted = ? "
				"WHERE query = ? AND ware = ?"
, query, ware, lastModified, deleted, orig.query, orig.ware);

		

		tr.commit();
	}



	ItemQueryWareSet ItemQueryWareSet::fromDb(
			SqlConnection & sql, const Text & _query)
	{
		ItemQueryWareSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT query, ware, last_modified, deleted "
				"FROM item_query_ware "
								" WHERE query = ?"
, _query								);

		DBG("----- Reading all ItemQueryWare from db:");
		for(auto & row : result){
			csjp::Object<ItemQueryWare> record;
			record->query <<= csjp::CString(row.value(0));
						record->ware <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	ItemQueryPartner ItemQueryPartner::fromDb(SqlConnection & sql, const Text & _query, const Text & _partner)
	{
		SqlResult result=sql.exec("SELECT query, partner, last_modified, deleted "
				"FROM item_query_partner WHERE query = ?, partner = ?"
, _query, _partner				);

		ItemQueryPartner ret;
		ret.query <<= csjp::CString(result.value(0));
				ret.partner <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void ItemQueryPartner::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	ItemQueryPartner orig;
			try{
			orig = ItemQueryPartner::fromDb(SqlConnection & sql, const Text & _query, const Text & _partner);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item_query_partner (query, partner, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, query, partner, lastModified, deleted);
		
		tr.commit();
	}

	void ItemQueryPartner::toDb(SqlConnection & sql, const ItemQueryPartner & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item_query_partner SET query = ?, partner = ?, last_modified = ?, deleted = ? "
				"WHERE query = ? AND partner = ?"
, query, partner, lastModified, deleted, orig.query, orig.partner);

		

		tr.commit();
	}



	ItemQueryPartnerSet ItemQueryPartnerSet::fromDb(
			SqlConnection & sql, const Text & _query)
	{
		ItemQueryPartnerSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT query, partner, last_modified, deleted "
				"FROM item_query_partner "
								" WHERE query = ?"
, _query								);

		DBG("----- Reading all ItemQueryPartner from db:");
		for(auto & row : result){
			csjp::Object<ItemQueryPartner> record;
			record->query <<= csjp::CString(row.value(0));
						record->partner <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}



	ItemQuery ItemQuery::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, start_date, end_date, stock_option, tag_option, last_modified, deleted "
				"FROM item_query WHERE name = ?"
, _name				);

		ItemQuery ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.startDate <<= csjp::CString(result.value(1));
				ret.endDate <<= csjp::CString(result.value(2));
				ret.stockOption <<= csjp::CString(result.value(3));
				ret.tagOption <<= csjp::CString(result.value(4));
				ret.lastModified <<= csjp::CString(result.value(5));
				ret.deleted <<= csjp::CString(result.value(6));
			ret.withTags = ItemQueryWithTagSet::fromDb(sql, _name);
					ret.withoutTags = ItemQueryWithoutTagSet::fromDb(sql, _name);
					ret.wares = ItemQueryWareSet::fromDb(sql, _name);
					ret.partners = ItemQueryPartnerSet::fromDb(sql, _name);
				return ret;
	}

	void ItemQuery::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	ItemQuery orig;
			try{
			orig = ItemQuery::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item_query (name, start_date, end_date, stock_option, tag_option, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?)"
, name, startDate, endDate, stockOption, tagOption, lastModified, deleted);
		for(auto& item : withTags)
			item.toDb(sql);
				for(auto& item : withoutTags)
			item.toDb(sql);
				for(auto& item : wares)
			item.toDb(sql);
				for(auto& item : partners)
			item.toDb(sql);
				tr.commit();
	}

	void ItemQuery::toDb(SqlConnection & sql, const ItemQuery & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item_query SET name = ?, start_date = ?, end_date = ?, stock_option = ?, tag_option = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, startDate, endDate, stockOption, tagOption, lastModified, deleted, orig.name);

		for(auto& item : withTags){
			if(orig.withTags.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.withTags){
			if(withTags.has(item)){
				auto & modified = withTags.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			ItemQueryWithTag modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				for(auto& item : withoutTags){
			if(orig.withoutTags.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.withoutTags){
			if(withoutTags.has(item)){
				auto & modified = withoutTags.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			ItemQueryWithoutTag modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				for(auto& item : wares){
			if(orig.wares.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.wares){
			if(wares.has(item)){
				auto & modified = wares.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			ItemQueryWare modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				for(auto& item : partners){
			if(orig.partners.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.partners){
			if(partners.has(item)){
				auto & modified = partners.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			ItemQueryPartner modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				tr.commit();
	}



	ItemQuerySet ItemQuerySet::fromDb(
			SqlConnection & sql)
	{
		ItemQuerySet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, start_date, end_date, stock_option, tag_option, last_modified, deleted "
				"FROM item_query "
								);

		DBG("----- Reading all ItemQuery from db:");
		for(auto & row : result){
			csjp::Object<ItemQuery> record;
			record->name <<= csjp::CString(row.value(0));
						record->startDate <<= csjp::CString(row.value(1));
						record->endDate <<= csjp::CString(row.value(2));
						record->stockOption <<= csjp::CString(row.value(3));
						record->tagOption <<= csjp::CString(row.value(4));
						record->lastModified <<= csjp::CString(row.value(5));
						record->deleted <<= csjp::CString(row.value(6));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		for(auto& item : list)
			const_cast<ItemQuery &>(item).withTags = ItemQueryWithTagSet::fromDb(sql, item.name);
				for(auto& item : list)
			const_cast<ItemQuery &>(item).withoutTags = ItemQueryWithoutTagSet::fromDb(sql, item.name);
				for(auto& item : list)
			const_cast<ItemQuery &>(item).wares = ItemQueryWareSet::fromDb(sql, item.name);
				for(auto& item : list)
			const_cast<ItemQuery &>(item).partners = ItemQueryPartnerSet::fromDb(sql, item.name);
				return list;
	}



	PaymentQuery PaymentQuery::fromDb(SqlConnection & sql, const Text & _name)
	{
		SqlResult result=sql.exec("SELECT name, start_date, end_date, last_modified, deleted "
				"FROM payment_query WHERE name = ?"
, _name				);

		PaymentQuery ret;
		ret.name <<= csjp::CString(result.value(0));
				ret.startDate <<= csjp::CString(result.value(1));
				ret.endDate <<= csjp::CString(result.value(2));
				ret.lastModified <<= csjp::CString(result.value(3));
				ret.deleted <<= csjp::CString(result.value(4));
			ret.partners = PaymentQueryPartnerSet::fromDb(sql, _name);
				return ret;
	}

	void PaymentQuery::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	PaymentQuery orig;
			try{
			orig = PaymentQuery::fromDb(SqlConnection & sql, const Text & _name);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO payment_query (name, start_date, end_date, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?)"
, name, startDate, endDate, lastModified, deleted);
		for(auto& item : partners)
			item.toDb(sql);
				tr.commit();
	}

	void PaymentQuery::toDb(SqlConnection & sql, const PaymentQuery & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE payment_query SET name = ?, start_date = ?, end_date = ?, last_modified = ?, deleted = ? "
				"WHERE name = ?"
, name, startDate, endDate, lastModified, deleted, orig.name);

		for(auto& item : partners){
			if(orig.partners.has(item))
				continue;
			item.toDb(sql);
		}
		for(auto& item : orig.partners){
			if(partners.has(item)){
				auto & modified = partners.query(item);
				if(item == modified)
					continue;
				modified.toDb(sql, item);
			}
			PaymentQueryPartner modified(item);
			modified.deleted = true;
			modified.toDb(sql, item);
		}
				tr.commit();
	}



	PaymentQuerySet PaymentQuerySet::fromDb(
			SqlConnection & sql)
	{
		PaymentQuerySet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT name, start_date, end_date, last_modified, deleted "
				"FROM payment_query "
								);

		DBG("----- Reading all PaymentQuery from db:");
		for(auto & row : result){
			csjp::Object<PaymentQuery> record;
			record->name <<= csjp::CString(row.value(0));
						record->startDate <<= csjp::CString(row.value(1));
						record->endDate <<= csjp::CString(row.value(2));
						record->lastModified <<= csjp::CString(row.value(3));
						record->deleted <<= csjp::CString(row.value(4));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		for(auto& item : list)
			const_cast<PaymentQuery &>(item).partners = PaymentQueryPartnerSet::fromDb(sql, item.name);
				return list;
	}



	PaymentQueryPartner PaymentQueryPartner::fromDb(SqlConnection & sql, const Text & _query, const Text & _partner)
	{
		SqlResult result=sql.exec("SELECT query, partner, last_modified, deleted "
				"FROM payment_query_partner WHERE query = ?, partner = ?"
, _query, _partner				);

		PaymentQueryPartner ret;
		ret.query <<= csjp::CString(result.value(0));
				ret.partner <<= csjp::CString(result.value(1));
				ret.lastModified <<= csjp::CString(result.value(2));
				ret.deleted <<= csjp::CString(result.value(3));
				

		return ret;
	}

	void PaymentQueryPartner::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	PaymentQueryPartner orig;
			try{
			orig = PaymentQueryPartner::fromDb(SqlConnection & sql, const Text & _query, const Text & _partner);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO payment_query_partner (query, partner, last_modified, deleted) VALUES ("
				"?, ?, ?, ?)"
, query, partner, lastModified, deleted);
		
		tr.commit();
	}

	void PaymentQueryPartner::toDb(SqlConnection & sql, const PaymentQueryPartner & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE payment_query_partner SET query = ?, partner = ?, last_modified = ?, deleted = ? "
				"WHERE query = ? AND partner = ?"
, query, partner, lastModified, deleted, orig.query, orig.partner);

		

		tr.commit();
	}



	PaymentQueryPartnerSet PaymentQueryPartnerSet::fromDb(
			SqlConnection & sql, const Text & _query)
	{
		PaymentQueryPartnerSet list;
		SqlTransaction tr(sql);
		SqlResult result=sql.exec("SELECT query, partner, last_modified, deleted "
				"FROM payment_query_partner "
								" WHERE query = ?"
, _query								);

		DBG("----- Reading all PaymentQueryPartner from db:");
		for(auto & row : result){
			csjp::Object<PaymentQueryPartner> record;
			record->query <<= csjp::CString(row.value(0));
						record->partner <<= csjp::CString(row.value(1));
						record->lastModified <<= csjp::CString(row.value(2));
						record->deleted <<= csjp::CString(row.value(3));
						DBG("Record: %", record->toJson());
			list.add(record);
		}
		DBG("-----");

		tr.commit();

		

		return list;
	}





	Item Item::fromDb(SqlConnection & sql, const DateTime & _uploadDate)
	{
		SqlResult result=sql.exec("SELECT upload_date, name, unit, type, brand, quantity, price, currency, account, partner, inventory, comment, inv_change_date, last_modified, deleted "
				"FROM item WHERE upload_date = ?"
, _uploadDate				);

		Item ret;
		ret.uploadDate <<= csjp::CString(result.value(0));
				ret.name <<= csjp::CString(result.value(1));
				ret.unit <<= csjp::CString(result.value(2));
				ret.type <<= csjp::CString(result.value(3));
				ret.brand <<= csjp::CString(result.value(4));
				ret.quantity <<= csjp::CString(result.value(5));
				ret.price <<= csjp::CString(result.value(6));
				ret.currency <<= csjp::CString(result.value(7));
				ret.account <<= csjp::CString(result.value(8));
				ret.partner <<= csjp::CString(result.value(9));
				ret.inventory <<= csjp::CString(result.value(10));
				ret.comment <<= csjp::CString(result.value(11));
				ret.invChangeDate <<= csjp::CString(result.value(12));
				ret.lastModified <<= csjp::CString(result.value(13));
				ret.deleted <<= csjp::CString(result.value(14));
				

		return ret;
	}

	void Item::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Item orig;
			try{
			orig = Item::fromDb(SqlConnection & sql, const DateTime & _uploadDate);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO item (upload_date, name, unit, type, brand, quantity, price, currency, account, partner, inventory, comment, inv_change_date, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)"
, uploadDate, name, unit, type, brand, quantity, price, currency, account, partner, inventory, comment, invChangeDate, lastModified, deleted);
		
		tr.commit();
	}

	void Item::toDb(SqlConnection & sql, const Item & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE item SET upload_date = ?, name = ?, unit = ?, type = ?, brand = ?, quantity = ?, price = ?, currency = ?, account = ?, partner = ?, inventory = ?, comment = ?, inv_change_date = ?, last_modified = ?, deleted = ? "
				"WHERE upload_date = ?"
, uploadDate, name, unit, type, brand, quantity, price, currency, account, partner, inventory, comment, invChangeDate, lastModified, deleted, orig.uploadDate);

		

		tr.commit();
	}



	Payment Payment::fromDb(SqlConnection & sql, const DateTime & _uploadDate)
	{
		SqlResult result=sql.exec("SELECT upload_date, account, partner, amount, subject, pay_date, comment, last_modified, deleted "
				"FROM payment WHERE upload_date = ?"
, _uploadDate				);

		Payment ret;
		ret.uploadDate <<= csjp::CString(result.value(0));
				ret.account <<= csjp::CString(result.value(1));
				ret.partner <<= csjp::CString(result.value(2));
				ret.amount <<= csjp::CString(result.value(3));
				ret.subject <<= csjp::CString(result.value(4));
				ret.payDate <<= csjp::CString(result.value(5));
				ret.comment <<= csjp::CString(result.value(6));
				ret.lastModified <<= csjp::CString(result.value(7));
				ret.deleted <<= csjp::CString(result.value(8));
				

		return ret;
	}

	void Payment::toDb(SqlConnection & sql) const
	{
		SqlTransaction tr(sql);
		/*	Payment orig;
			try{
			orig = Payment::fromDb(SqlConnection & sql, const DateTime & _uploadDate);
			} catch(csjp::Exception & e) {
			}
			toDb(sql, orig);
			*/
		sql.exec("INSERT INTO payment (upload_date, account, partner, amount, subject, pay_date, comment, last_modified, deleted) VALUES ("
				"?, ?, ?, ?, ?, ?, ?, ?, ?)"
, uploadDate, account, partner, amount, subject, payDate, comment, lastModified, deleted);
		
		tr.commit();
	}

	void Payment::toDb(SqlConnection & sql, const Payment & orig) const
	{
		SqlTransaction tr(sql);
		sql.exec("UPDATE payment SET upload_date = ?, account = ?, partner = ?, amount = ?, subject = ?, pay_date = ?, comment = ?, last_modified = ?, deleted = ? "
				"WHERE upload_date = ?"
, uploadDate, account, partner, amount, subject, payDate, comment, lastModified, deleted, orig.uploadDate);

		

		tr.commit();
	}





