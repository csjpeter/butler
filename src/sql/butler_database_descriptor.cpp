/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_database_descriptor.h"

DatabaseDescriptor::DatabaseDescriptor() :
	name(""),
	driver(""),
	databaseName(""),
	username(""),
	password(""),
	savePassword(false),
	host(""),
	port(0)
{
}

DatabaseDescriptor::DatabaseDescriptor(const DatabaseDescriptor & orig)
{
	copy(orig);
}

DatabaseDescriptor::~DatabaseDescriptor()
{
}

DatabaseDescriptor& DatabaseDescriptor::operator=(const DatabaseDescriptor & orig)
{
	copy(orig);
	return *this;
}

void DatabaseDescriptor::copy(const DatabaseDescriptor & orig)
{
	name = orig.name;
	driver = orig.driver;
	databaseName = orig.databaseName;
	username = orig.username;
	password = orig.password;
	savePassword = orig.savePassword;
	host = orig.host;
	port = orig.port;
}

bool DatabaseDescriptor::isEqual(const DatabaseDescriptor &dd) const
{
	return (name == dd.name) &&
		(databaseName == dd.databaseName) &&
		(driver == dd.driver) &&
		(username == dd.username) &&
		(password == dd.password) &&
		(savePassword == dd.savePassword) &&
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
	savePassword = dd.savePassword;
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
