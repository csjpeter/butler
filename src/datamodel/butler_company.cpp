/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_company.h"

Company::Company() :
	name(""),
	country(""),
	city(""),
	postalCode(""),
	address(""),
	taxId("")
{
}

Company::Company(const QString &_name) :
	name(_name),
	country(""),
	postalCode(""),
	address(""),
	taxId("")
{
}

Company::Company(const Company &s)
{
	copy(s);
}

Company::~Company()
{
}

Company& Company::operator=(const Company &s)
{
	copy(s);
	return *this;
}

bool Company::isEqual(const Company &s) const
{
	if(		name != s.name ||
			country != s.country ||
			city != s.city ||
			postalCode != s.postalCode ||
			address != s.address ||
			taxId != s.taxId)
		return false;
	return true;
}

bool Company::isLess(const Company &s) const
{
	return QString::localeAwareCompare(name, s.name) < 0;
}

bool Company::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Company::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void Company::copy(const Company &s)
{
	name = s.name;
	country = s.country;
	city = s.city;
	postalCode = s.postalCode;
	address = s.address;
	taxId = s.taxId;
}

bool operator==(const Company &a, const Company &b)
{
	return a.isEqual(b);
}

bool operator!=(const Company &a, const Company &b)
{
	return !a.isEqual(b);
}

bool operator<(const Company &a, const Company &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Company &b)
{
	return b.isMore(a);
}

bool operator<(const Company &a, const QString &b)
{
	return a.isLess(b);
}
