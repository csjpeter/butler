/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

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


@declare@ Tag
class Tag
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ WareType
class WareType
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
inline bool operator<(const Text & a, const WareType & b) { return b.type.isMore(a); }
inline bool operator<(const WareType & a, const Text & b) { return a.type.isLess(b); }
@include@ dataclass_set.h

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
/* non-transactional */
inline QStringList & operator<<= (QStringList & list, const WareTypeSet & wareTypes)
{
	list.clear();
	for(auto & wareType : wareTypes)
		list.append(wareType.type);
	return list;
}
/* non-transactional */
inline QString & operator<<= (QString & str, const WareTypeSet & wareTypes)
{
	str.clear();
	for(auto & wareType : wareTypes){
		if(str.length())
			str.append(", ");
		str.append(wareType.type);
	}
	return str;
}


@declare@ WareTag
class WareTag
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
inline bool operator<(const Text & a, const WareTag & b) { return b.tag.isMore(a); }
inline bool operator<(const WareTag & a, const Text & b) { return a.tag.isLess(b); }
@include@ dataclass_set.h

/* non-transactional */
inline StringSet & operator<<= (StringSet & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.add(new Text(wareTag.tag));
	return list;
}
/* non-transactional */
inline QStringList & operator<<= (QStringList & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.append(wareTag.tag);
	return list;
}
/* non-transactional */
inline QString & operator<<= (QString & str, const WareTagSet & wareTags)
{
	str.clear();
	for(auto & wareTag : wareTags){
		if(str.length())
			str.append(", ");
		str.append(wareTag.tag);
	}
	return str;
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
inline bool operator!=(const WareTagSet & a, const StringSet & b)
{
	return !(a == b);
}


@declare@ Ware
class Ware
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h

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
@include@ dataclass_set.h


@declare@ Company
class Company
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Brand
class Brand
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Inventory
class Inventory
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Partner
class Partner
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Account
class Account
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Payment
class Payment
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ Item
class Item
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h


@declare@ QueryWithTag
class QueryWithTag
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
inline bool operator<(const Text & a, const QueryWithTag & b) { return b.tag.isMore(a); }
inline bool operator<(const QueryWithTag & a, const Text & b) { return a.tag.isLess(b); }
@include@ dataclass_set.h
/* non-transactional */
inline QString & operator<<= (QString & str, const QueryWithTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}
/* non-transactional */
inline StringSet & operator<<= (StringSet & list, const QueryWithTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}
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
inline bool operator!=(const QueryWithTagSet & a, const StringSet & b)
{
	return !(a == b);
}

@declare@ QueryWithoutTag
class QueryWithoutTag
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
inline bool operator<(const Text & a, const QueryWithoutTag & b) { return b.tag.isMore(a); }
inline bool operator<(const QueryWithoutTag & a, const Text & b) { return a.tag.isLess(b); }
@include@ dataclass_set.h
/* non-transactional */
inline QString & operator<<= (QString & str, const QueryWithoutTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}
/* non-transactional */
inline StringSet & operator<<= (StringSet & list, const QueryWithoutTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}
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
inline bool operator!=(const QueryWithoutTagSet & a, const StringSet & b)
{
	return !(a == b);
}


@declare@ QueryWare
class QueryWare
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h
/* non-transactional */
inline QString & operator<<= (QString & str, const QueryWareSet & wares)
{
	str.clear();
	for(auto & ware : wares){
		if(str.size())
			str.append(", ");
		str.append(ware.ware);
	}
	return str;
}


@declare@ QueryPartner
class QueryPartner
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h
};
@include@ dataclass_nonmembers.h
@include@ dataclass_set.h
/* non-transactional */
inline QString & operator<<= (QString & str, const QueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}


@declare@ Query
class Query
{
	@include@ dataclass_members.h
	@include@ dataclass_db.h

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
@include@ dataclass_set.h


#endif
