/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASE_DESCRIPTOR_H
#define BUTLER_DATABASE_DESCRIPTOR_H

#include <QString>

#include <csjp_exception.h>

class DatabaseDescriptor
{
public:
	DatabaseDescriptor();
	explicit DatabaseDescriptor(const DatabaseDescriptor &);
	~DatabaseDescriptor();

	DatabaseDescriptor& operator=(const DatabaseDescriptor &);

	void copy(const DatabaseDescriptor & orig);

	bool isEqual(const DatabaseDescriptor &t) const;

	bool isLess(const DatabaseDescriptor &t) const;
	
	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

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

private:
	void equal(const DatabaseDescriptor &tag);
};

bool operator==(const DatabaseDescriptor &a, const DatabaseDescriptor &b);

bool operator!=(const DatabaseDescriptor &a, const DatabaseDescriptor &b);

bool operator<(const DatabaseDescriptor &a, const DatabaseDescriptor &b);
bool operator<(const QString &a, const DatabaseDescriptor &b);
bool operator<(const DatabaseDescriptor &a, const QString &b);

#endif
