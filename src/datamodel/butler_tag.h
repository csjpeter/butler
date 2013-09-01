/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_H
#define BUTLER_TAG_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <csjp_datetime.h>
#include <csjp_text.h>

#include <butler_macros.h>

class Tag
{
public:
	csjp::Text name;
	csjp::DateTime lastModified; /* non editable */
	csjp::Text description;

	enum Fields {
		Name = 0,
		Description,
		LastModified,
		NumOfFields
	};

public:
	Tag() :
		name(""),
		lastModified(),
		description("")
	{
	}

	explicit Tag(const csjp::Text &_name) :
		lastModified(),
		description("")
	{
		name = _name;
	}

	explicit Tag(const Tag &tag)
	{
		equal(tag);
	}

	~Tag()
	{
	}

	Tag& operator=(const Tag& tag)
	{
		equal(tag);
		return *this;
	}

	bool isEqual(const Tag &t) const
	{
		if(		name != t.name ||
				lastModified != t.lastModified ||
				description != t.description)
			return false;
		return true;
	}

	bool isLess(const Tag &t) const
	{
		return name < t.name;
	}

	bool isLess(const csjp::Text &s) const
	{
		return name < s;
	}

	bool isMore(const csjp::Text &s) const
	{
		return s < name;
	}

private:
	void equal(const Tag &tag)
	{
		name = tag.name;
		lastModified = tag.lastModified;
		description = tag.description;
	}
};

inline bool operator==(const Tag &a, const Tag &b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Tag &a, const Tag &b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Tag &a, const Tag &b)
{
	return a.isLess(b);
}

inline bool operator<(const csjp::Text &a, const Tag &b)
{
	return b.isMore(a);
}

inline bool operator<(const Tag &a, const csjp::Text &b)
{
	return a.isLess(b);
}

class TagSet : public csjp::SorterOwnerContainer<Tag>
{
public:
	Tag::Fields ordering;
	bool ascending;

	int compare(const Tag &a, const Tag &b) const
	{
		bool ret;

		switch(ordering) {
			case Tag::Name :
				ret = a.name < b.name;
				break;
			case Tag::Description :
				ret = a.description < b.description;
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

public:
	TagSet() :
		csjp::SorterOwnerContainer<Tag>(),
		ordering(Tag::Name),
		ascending(true){}
	TagSet(const TagSet &ts) :
		csjp::SorterOwnerContainer<Tag>(ts),
		ordering(Tag::Name),
		ascending(true){}
	~TagSet() {}

	Tag& query(const csjp::Text & name) const {
		return csjp::SorterOwnerContainer<Tag>::query<csjp::Text>(name);}

	bool has(const csjp::Text & name) const {
		return csjp::SorterOwnerContainer<Tag>::has<csjp::Text>(name);}

	unsigned index(const csjp::Text & name) const {
		return csjp::SorterOwnerContainer<Tag>::index<csjp::Text>(name);}
};

typedef csjp::OwnerContainer<csjp::Text> TagNameSet;

#endif
