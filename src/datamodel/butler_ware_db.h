/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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


