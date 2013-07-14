/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_partner.h"

Partner::Partner() :
	name(""),
	lastModified(QDate(0,0,0), QTime(0,0,0)),
	country(""),
	city(""),
	postalCode(""),
	address(""),
	company(""),
	storeName("")
{
}

Partner::Partner(const QString &_name) :
	name(_name),
	lastModified(QDate(0,0,0), QTime(0,0,0)),
	country(""),
	city(""),
	postalCode(""),
	address(""),
	company(""),
	storeName("")
{
}

Partner::Partner(const Partner &s)
{
	copy(s);
}

Partner::~Partner()
{
}

Partner& Partner::operator=(const Partner &s)
{
	copy(s);
	return *this;
}

bool Partner::isEqual(const Partner &s) const
{
	if(		name != s.name ||
			lastModified.toString() != s.lastModified.toString() ||
			country != s.country ||
			city != s.city ||
			postalCode != s.postalCode ||
			address != s.address ||
			company != s.company ||
			storeName != s.storeName)
		return false;
	return true;
}

bool Partner::isLess(const Partner &s) const
{
	return QString::localeAwareCompare(name, s.name) < 0;
}

bool Partner::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Partner::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void Partner::copy(const Partner &s)
{
	name = s.name;
	lastModified = s.lastModified;
	country = s.country;
	city = s.city;
	postalCode = s.postalCode;
	address = s.address;
	company = s.company;
	storeName = s.storeName;
}

bool operator==(const Partner &a, const Partner &b)
{
	return a.isEqual(b);
}

bool operator!=(const Partner &a, const Partner &b)
{
	return !a.isEqual(b);
}

bool operator<(const Partner &a, const Partner &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Partner &b)
{
	return b.isMore(a);
}

bool operator<(const Partner &a, const QString &b)
{
	return a.isLess(b);
}
