/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_TYPES_H
#define BUTLER_TYPES_H

#include <csjp_str.h>
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



/*
inline bool operator<<=(bool lhs, const csjp::CString & rhs)
{
	csjp::Str s(rhs.ptr);
	lhs = false;
	if(s == "Y" && s == "true")
		lhs = true;
	return lhs;
}
*/

inline csjp::String & operator<<=(csjp::String & lhs, const bool rhs)
{
	if(rhs)
		lhs = "Y";
	else
		lhs = "N";
	return lhs;
}


class SqlDriver
{
	public:
		enum class Enum{
			SQLite,
#ifdef PGSQL
			PSql,
#endif
#ifdef MYSQL
			MySQL
#endif
		};
	private:
		Enum value;
	public:
		SqlDriver() : value((Enum)(0)) {}
		SqlDriver(SqlDriver::Enum value) : value(value) {}
		SqlDriver(const SqlDriver & orig) : value(orig.value) {}

		SqlDriver &operator = (const SqlDriver & rhs) { value = rhs.value; return *this; }
		operator enum Enum () const { return value; }
};

inline bool operator==(const SqlDriver & lhs, const char * rhs)
{
	switch(lhs)
	{
		case SqlDriver::Enum::SQLite :
			return csjp::CString(rhs) == "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::Enum::PSql :
			return csjp::CString(rhs) == "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::Enum::MySQL :
			return csjp::CString(rhs) == "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return false;
}

inline QVariant & operator<<=(QVariant & lhs, const SqlDriver & rhs)
{
	switch(rhs)
	{
		case SqlDriver::Enum::SQLite :
			lhs = "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::Enum::PSql :
			lhs = "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::Enum::MySQL :
			lhs = "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline SqlDriver & operator<<=(SqlDriver & lhs, const QVariant & rhs)
{
		QString s = rhs.toString();
		if(s == "SQLite")
				lhs = SqlDriver::Enum::SQLite;
#ifdef PGSQL
		else if(s == "PSql")
				lhs = SqlDriver::Enum::PSql;
#endif
#ifdef MYSQL
		else if(s == "MySQL")
				lhs = SqlDriver::Enum::MySQL;
#endif
		else
			throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const SqlDriver & rhs)
{
	switch(rhs)
	{
		case SqlDriver::Enum::SQLite :
			lhs = "SQLite";
			break;
#ifdef PGSQL
		case SqlDriver::Enum::PSql :
			lhs = "PSql";
			break;
#endif
#ifdef MYSQL
		case SqlDriver::Enum::MySQL :
			lhs = "MySQL";
			break;
#endif
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

enum class ItemQueryStockOptions
{
	AllItemChanges,
	Gains,
	Looses
};

inline csjp::String & operator<<=(csjp::String & lhs, const enum ItemQueryStockOptions & rhs)
{
	switch(rhs)
	{
		case ItemQueryStockOptions::AllItemChanges :
			lhs = "AllItemChanges";
			break;
		case ItemQueryStockOptions::Gains :
			lhs = "Gains";
			break;
		case ItemQueryStockOptions::Looses :
			lhs = "Looses";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline QString & operator<<=(QString & lhs, const enum ItemQueryStockOptions & rhs)
{
	switch(rhs)
	{
		case ItemQueryStockOptions::AllItemChanges :
			lhs = "AllItemChanges";
			break;
		case ItemQueryStockOptions::Gains :
			lhs = "Gains";
			break;
		case ItemQueryStockOptions::Looses :
			lhs = "Looses";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum ItemQueryStockOptions & operator<<=(enum ItemQueryStockOptions & lhs, const QVariant & rhs)
{
	QString s = rhs.toString();
	if(s == "AllItemChanges")
		lhs = ItemQueryStockOptions::AllItemChanges;
	else if(s == "Gains")
		lhs = ItemQueryStockOptions::Gains;
	else if(s == "Looses")
		lhs = ItemQueryStockOptions::Looses;
	else
		throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline enum ItemQueryStockOptions & operator<<=(enum ItemQueryStockOptions & lhs, const csjp::CString & rhs)
{
	csjp::Str s(rhs.ptr);
	if(s == "AllItemChanges")
		lhs = ItemQueryStockOptions::AllItemChanges;
	else if(s == "Gains")
		lhs = ItemQueryStockOptions::Gains;
	else if(s == "Looses")
		lhs = ItemQueryStockOptions::Looses;
	else
		throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

enum class ItemQueryTagOptions
{
	MatchAll,
	MatchAny
};

inline enum ItemQueryTagOptions & operator<<=(enum ItemQueryTagOptions & lhs, const csjp::CString & rhs)
{
	csjp::Str s(rhs.ptr);
	if(s == "MatchAll")
		lhs = ItemQueryTagOptions::MatchAll;
	else if(s == "MatchAny")
		lhs = ItemQueryTagOptions::MatchAny;
	else
		throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline QString & operator<<=(QString & lhs, const enum ItemQueryTagOptions & rhs)
{
	switch(rhs)
	{
		case ItemQueryTagOptions::MatchAll :
			lhs = "MatchAll";
			break;
		case ItemQueryTagOptions::MatchAny :
			lhs = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const enum ItemQueryTagOptions & rhs)
{
	switch(rhs)
	{
		case ItemQueryTagOptions::MatchAll :
			lhs = "MatchAll";
			break;
		case ItemQueryTagOptions::MatchAny :
			lhs = "MatchAny";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return lhs;
}

inline enum ItemQueryTagOptions & operator<<=(enum ItemQueryTagOptions & lhs, const QVariant & rhs)
{
	QString s = rhs.toString();
	if(s == "MatchAll")
		lhs = ItemQueryTagOptions::MatchAll;
	else if(s == "MatchAny")
		lhs = ItemQueryTagOptions::MatchAny;
	else
		throw csjp::InvalidArgument(EXCLI);
	return lhs;
}

inline csjp::String & operator<<=(csjp::String & lhs, const csjp::String & rhs)
				{ lhs.cutAt(0); lhs << rhs; return lhs; }
inline csjp::String & operator<<=(csjp::String & lhs, const char * rhs)
				{ lhs.cutAt(0); lhs << rhs; return lhs; }

#endif
