/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_TYPES_H
#define BUTLER_TYPES_H

#include <csjp_string.h>

enum class SqlDriver
{
	PSql,
	SQLite,
	MySQL
};

inline bool operator==(const SqlDriver & lhs, const char * rhs)
{
	switch(lhs)
	{
		case SqlDriver::PSql :
			return csjp::CString(rhs) == "PSql";
			break;
		case SqlDriver::SQLite :
			return csjp::CString(rhs) == "SQLite";
			break;
		case SqlDriver::MySQL :
			return csjp::CString(rhs) == "MySQL";
			break;
		default:
			throw csjp::InvalidArgument(EXCLI);
	}
	return false;
}

enum class QueryStockOptions
{
	AllItemChanges,
	Gains,
	Looses
};

enum class QueryTagOptions
{
	MatchAll,
	MatchAny
};

#endif
