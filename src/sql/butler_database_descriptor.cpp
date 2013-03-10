/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_database_descriptor.h"

DatabaseDescriptor::DatabaseDescriptor() :
	name(""),
	databaseName(""),
	driver(""),
	username(""),
	password(""),
	host(""),
	port(0)
{
}

DatabaseDescriptor::~DatabaseDescriptor()
{
}
/*
void DatabaseDescriptor::copy(const DatabaseDescriptor & orig)
{
	name = orig.name;
	databaseName = orig.databaseName;
	driver = orig.driver;
	username = orig.username;
	password = orig.password;
	host = orig.host;
	port = orig.port;
}
*/
bool DatabaseDescriptor::isEqual(const DatabaseDescriptor &dd) const
{
	return (name == dd.name) &&
		(databaseName == dd.databaseName) &&
		(driver == dd.driver) &&
		(username == dd.username) &&
		(password == dd.password) &&
		(host == dd.host) &&
		(port == dd.port);
}
		
bool DatabaseDescriptor::isLess(const DatabaseDescriptor &dd) const
{
	return QString::localeAwareCompare(name, dd.name) < 0;
}

bool DatabaseDescriptor::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool DatabaseDescriptor::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void DatabaseDescriptor::equal(const DatabaseDescriptor &dd)
{
	name = dd.name;
	databaseName = dd.databaseName;
	driver = dd.driver;
	username = dd.username;
	password = dd.password;
	host = dd.host;
	port = dd.port;
}

bool operator==(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return a.isEqual(b);
}

bool operator!=(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return !a.isEqual(b);
}

bool operator<(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const DatabaseDescriptor &b)
{
	return b.isMore(a);
}

bool operator<(const DatabaseDescriptor &a, const QString &b)
{
	return a.isLess(b);
}
