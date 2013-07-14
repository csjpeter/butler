/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANY_H
#define BUTLER_COMPANY_H

#include <QDate>
#include <QString>

#include <butler_tag_set.h>

class Tag;

class Company
{
public:
	Company();
	explicit Company(const QString &_name);
	explicit Company(const Company &);
	~Company();
	
	Company& operator=(const Company &);

	bool isEqual(const Company &) const;

	bool isLess(const Company &) const;

	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

public:
	QString name;		/* Tesco Global Áruházak ZRt. */
	QDateTime lastModified; /* non editable */
	QString country;	/* Magyarország */
	QString city;		/* Budaörs */
	QString postalCode;	/* 2040 */
	QString address;	/* Kinizsi út 1-3. */
	QString taxId;		/* 10307078-2-44 */
	QString icon;		/* base64 representation of an image */
	
	enum Fields {
		Name = 0,
		Country,
		City,
		PostalCode,
		Address,
		TaxId,
		Icon,
		LastModified,
		NumOfFields
	};

private:
	void copy(const Company &);
};

bool operator==(const Company &a, const Company &b);
bool operator!=(const Company &a, const Company &b);

bool operator<(const Company &a, const Company &b);
bool operator<(const QString &a, const Company &b);
bool operator<(const Company &a, const QString &b);

#endif
