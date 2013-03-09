/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_SET_H
#define BUTLER_QUERY_SET_H

#include <QString>

#include <butler_query.h>
#include <csjp_sorter_owner_container.h>

class QuerySet : public csjp::SorterOwnerContainer<Query>
{
public:
	QuerySet() : csjp::SorterOwnerContainer<Query>() {}
	QuerySet(QuerySet &qs) : csjp::SorterOwnerContainer<Query>(qs) {}
	~QuerySet() {}

	Query& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Query>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Query>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Query>::index<QString>(name);}

	Query::Fields ordering;
	bool ascending;
};

#endif
