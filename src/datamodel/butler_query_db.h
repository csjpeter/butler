/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_DB_H
#define BUTLER_QUERY_DB_H

#include <butler_query_set.h>

class QueryDb
{
public:
	/* A query must have valid start and end dates. */
	virtual bool insert(
			const Query &) = 0;

	/* */
	virtual bool update(
			const Query &orig,
			const Query &modified) = 0;

	/* */
	virtual bool del(
			const Query &) = 0;

	/* may be there is no need for this */
/*			virtual bool query(const QString &name, Query &) = 0;*/

	/* A set of all available queries. */
	virtual bool query(QuerySet &) = 0;
};

#endif

