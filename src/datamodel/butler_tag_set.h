/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_SET_H
#define BUTLER_TAG_SET_H

#include <QString>

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_macros.h>
#include <butler_tag.h>

class TagSet : public csjp::SorterOwnerContainer<Tag>
{
public:
	TagSet() :
		csjp::SorterOwnerContainer<Tag>(),
		ordering(Tag::Name),
		ascending(true){}
	TagSet(TagSet &ts) :
		csjp::SorterOwnerContainer<Tag>(ts),
		ordering(Tag::Name),
		ascending(true){}
	~TagSet() {}

	Tag& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Tag>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Tag>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Tag>::index<QString>(name);}

	virtual int compare(const Tag &a, const Tag &b) const;

	Tag::Fields ordering;
	bool ascending;
};

/*
class TagDelegate : public BTreeToCStrIface<CustomBox<Tag> >
{
	public:
	const char *toCStr(const CustomBox<Tag> &box) const
	{
		static char address[256];
		snprintf(address, sizeof(address), "%s - %s",
				C_STR(box.data->name),
				C_STR(box.data->description));
		return address;
	}
};
*/

class TagNameSet : public csjp::OwnerContainer<QString>
{
public:
	TagNameSet() : csjp::OwnerContainer<QString>() {}
	TagNameSet(TagNameSet &tns) : csjp::OwnerContainer<QString>(tns) {}
	~TagNameSet() {}
};

#endif

