/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_H
#define BUTLER_WARE_H

#include <QString>

#include <csjp_sorter_owner_container.h>

#include <butler_tag_set.h>

class CategoryNameSet : public csjp::SorterOwnerContainer<QString>
{
public:
	CategoryNameSet() : csjp::SorterOwnerContainer<QString>(), ascending(true){}
	CategoryNameSet(CategoryNameSet &cns) :
		csjp::SorterOwnerContainer<QString>(cns),
		ascending(cns.ascending) {}
	~CategoryNameSet() {}
	int compare(const QString &a, const QString &b) const
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
	Ware();
	explicit Ware(const QString &);
	explicit Ware(const Ware &);
	~Ware();
	
	Ware& operator=(const Ware &);

	bool isEqual(const Ware &) const;

	bool isLess(const Ware &) const;
	
	bool isLess(const QString &) const;
	bool isMore(const QString &) const;

public:
	QString name;
	QString unit;
	CategoryNameSet categories;
	TagNameSet tags;
	
	enum Fields {
		Name = 0,
		Unit,
		Tags,
		Categories,
		NumOfFields
	};

private:
	void copy(const Ware &);
};

bool operator==(const Ware &a, const Ware &b);
bool operator!=(const Ware &a, const Ware &b);

bool operator<(const Ware &a, const Ware &b);
bool operator<(const Ware &a, const QString &b);
bool operator<(const QString &a, const Ware &b);

#endif
