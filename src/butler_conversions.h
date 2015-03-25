/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_CONVERSIONS_H
#define BUTLER_CONVERSIONS_H

#include <csjp_exception.h>
#include <csjp_string.h>
#include <butler_types.h>
#include <butler_text.h>
#include <butler_datetime.h>
#include <butler_dataclasses.h>

#include <QString>
#include <QVariant>

inline QVariant & operator<<= (QVariant & lhs, const enum SqlDriver & rhs)
{
	switch(rhs)
	{
		case SqlDriver::PSql :
			lhs = "PSql";
			break;
		case SqlDriver::SQLite :
			lhs = "SQLite";
			break;
		case SqlDriver::MySQL :
			lhs = "MySQL";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum SqlDriver & operator<<= (enum SqlDriver & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "PSql")
				lhs = SqlDriver::PSql;
		else if(s == "SQLite")
				lhs = SqlDriver::SQLite;
		else if(s == "MySQL")
				lhs = SqlDriver::MySQL;
		else
			throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline QString & operator<<= (QString & qstr, const enum QueryStockOptions & rhs)
{
	switch(rhs)
	{
		case QueryStockOptions::AllItemChanges :
			qstr = "AllItemChanges";
			break;
		case QueryStockOptions::Gains :
			qstr = "Gains";
			break;
		case QueryStockOptions::Looses :
			qstr = "Looses";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return qstr;
}

inline enum QueryStockOptions & operator<<= (enum QueryStockOptions & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "AllItemChanges")
				lhs = QueryStockOptions::AllItemChanges;
		else if(s == "Gains")
				lhs = QueryStockOptions::Gains;
		else if(s == "Looses")
				lhs = QueryStockOptions::Looses;
		else
			throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline QString & operator<<= (QString & qstr, const enum QueryTagOptions & rhs)
{
	switch(rhs)
	{
		case QueryTagOptions::MatchAll :
			qstr = "MatchAll";
			break;
		case QueryTagOptions::MatchAny :
			qstr = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return qstr;
}

inline enum QueryTagOptions & operator<<= (enum QueryTagOptions & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "MatchAll")
				lhs = QueryTagOptions::MatchAll;
		else if(s == "MatchAny")
				lhs = QueryTagOptions::MatchAny;
		else
			throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline csjp::String & operator<<= (csjp::String & str, const QString & qstr)
{
	str = C_STR(qstr); return str;
}

inline csjp::String & operator<<= (csjp::String & str, unsigned u)
{
	str.cutAt(0); str << u; return str;
}

inline QString & operator<<= (QString & qstr, const csjp::String & str)
{
	qstr = str.c_str(); return qstr;
}

inline QVariant & operator<<= (QVariant & var, const csjp::String & rhs)
{
	QVariant v(QString(rhs.str));
	var = v;
	return var;
}

inline QVariant & operator<<= (QVariant & var, const bool & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const int & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const unsigned & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const double & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const QString & rhs)
{
	var.setValue(rhs);
	return var;
}

inline QVariant & operator<<= (QVariant & var, const QDateTime & rhs)
{
	var.setValue(rhs);
	return var;
}

inline csjp::String & operator<<= (csjp::String & str, const QVariant & v)
{
	str = C_STR(v.toString()); return str;
}

inline QDateTime & operator<<= (QDateTime & lhs, const QVariant & v)
{
	lhs = v.toDateTime(); return lhs;
}

inline QString & operator<<= (QString & qstr, const QVariant & v)
{
	qstr = v.toString(); return qstr;
}

inline DateTime & operator<<= (DateTime & time, const QVariant & v)
{
	time = v.toDateTime();
	return time;
}

inline int & operator<<= (int & i, const QVariant & v)
{
	i = v.toInt(); return i;
}

inline unsigned & operator<<= (unsigned & u, const QVariant & v)
{
	u = v.toUInt(); return u;
}

inline double & operator<<= (double & d, const QVariant & v)
{
	d = v.toDouble(); return d;
}

inline bool & operator<<= (bool & b, const QVariant & v)
{
	b = v.toBool(); return b;
}

inline QStringList & operator<<= (QStringList & list, const WareTypeSet & wareTypes)
{
	list.clear();
	for(auto & wareType : wareTypes)
		list.append(wareType.type);
	return list;
}

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

inline StringSet & operator<<= (StringSet & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.add(new Text(wareTag.tag));
	return list;
}

inline QStringList & operator<<= (QStringList & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.append(wareTag.tag);
	return list;
}

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

inline StringSet & operator<<= (StringSet & list, const QueryWithTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

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

inline StringSet & operator<<= (StringSet & list, const QueryWithoutTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

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
