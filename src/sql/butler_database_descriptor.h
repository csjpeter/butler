/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASE_DESCRIPTOR_H
#define BUTLER_DATABASE_DESCRIPTOR_H

//#include <float.h>

#include <QString>

#include <csjp_sorter_owner_container.h>
#include <csjp_exception.h>

#include <butler_item.h>

class DatabaseDescriptor
{
public:
	QString name; // will be the connection name
	QString driver; // for example "QSQLITE"
	QString databaseName; // file name in case of sqlite
	QString username;
	QString password;
	bool savePassword;
	QString host; // domain name or ip
	unsigned port;

	enum Fields {
		Name = 0,
		Driver,
		DatabaseName,
		Username,
		Password,
		SavePassword,
		Host,
		Port,
		NumOfFields
	};

public:
	DatabaseDescriptor() :
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

	DatabaseDescriptor(const DatabaseDescriptor & orig)
	{
		copy(orig);
	}

	~DatabaseDescriptor()
	{
	}

	DatabaseDescriptor& operator=(const DatabaseDescriptor & orig)
	{
		copy(orig);
		return *this;
	}

	void copy(const DatabaseDescriptor & orig)
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

	bool isEqual(const DatabaseDescriptor &dd) const
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

	bool isLess(const DatabaseDescriptor &dd) const
	{
		return QString::localeAwareCompare(name, dd.name) < 0;
	}

	bool isLess(const QString &s) const
	{
		return QString::localeAwareCompare(name, s) < 0;
	}

	bool isMore(const QString &s) const
	{
		return 0 < QString::localeAwareCompare(name, s);
	}

private:
	void equal(const DatabaseDescriptor &dd)
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
};

inline bool operator==(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return a.isEqual(b);
}

inline bool operator!=(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return !a.isEqual(b);
}

inline bool operator<(const DatabaseDescriptor &a, const DatabaseDescriptor &b)
{
	return a.isLess(b);
}

inline bool operator<(const QString &a, const DatabaseDescriptor &b)
{
	return b.isMore(a);
}

inline bool operator<(const DatabaseDescriptor &a, const QString &b)
{
	return a.isLess(b);
}

class DatabaseDescriptorSet : public csjp::SorterOwnerContainer<DatabaseDescriptor>
{
public:
	DatabaseDescriptor::Fields ordering;
	bool ascending;
public:
	DatabaseDescriptorSet() :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(),
		ordering(DatabaseDescriptor::Name),
		ascending(true){}
	DatabaseDescriptorSet(const DatabaseDescriptorSet &is) :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(is),
		ordering(DatabaseDescriptor::Name),
		ascending(true){}
	~DatabaseDescriptorSet() {}

	DatabaseDescriptor& query(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::query<QString>(str);}

	bool has(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::has<QString>(str);}

	unsigned index(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::index<QString>(str);}

	virtual int compare(const DatabaseDescriptor &a, const DatabaseDescriptor &b) const
	{
		bool ret;

		switch(ordering) {
			case DatabaseDescriptor::Name :
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
			case DatabaseDescriptor::Driver :
				ret = QString::localeAwareCompare(a.driver, b.driver) < 0;
				break;
			case DatabaseDescriptor::DatabaseName :
				ret = a.databaseName < b.databaseName;
				break;
			case DatabaseDescriptor::Username :
				ret = QString::localeAwareCompare(a.username, b.username) < 0;
				break;
			case DatabaseDescriptor::Password :
				ret = a.password < b.password;
				break;
			case DatabaseDescriptor::Host :
				ret = a.host < b.host;
				break;
			case DatabaseDescriptor::Port :
				ret = a.port < b.port;
				break;
			default:
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
		}

		DBG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}
};

#endif
