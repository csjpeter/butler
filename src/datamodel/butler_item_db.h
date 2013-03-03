/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_DB_H
#define BUTLER_ITEM_DB_H

#include <csjp_sorter_owner_container.h>
#include <butler_query.h>
#include <butler_item_set.h>
#include <butler_ware_set.h>

class ItemDb
{
public:
	/* The update field must have valid value and can not be
	 * changed in future. */
	virtual bool insert(const Item &) = 0;

	/* */
	virtual bool update(
			const Item &orig,
			const Item &modified) = 0;

	/* */
	virtual bool del(const Item &) = 0;

	/* items on shopping list having tags marked in tagset */
	virtual bool query(const TagNameSet &, ItemSet &) = 0;

	/* bought items filtered with query options */
	virtual bool query(const Query &, QueryStat &stat, ItemSet &) = 0;

//	virtual bool query(WareSet &s) = 0;
};

#endif

