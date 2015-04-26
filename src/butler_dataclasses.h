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
#include <csjp_json.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_sql_connection.h>


typedef csjp::OwnerContainer<Text> StringSet;

class ItemQueryStat{
public:
	ItemQueryStat() : cheapestUnitPrice(DBL_MAX) {}
	UInt itemCount;
	Double sumQuantity;
	Double sumPrice;
	Double avgPrice;
	Double cheapestUnitPrice;
	Double mostExpUnitPrice;
	Double queryTime; /** How much time the query took. */
};

class PaymentQueryStat{
public:
	PaymentQueryStat() : cheapestPrice(DBL_MAX) {}
	UInt paymentCount;
	Double sumPrice;
	Double avgPrice;
	Double cheapestPrice;
	Double mostExpPrice;
	Double queryTime; /** How much time the query took. */
};

@Define{dbquery@
	static @Type@Set fromDb(SqlConnection & sql@For{LinkField@, const @.Type@ & _@.Name@@}@);
@}Define@

@Define{todb@
	static @Type@ fromDb(SqlConnection & sql@For{KeyField@, const @.Type@ & _@.Name@@}@);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const @Type@ & orig) const;
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
		for(auto & s : sl)
			s = s.trimmed();
		for(auto & type : types)
			if(!sl.contains(type.type))
				const_cast<WareType &>(type).deleted = true;
		for(auto & s : sl){
			Text type(s);
			if(!types.has(type))
				types.add(new WareType(name, type));
		}
	}
#if 0
	/* non-transactional */
	void setAsTypes(const QStringList & strings)
	{
		for(auto & type : types)
			if(!strings.contains(type.type))
				const_cast<WareType &>(type).deleted = true;
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
				const_cast<WareTag &>(tag).deleted = true;
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
				const_cast<WareTag &>(tag).deleted = true;
		for(auto & string : strings){
			Text tag(string);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}
#endif
	/* non-transactional */
	void setAsTags(const StringSet & stringSet)
	{
		for(auto & tag : tags)
			if(!stringSet.has(tag.tag))
				const_cast<WareTag &>(tag).deleted = true;
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


@ForTypes{Company,Brand,Inventory,Partner,Account,ItemQueryWithTag,ItemQueryWithoutTag,ItemQueryWare
				ItemQueryPartner,PaymentQueryPartner,PaymentQuery@
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

inline bool operator<(const Text & a, const ItemQueryWithTag & b) { return b.tag.isMore(a); }
inline bool operator<(const ItemQueryWithTag & a, const Text & b) { return a.tag.isLess(b); }
inline bool operator<(const Text & a, const ItemQueryWithoutTag & b) { return b.tag.isMore(a); }
inline bool operator<(const ItemQueryWithoutTag & a, const Text & b) { return a.tag.isLess(b); }


@declare@ ItemQuery
class ItemQuery
{
	@include@ dataclass_members.h
	@include@ todb

	/* non-transactional */
	void setAsWithTags(const StringSet & names)
	{
		for(auto & tag : withTags)
			if(!names.has(tag.tag))
				const_cast<ItemQueryWithTag &>(tag).deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withTags.has(tag))
				withTags.add(new ItemQueryWithTag(name, tag));
		}
	}
	/* non-transactional */
	void setAsWithoutTags(const StringSet & names)
	{
		for(auto & tag : withoutTags)
			if(!names.has(tag.tag))
				const_cast<ItemQueryWithoutTag &>(tag).deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withoutTags.has(tag))
				withoutTags.add(new ItemQueryWithoutTag(name, tag));
		}
	}
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	@include@ dbquery
};


@ForTypes{Item,Payment@
class @Type@
{
	@include@ dataclass_members.h
	@include@ todb
};
@include@ dataclass_nonmembers.h
class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
	@include@ dataclass_set.h
	static @Type@Set fromDb(SqlConnection & sql, const @Type@Query & q, @Type@QueryStat & stat);
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

inline QString & operator<<=(QString & str, const ItemQueryWithTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const ItemQueryWithTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const ItemQueryWithoutTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const ItemQueryWithoutTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const ItemQueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}

inline QString & operator<<=(QString & str, const PaymentQueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}

inline QString & operator<<=(QString & str, const ItemQueryWareSet & wares)
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

inline bool operator==(const ItemQueryWithTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const ItemQueryWithTagSet & a, const StringSet & b){ return !(a == b); }

inline bool operator==(const ItemQueryWithoutTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const ItemQueryWithoutTagSet & a, const StringSet & b){ return !(a == b); }

#endif
