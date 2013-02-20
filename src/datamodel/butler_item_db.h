/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

