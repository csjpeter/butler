/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_H
#define BUTLER_TAG_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_macros.h>

class Tag
{
public:
	Text name;
	DateTime lastModified; /* non editable */
	Text description;

	enum Fields {
		Name = 0,
		Description,
		LastModified,
		NumOfFields
	};

public:
	Tag() {}

	explicit Tag(const Text & _name) :
		name(_name)
	{
	}

	explicit Tag(const Tag & tag)
	{
		copy(tag);
	}

	~Tag() {}

	Tag & operator=(const Tag & tag)
	{
		copy(tag);
		return *this;
	}

	bool isEqual(const Tag & t) const
	{
		if(		name != t.name ||
				lastModified != t.lastModified ||
				description != t.description)
			return false;
		return true;
	}

	bool isLess(const Tag & t) const
	{
		return name < t.name;
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
	void copy(const Tag & tag)
	{
		name = tag.name;
		lastModified = tag.lastModified;
		description = tag.description;
	}
};

inline bool operator==(const Tag & a, const Tag & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Tag & a, const Tag & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Tag & a, const Tag & b)
{
	return a.isLess(b);
}

inline bool operator<(const Text & a, const Tag & b)
{
	return b.isMore(a);
}

inline bool operator<(const Tag & a, const Text & b)
{
	return a.isLess(b);
}

class TagSet : public csjp::SorterOwnerContainer<Tag>
{
public:
	Tag::Fields ordering;
	bool ascending;

	virtual int compare(const Tag & a, const Tag & b) const
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
	TagSet(const TagSet & ts) :
		csjp::SorterOwnerContainer<Tag>(ts),
		ordering(Tag::Name),
		ascending(true){}
	~TagSet() {}

};

typedef csjp::OwnerContainer<Text> TagNameSet;

#endif
