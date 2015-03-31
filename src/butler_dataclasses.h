/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

#include <math.h>
#include <float.h>

#include <csjp_string.h>
#include <csjp_object.h>
#include <csjp_pod_array.h>
#include <csjp_array.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_sql_connection.h>


typedef csjp::OwnerContainer<Text> StringSet;

class QueryStat{
public:
	unsigned itemCount;
	double sumQuantity;
	double sumPrice;
	double avgPrice;
	double cheapestUnitPrice;
	double mostExpUnitPrice;
	double queryTime; /** How much time the query took. */
};

@Define{itemquery@
static @Type@Set fromDb(SqlConnection & sql, const Query & q, QueryStat & stat)
{
	@Type@Set list;
	SqlTransaction tr(sql);
	/* assemble command */
	csjp::Array<csjp::String> params;
	csjp::String cmd("SELECT"
			" MAX(Item.uploadDate) AS uploadDate,"
			" MAX(Item.name) AS name,"
			" MAX(Item.unit) AS unit,"
			" MAX(Item.type) AS type,"
			" MAX(Item.brand) AS brand,"
			" MAX(Item.quantity) AS quantity, "
			" MAX(Item.price) AS price,"
			" MAX(Item.currency) AS currency,"
			" MAX(Item.account) AS account,"
			" MAX(Item.partner) AS partner,"
			" MAX(Item.inventory) AS inventory,"
			" MAX(Item.comment) AS comment,"
			" MAX(Item.invChangeDate) AS invChangeDate"
			" FROM Item"
			" LEFT JOIN WareTag ON Item.name = WareTag.ware"
			" LEFT JOIN Partner ON Item.partner = Partner.name");

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
	filter += "? < invChangeDate";
	params.add(C_STR(q.startDate.isoUtcString()));

	if(filter.length)
		filter += " AND";
	filter += " invChangeDate < ?";
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
			wcmd += " Item.name = ?";
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
		tcmd += " Item.name NOT IN(SELECT name FROM WareTag WHERE ";
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
		cmd += " WHERE";
		cmd += filter;
	}

	cmd += " GROUP BY Item.uploadDate";

	if(q.tagOption == QueryTagOptions::MatchAll){
		cmd += " HAVING COUNT(*) = ";
		cmd << q.withTags.size();
	}

	if(sql.desc.driver == SqlDriver::PSql){
		auto s = params.size();
		for(size_t i = 0; i < s; i++){
			csjp::String p("$");
			p << i;
			cmd.replace("?", p.str, 0, cmd.length, 1);
		}
	}

	SqlResult result = sql.exec(params, cmd);

	/* statistics */
	stat.itemCount = 0;
	stat.sumQuantity = 0;
	stat.sumPrice = 0;
	stat.cheapestUnitPrice = DBL_MAX;
	stat.mostExpUnitPrice = 0;
	double sumPrice = 0;
	double sumQuantity = 0;

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

			double unitPrice = record->price / record->quantity;
			if(unitPrice < stat.cheapestUnitPrice)
				stat.cheapestUnitPrice = unitPrice;
			if(stat.mostExpUnitPrice < unitPrice)
				stat.mostExpUnitPrice = unitPrice;
		}

		list.add(record);
	}
	DBG("-----");

	stat.avgPrice = sumPrice / sumQuantity;
	if(!stat.itemCount){
		stat.cheapestUnitPrice = NAN;
		stat.mostExpUnitPrice = NAN;
	}

	tr.commit();

	return list;
}
@}Define@

@Define{dbquery@
static @Type@Set fromDb(SqlConnection & sql@For{LinkField@, const @.Type@ & _@.Name@@}@)
{
	@Type@Set list;
	SqlTransaction tr(sql);
	SqlResult result = sql.exec("SELECT @TableFieldList@ FROM @Type@"
@IfHasLinkField{@
			" WHERE @For{LinkField@@.Name@ = ?, @-@@.Name@ = ?@}@"
			@For{LinkField@, _@.Name@@}@
@}IfHasLinkField@
			);

	DBG("----- Reading all @Type@ from db:");
	for(auto & row : result){
		DBG("Next row");
		csjp::Object<@Type@> record;
@For{TableField@
		record->@.Name@ <<= csjp::CString(row.value(@.Idx@));
@}@
		list.add(record);
	}
	DBG("-----");

	tr.commit();

@For{SetField@
	for(auto& item : list)
		item.@.Name@ = @.SetSubType@Set::fromDb(sql@For{KeyField@, item.@.Name@@}@);
@}@

	return list;
}
@}Define@

