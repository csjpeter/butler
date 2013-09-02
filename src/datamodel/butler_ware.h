/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_H
#define BUTLER_WARE_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <csjp_datetime.h>
#include <csjp_text.h>

#include <QString>

#include <butler_tag.h>

class CategoryNameSet : public csjp::SorterOwnerContainer<QString>
{
public:
	CategoryNameSet() : csjp::SorterOwnerContainer<QString>(), ascending(true){}
	CategoryNameSet(CategoryNameSet & cns) :
		csjp::SorterOwnerContainer<QString>(cns),
		ascending(cns.ascending) {}
	~CategoryNameSet() {}
	int compare(const QString & a, const QString & b) const
	{
		bool ret = QString::localeAwareCompare(a, b) < 0;
		if(!ascending)
			ret = !ret;
		return ret ? -1 : 1;
	}
	bool ascending;
};

class Ware
{
public:
	QString name;
	csjp::DateTime lastModified; /* non editable */
	QString unit;
	CategoryNameSet categories;
	TagNameSet tags;

	enum Fields {
		Name = 0,
		Unit,
		Tags,
		Categories,
		LastModified,
		NumOfFields
	};

public:
	Ware() {}

	explicit Ware(const QString & _name) :
		name(_name)
	{
	}

	explicit Ware(const Ware & i)
	{
		copy(i);
	}

	~Ware()
	{
	}

	Ware& operator=(const Ware & i)
	{
		copy(i);
		return *this;
	}

	bool isEqual(const Ware & w) const
	{
		if(		name != w.name ||
				lastModified != w.lastModified ||
				unit != w.unit ||
				categories != w.categories ||
				tags != w.tags)
			return false;
		return true;
	}

	bool isLess(const Ware & i) const
	{
		return QString::localeAwareCompare(name, i.name) < 0;
	}

	bool isLess(const QString & s) const
	{
		return QString::localeAwareCompare(name, s) < 0;
	}

	bool isMore(const QString & s) const
	{
		return 0 < QString::localeAwareCompare(name, s);
	}

private:
	void copy(const Ware & w)
	{
		name = w.name;
		lastModified = w.lastModified;
		unit = w.unit;
		categories.copy(w.categories);
		tags.copy(w.tags);
	}
};

inline bool operator==(const Ware & a, const Ware & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Ware & a, const Ware & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Ware & a, const Ware & b)
{
	return a.isLess(b);
}

inline bool operator<(const QString & a, const Ware & b)
{
	return b.isMore(a);
}

inline bool operator<(const Ware & a, const QString & b)
{
	return a.isLess(b);
}

class WareSet : public csjp::SorterOwnerContainer<Ware>
{
public:
	Ware::Fields ordering;
	bool ascending;
public:
	WareSet() :
		csjp::SorterOwnerContainer<Ware>(),
		ordering(Ware::Name),
		ascending(true){}
	WareSet(const WareSet & ws) :
		csjp::SorterOwnerContainer<Ware>(ws),
		ordering(Ware::Name),
		ascending(true){}
	~WareSet() {}

	Ware& query(const QString & name) const {
		return csjp::SorterOwnerContainer<Ware>::query<QString>(name);}

	bool has(const QString & name) const {
		return csjp::SorterOwnerContainer<Ware>::has<QString>(name);}

	unsigned index(const QString & name) const {
		return csjp::SorterOwnerContainer<Ware>::index<QString>(name);}

	virtual int compare(const Ware & a, const Ware & b) const
	{
		bool ret;

		switch(ordering) {
			case Ware::Name :
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
			case Ware::Unit :
				ret = QString::localeAwareCompare(a.unit, b.unit) < 0;
				break;
			case Ware::Categories :
				ret = a.categories.size() < b.categories.size();
				break;
			case Ware::Tags :
				ret = a.tags.size() < b.tags.size();
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

typedef csjp::OwnerContainer<QString> WareNameSet;

#endif
