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

