/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOP_DB_H
#define BUTLER_SHOP_DB_H

#include <butler_shop_set.h>

class ShopDb
{
public:
	/* The update field must have valid value and can not be
	 * changed in future. */
	virtual void insert(const Shop &) = 0;

	/* */
	virtual void update(
			const Shop &orig,
			const Shop &modified) = 0;

	/* */
	virtual void del(const Shop &) = 0;

	/* */
	virtual void query(ShopSet &) = 0;
};

#endif

