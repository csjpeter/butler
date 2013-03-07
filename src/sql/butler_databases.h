/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASE_DESCRIPTION_H
#define BUTLER_DATABASE_DESCRIPTION_H

#include <csjp_exception.h>

#include <butler_db.h>

extern csjp::OwnerContainer<DatabaseDescription> databases;

class DatabaseDescription
{
public:
	DatabaseDescription();
	explicit DatabaseDescription(const DatabaseDescription &) = delete;
	~DatabaseDescription();

	DatabaseDescription& operator=(const DatabaseDescription &) = delete;

	bool isEqual(const DatabaseDescription &t) const;

	bool isLess(const DatabaseDescription &t) const;
	
	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

	Db & db();

public:
	QString name; // will be the connection name
	QString databaseName; // file name in case of sqlite
	QString driver; // for example "QSQLITE"
	QString username;
	QString password;
	QString host; // domain name or ip
	unsigned port;

private:
	Db * database;

private:
	void equal(const DatabaseDescription &tag);
};

bool operator==(const DatabaseDescription &a, const DatabaseDescription &b);

bool operator!=(const DatabaseDescription &a, const DatabaseDescription &b);

bool operator<(const DatabaseDescription &a, const DatabaseDescription &b);
bool operator<(const QString &a, const DatabaseDescription &b);
bool operator<(const DatabaseDescription &a, const QString &b);

#endif
