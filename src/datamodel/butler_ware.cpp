/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware.h"

Ware::Ware() :
	name(""),
	lastModified(QDate(0,0,0), QTime(0,0,0)),
	unit("")
{
}

Ware::Ware(const QString &_name) :
	name(_name),
	lastModified(QDate(0,0,0), QTime(0,0,0)),
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

bool Ware::isEqual(const Ware &w) const
{
	if(		name != w.name ||
			lastModified.toString() != w.lastModified.toString() ||
			unit != w.unit ||
			categories != w.categories ||
			tags != w.tags)
		return false;
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

void Ware::copy(const Ware &w)
{
	name = w.name;
	lastModified = w.lastModified;
	unit = w.unit;
	categories.copy(w.categories);
	tags.copy(w.tags);
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
