/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

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

@ForTypes{Tag,WareType,WareTag@
class @Type@
{
	@include@ dataclass_members.h
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
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};
@}ForTypes@
@ForTypes{Item@
class @Type@
{
	@include@ dataclass_members.h
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
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


#endif
