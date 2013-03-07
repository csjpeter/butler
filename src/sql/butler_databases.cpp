/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <butler_db.h>

#include "butler_database_description.h"

csjp::OwnerContainer<DatabaseDescription> databases;

DatabaseDescription::DatabaseDescription() :
	name(""),
	databaseName(""),
	driver(""),
	username(""),
	password(""),
	host(""),
	port(0),
	database(0)
{
}

DatabaseDescription::DatabaseDescription(const DatabaseDescription & dd)
{
	equal(dd);
}

DatabaseDescription::~DatabaseDescription()
{
	delete sql;
}

bool DatabaseDescription::isEqual(const DatabaseDescription &db) const
{
	return (name == db.name) &&
		(databaseName == db.databaseName) &&
		(driver == db.driver) &&
		(username == db.username) &&
		(password == db.password) &&
		(host == db.host) &&
		(port == db.port);
}
		
bool DatabaseDescription::isLess(const DatabaseDescription &db) const
{
	return QString::localeAwareCompare(name, db.name) < 0;
}

bool DatabaseDescription::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool DatabaseDescription::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

Db & DatabaseDescription::db()
{
	if(database)
		return *database;

	if(driver == "QSQLITE") {
		database = new Db(*this);
//		if(!QFile::exists(filepath))
	} else
		throw DbError("Driver '%s' is not yet supported.", C_STR(driver));
}
		
void DatabaseDescription::equal(const DatabaseDescription &dd)
{
	name == db.name;
	databaseName == db.databaseName;
	driver == db.driver;
	username == db.username;
	password == db.password;
	host == db.host;
	port == db.port;
}

bool operator==(const DatabaseDescription &a, const DatabaseDescription &b)
{
	return a.isEqual(b);
}

bool operator!=(const DatabaseDescription &a, const DatabaseDescription &b)
{
	return !a.isEqual(b);
}

bool operator<(const DatabaseDescription &a, const DatabaseDescription &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const DatabaseDescription &b)
{
	return b.isMore(a);
}

bool operator<(const DatabaseDescription &a, const QString &b)
{
	return a.isLess(b);
}
