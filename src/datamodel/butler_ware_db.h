/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_DB_H
#define BUTLER_WARE_DB_H

#include <butler_ware_set.h>

class WareDb
{
public:
	/* Add new tag to be used. A tag not existing in tag database,
	 * should not be in use in other database parts. */
	virtual bool insert(const Ware &) = 0;

	/* A tag changed it's name should result in update of all the
	 * other database parts having the old value. */
	virtual bool update(
			const Ware &orig,
			const Ware &modified) = 0;

	/* Deletion of tag should fail if it is referenced in any other
	 * parts of the database. */
	virtual bool del(const Ware &) = 0;

	/* A set of all available tags. */
	virtual bool query(WareSet &) = 0;
};

#endif


