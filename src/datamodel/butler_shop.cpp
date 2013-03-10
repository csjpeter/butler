/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_shop.h"

Shop::Shop() :
	name(""),
	storeName(""),
	city(""),
	address(""),
	company("")
{
}

Shop::Shop(const QString &_name) :
	name(_name),
	storeName(""),
	city(""),
	address(""),
	company("")
{
}

Shop::Shop(const Shop &s)
{
	copy(s);
}

Shop::~Shop()
{
}

Shop& Shop::operator=(const Shop &s)
{
	copy(s);
	return *this;
}

bool Shop::isEqual(const Shop &s) const
{
	if(		name != s.name ||
			storeName != s.storeName ||
			city != s.city ||
			address != s.address ||
			company != s.company)
		return false;
	return true;
}

bool Shop::isLess(const Shop &s) const
{
	return QString::localeAwareCompare(name, s.name) < 0;
}

bool Shop::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Shop::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void Shop::copy(const Shop &s)
{
	name = s.name;
	storeName = s.storeName;
	city = s.city;
	address = s.address;
	company = s.company;
}

bool operator==(const Shop &a, const Shop &b)
{
	return a.isEqual(b);
}

bool operator!=(const Shop &a, const Shop &b)
{
	return !a.isEqual(b);
}

bool operator<(const Shop &a, const Shop &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Shop &b)
{
	return b.isMore(a);
}

bool operator<(const Shop &a, const QString &b)
{
	return a.isLess(b);
}
