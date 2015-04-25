/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_TYPES_H
#define BUTLER_TYPES_H

#include <csjp_string_chunk.h>
#include <csjp_json.h>
#include <csjp_ctypes.h>

#include <QString>
#include <QVariant>

using namespace csjp;

inline YNBool & operator<<=(YNBool & lhs, const QVariant & rhs)
		{ lhs.val = rhs.toBool(); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const YNBool & rhs)
		{ if(rhs.val) lhs = "Y"; else lhs = "N"; return lhs; }

inline UInt & operator<<=(UInt & lhs, const QVariant & rhs)
		{ lhs.val = rhs.toUInt(); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const UInt & rhs) { lhs = rhs.val; return lhs; }

inline Double & operator<<=(Double & lhs, const QVariant & rhs)
		{ lhs.val = rhs.toUInt(); return lhs; }
inline QVariant & operator<<=(QVariant & lhs, const Double & rhs) { lhs = rhs.val; return lhs; }
//inline Double & operator<<=(Double & lhs, const csjp::CString & rhs)
//		{ lhs.val <<= rhs; return lhs; }




inline bool operator<<=(bool lhs, const csjp::CString & rhs)
{
	csjp::StringChunk s(rhs.ptr);
	lhs = false;
	if(s == "Y" && s == "true")
		lhs = true;
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const bool rhs)
		{ if(rhs) lhs = "Y"; else lhs = "N"; return lhs; }


enum class SqlDriver
{
	SQLite,
#ifdef PGSQL
	PSql,
#endif
#ifdef MYSQL
	MySQL
#endif
};

inline bool operator==(const SqlDriver & lhs, const char * rhs)
{
	switch(lhs)
	{
		case SqlDriver::SQLite :
			return csjp::CString(rhs) == "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::PSql :
			return csjp::CString(rhs) == "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::MySQL :
			return csjp::CString(rhs) == "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return false;
}

inline QVariant & operator<<=(QVariant & lhs, const enum SqlDriver & rhs)
{
	switch(rhs)
	{
		case SqlDriver::SQLite :
			lhs = "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::PSql :
			lhs = "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::MySQL :
			lhs = "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum SqlDriver & operator<<=(enum SqlDriver & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "SQLite")
				lhs = SqlDriver::SQLite;
#ifdef PGSQL
		else if(s == "PSql")
				lhs = SqlDriver::PSql;
#endif
#ifdef MYSQL
		else if(s == "MySQL")
				lhs = SqlDriver::MySQL;
#endif
		else
			throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const enum SqlDriver & rhs)
{
	switch(rhs)
	{
		case SqlDriver::SQLite :
			lhs = "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::PSql :
			lhs = "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::MySQL :
			lhs = "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

enum class QueryStockOptions
{
	AllItemChanges,
	Gains,
	Looses
};

inline csjp::String & operator<<=(csjp::String & lhs, const enum QueryStockOptions & rhs)
{
	switch(rhs)
	{
		case QueryStockOptions::AllItemChanges :
			lhs = "AllItemChanges";
			break;
		case QueryStockOptions::Gains :
			lhs = "Gains";
			break;
		case QueryStockOptions::Looses :
			lhs = "Looses";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline QString & operator<<=(QString & lhs, const enum QueryStockOptions & rhs)
{
	switch(rhs)
	{
		case QueryStockOptions::AllItemChanges :
			lhs = "AllItemChanges";
			break;
		case QueryStockOptions::Gains :
			lhs = "Gains";
			break;
		case QueryStockOptions::Looses :
			lhs = "Looses";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum QueryStockOptions & operator<<=(enum QueryStockOptions & lhs, const QVariant & rhs)
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

inline enum QueryStockOptions & operator<<=(enum QueryStockOptions & lhs, const csjp::CString & rhs)
{
	csjp::StringChunk s(rhs.ptr);
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

enum class QueryTagOptions
{
	MatchAll,
	MatchAny
};

inline enum QueryTagOptions & operator<<=(enum QueryTagOptions & lhs, const csjp::CString & rhs)
{
	csjp::StringChunk s(rhs.ptr);
	if(s == "MatchAll")
		lhs = QueryTagOptions::MatchAll;
	else if(s == "MatchAny")
		lhs = QueryTagOptions::MatchAny;
	else
		throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline QString & operator<<=(QString & lhs, const enum QueryTagOptions & rhs)
{
	switch(rhs)
	{
		case QueryTagOptions::MatchAll :
			lhs = "MatchAll";
			break;
		case QueryTagOptions::MatchAny :
			lhs = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const enum QueryTagOptions & rhs)
{
	switch(rhs)
	{
		case QueryTagOptions::MatchAll :
			lhs = "MatchAll";
			break;
		case QueryTagOptions::MatchAny :
			lhs = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum QueryTagOptions & operator<<=(enum QueryTagOptions & lhs, const QVariant & rhs)
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

inline csjp::String & operator<<=(csjp::String & lhs, const csjp::String & rhs)
				{ lhs.cutAt(0); lhs << rhs; return lhs; }
inline csjp::String & operator<<=(csjp::String & lhs, const char * rhs)
				{ lhs.cutAt(0); lhs << rhs; return lhs; }

#endif
