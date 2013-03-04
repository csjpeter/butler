/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware.h"

Ware::Ware() :
	name(""),
	unit("")
{
}

Ware::Ware(const QString &_name) :
	name(_name),
	unit("")
{
}

Ware::Ware(const Ware &i)
{
	copy(i);
}

Ware::~Ware()
{
}

Ware& Ware::operator=(const Ware &i)
{
	copy(i);
	return *this;
}

bool Ware::isEqual(const Ware &i) const
{
//	MSG("[%s] == [%s]", qPrintable(name), qPrintable(i.name));
	if(name != i.name) return false;

//	MSG("[%s] == [%s]", qPrintable(unit), qPrintable(i.unit));
	if(unit != i.unit) return false;

//	MSG("[%s] == [%s]", qPrintable(categories), qPrintable(i.categories));
	if(categories != i.categories) return false;

//	MSG("[%s] == [%s]", qPrintable(tags), qPrintable(i.tags));
	if(tags != i.tags) return false;

	return true;
}

bool Ware::isLess(const Ware &i) const
{
	return QString::localeAwareCompare(name, i.name) < 0;
}

bool Ware::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Ware::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void Ware::copy(const Ware &i)
{
	name = i.name;
	unit = i.unit;
	categories.copy(i.categories);
	tags.copy(i.tags);
}

bool operator==(const Ware &a, const Ware &b)
{
	return a.isEqual(b);
}

bool operator!=(const Ware &a, const Ware &b)
{
	return !a.isEqual(b);
}

bool operator<(const Ware &a, const Ware &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Ware &b)
{
	return b.isMore(a);
}

bool operator<(const Ware &a, const QString &b)
{
	return a.isLess(b);
}

