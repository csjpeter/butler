/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_DB_H
#define BUTLER_TAG_DB_H

#include <butler_tag_set.h>

class TagDb
{
public:
	/* Add new tag to be used. A tag not existing in tag database,
	 * should not be in use in other database parts. */
	virtual void insert(const Tag &) = 0;

	/* A tag changed it's name should result in update of all the
	 * other database parts having the old value. */
	virtual void update(
			const Tag &orig,
			const Tag &modified) = 0;

	/* Deletion of tag should fail if it is referenced in any other
	 * parts of the database. */
	virtual void del(const Tag &) = 0;

	/* A set of all available tags. */
	virtual void query(TagSet &) = 0;
};

#endif