@Define{todb@
static @Type@ fromDb(SqlConnection & sql@For{KeyField@, const @.Type@ & _@.Name@@}@)
{
	SqlResult result = sql.exec("SELECT @TableFieldList@ FROM @Type@"
			" WHERE @For{KeyField@@.Name@ = ?, @-@@.Name@ = ?@}@"
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

void toDb(SqlConnection & sql)
{
	SqlTransaction tr(sql);
/*	@Type@ orig;
	try{
		orig = @Type@::fromDb(SqlConnection & sql@For{KeyField@, const @.Type@ & _@.Name@@}@);
	} catch(csjp::Exception & e) {
	}
	toDb(sql, orig);
*/
	sql.exec("INSERT INTO @Type@ (@TableFieldList@) VALUES ("
		"@For{TableField@?, @-@?@}@)"
		@For{TableField@, @.Name@@}@);
@For{SetField@
	for(auto& item : @.Name@)
		item.toDb(sql);
@}@
	tr.commit();
}

void toDb(SqlConnection & sql, const @Type@ & orig)
{
	SqlTransaction tr(sql);
	sql.exec("UPDATE @Type@ SET @For{TableField@@.Name@ = ?, @-@@.Name@ = ?@}@ "
			"WHERE @For{KeyField@@.Name@ = ? AND @-@@.Name@ = ?@}@"
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

@ForTypes{Tag,WareType,WareTag@
class @Type@
{
	@include@ dataclass_members.h
	@include@ todb
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};
@}ForTypes@

inline bool operator<(const Text & a, const WareType & b) { return b.type.isMore(a); }
inline bool operator<(const WareType & a, const Text & b) { return a.type.isLess(b); }
inline bool operator<(const Text & a, const WareTag & b) { return b.tag.isMore(a); }
inline bool operator<(const WareTag & a, const Text & b) { return a.tag.isLess(b); }

@declare@ Ware
class Ware
{
	@include@ dataclass_members.h
	@include@ todb

	/* non-transactional */
	void setAsTypes(const QString & str)
	{
		QStringList sl;
		sl = str.split(",", QString::SkipEmptyParts);
		for(auto & s :sl)
			s = s.trimmed();
		for(auto & type : types)
			if(!sl.contains(type.type))
				type.deleted = true;
		for(auto & s : sl){
			Text type(s);
			if(!types.has(type))
				types.add(new WareType(name, type));
		}
	}

	/* non-transactional */
	void setAsTypes(const QStringList & strings)
	{
		for(auto & type : types)
			if(!strings.contains(type.type))
				type.deleted = true;
		for(auto & string : strings){
			Text type(string);
			if(!types.has(type))
				types.add(new WareType(name, type));
		}
	}

	/* non-transactional */
	void setAsTags(const QString & str)
	{
		QStringList sl;
		sl = str.split(",", QString::SkipEmptyParts);
		for(auto & s :sl)
			s = s.trimmed();
		for(auto & tag : tags)
			if(!sl.contains(tag.tag))
				tag.deleted = true;
		for(auto & s : sl){
			Text tag(s);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}

	/* non-transactional */
	void setAsTags(const QStringList & strings)
	{
		for(auto & tag : tags)
			if(!strings.contains(tag.tag))
				tag.deleted = true;
		for(auto & string : strings){
			Text tag(string);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}

	/* non-transactional */
	void setAsTags(const StringSet & stringSet)
	{
		for(auto & tag : tags)
			if(!stringSet.has(tag.tag))
				tag.deleted = true;
		for(auto & string : stringSet){
			Text tag(string);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};


@ForTypes{Company,Brand,Inventory,Partner,Account,Payment,QueryWithTag,QueryWithoutTag,QueryWare,QueryPartner@
class @Type@
{
	@include@ dataclass_members.h
	@include@ todb
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};
@}ForTypes@

inline bool operator<(const Text & a, const QueryWithTag & b) { return b.tag.isMore(a); }
inline bool operator<(const QueryWithTag & a, const Text & b) { return a.tag.isLess(b); }
inline bool operator<(const Text & a, const QueryWithoutTag & b) { return b.tag.isMore(a); }
inline bool operator<(const QueryWithoutTag & a, const Text & b) { return a.tag.isLess(b); }


@declare@ Query
class Query
{
	@include@ dataclass_members.h
	@include@ todb

	/* non-transactional */
	void setAsWithTags(const StringSet & names)
	{
		for(auto & tag : withTags)
			if(!names.has(tag.tag))
				tag.deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withTags.has(tag))
				withTags.add(new QueryWithTag(name, tag));
		}
	}
	/* non-transactional */
	void setAsWithoutTags(const StringSet & names)
	{
		for(auto & tag : withoutTags)
			if(!names.has(tag.tag))
				tag.deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withoutTags.has(tag))
				withoutTags.add(new QueryWithoutTag(name, tag));
		}
	}
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};


@ForTypes{Item@
class @Type@
{
	@include@ dataclass_members.h
	@include@ todb
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ itemquery
};
@}ForTypes@






inline QStringList & operator<<=(QStringList & list, const WareTypeSet & wareTypes)
{
	list.clear();
	for(auto & wareType : wareTypes)
		list.append(wareType.type);
	return list;
}

inline QString & operator<<=(QString & str, const WareTypeSet & wareTypes)
{
	str.clear();
	for(auto & wareType : wareTypes){
		if(str.length())
			str.append(", ");
		str.append(wareType.type);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.add(new Text(wareTag.tag));
	return list;
}

inline QStringList & operator<<=(QStringList & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.append(wareTag.tag);
	return list;
}

inline QString & operator<<=(QString & str, const WareTagSet & wareTags)
{
	str.clear();
	for(auto & wareTag : wareTags){
		if(str.length())
			str.append(", ");
		str.append(wareTag.tag);
	}
	return str;
}

inline QString & operator<<=(QString & str, const QueryWithTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const QueryWithTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const QueryWithoutTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const QueryWithoutTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const QueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}

inline QString & operator<<=(QString & str, const QueryWareSet & wares)
{
	str.clear();
	for(auto & ware : wares){
		if(str.size())
			str.append(", ");
		str.append(ware.ware);
	}
	return str;
}

inline bool operator==(const WareTypeSet & a, const QString & b)
{
	QStringList sl;
	sl = b.split(",", QString::SkipEmptyParts);
	unsigned s = sl.size();
	if(s != a.size())
		return false;
	unsigned i;
	for(i=0; i<s; i++)
		if(!a.has(Text(sl.at(i).trimmed())))
			return false;
	return true;
}

inline bool operator==(const WareTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const WareTagSet & a, const StringSet & b){ return !(a == b); }

inline bool operator==(const QueryWithTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const QueryWithTagSet & a, const StringSet & b){ return !(a == b); }

inline bool operator==(const QueryWithoutTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const QueryWithoutTagSet & a, const StringSet & b){ return !(a == b); }

#endif
