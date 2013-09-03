/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_H
#define BUTLER_WARE_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_tag.h>

class CategoryNameSet : public csjp::SorterOwnerContainer<Text>
{
public:
	CategoryNameSet() : csjp::SorterOwnerContainer<Text>(), ascending(true){}
	CategoryNameSet(CategoryNameSet & cns) :
		csjp::SorterOwnerContainer<Text>(cns),
		ascending(cns.ascending) {}
	~CategoryNameSet() {}
	int compare(const Text & a, const Text & b) const
	{
		bool ret = a < b;
		if(!ascending)
			ret = !ret;
		return ret ? -1 : 1;
	}
	bool ascending;
};

class Ware
{
public:
	Text name;
	DateTime lastModified; /* non editable */
	Text unit;
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

	explicit Ware(const Text & _name) :
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
		return name < i.name;
	}

	bool isLess(const Text & s) const
	{
		return name < s;
	}

	bool isMore(const Text & s) const
	{
		return s < name;
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

inline bool operator<(const Text & a, const Ware & b)
{
	return b.isMore(a);
}

inline bool operator<(const Ware & a, const Text & b)
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

	virtual int compare(const Ware & a, const Ware & b) const
	{
		bool ret;

		switch(ordering) {
			case Ware::Name :
				ret = a.name < b.name;
				break;
			case Ware::Unit :
				ret = a.unit < b.unit;
				break;
			case Ware::Categories :
				ret = a.categories.size() < b.categories.size();
				break;
			case Ware::Tags :
				ret = a.tags.size() < b.tags.size();
				break;
			default:
				ret = a.name < b.name;
				break;
		}

		DBG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}
};

typedef csjp::OwnerContainer<Text> WareNameSet;

#endif
