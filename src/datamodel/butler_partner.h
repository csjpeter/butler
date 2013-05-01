/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_H
#define BUTLER_PARTNER_H

#include <QString>

#include <butler_tag_set.h>

class Tag;

class Partner
{
public:
	Partner();
	explicit Partner(const QString &_name);
	explicit Partner(const Partner &);
	~Partner();
	
	Partner& operator=(const Partner &);

	bool isEqual(const Partner &) const;

	bool isLess(const Partner &) const;

	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

public:
	QString name;		/* kertvárosi tesco */
	QString country;	/* Magyarország */
	QString city;		/* Pécs */
	QString postalCode;	/* 7631 */
	QString address;	/* Kincses u. 1. */
	QString company;	/* Tesco Global Áruházak ZRt. */
	QString storeName;	/* 41052 számú bolt */
	
	enum Fields {
		Name = 0,
		Country,
		City,
		PostalCode,
		Address,
		Company,
		StoreName,
		NumOfFields
	};

private:
	void copy(const Partner &);
};

bool operator==(const Partner &a, const Partner &b);
bool operator!=(const Partner &a, const Partner &b);

bool operator<(const Partner &a, const Partner &b);
bool operator<(const QString &a, const Partner &b);
bool operator<(const Partner &a, const QString &b);

#endif
