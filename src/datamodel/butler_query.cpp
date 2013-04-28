/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_query.h"

Query::Query() :
	name(""),
	startDate(QDate(1900, 1, 1)),
	endDate(QDateTime::currentDateTime()),
	stockOption(StockOptions::AllBoughtItem),
	tagOption(TagOptions::MatchAny)
{
}

Query::Query(const QString & name) :
	name(name),
	startDate(QDate(1900, 1, 1)),
	endDate(QDateTime::currentDateTime()),
	stockOption(StockOptions::AllBoughtItem),
	tagOption(TagOptions::MatchAny)
{
}

Query::Query(const Query &qo)
{
	equal(qo);
}

Query::~Query()
{
}

Query& Query::operator=(const Query& qo)
{
	equal(qo);
	return *this;
}

bool Query::isEqual(const Query &qo) const
{
	if(		name != qo.name ||
			startDate != qo.startDate ||
			endDate != qo.endDate ||
			stockOption != qo.stockOption ||
			tagOption != qo.tagOption ||
			withTags != qo.withTags ||
			withoutTags != qo.withoutTags ||
			wares != qo.wares ||
			partners != qo.partners)
		return false;
	return true;
}
		
bool Query::isLess(const Query &qo) const
{
	return QString::localeAwareCompare(name, qo.name) < 0;
}

bool Query::isLess(const QString &s) const
{
	return QString::localeAwareCompare(name, s) < 0;
}

bool Query::isMore(const QString &s) const
{
	return 0 < QString::localeAwareCompare(name, s);
}

void Query::equal(const Query &qo)
{
	name = qo.name;
	startDate = qo.startDate;
	endDate = qo.endDate;
	stockOption = qo.stockOption;
	tagOption = qo.tagOption;
	withTags.copy(qo.withTags);
	withoutTags.copy(qo.withoutTags);
	wares.copy(qo.wares);
	partners.copy(qo.partners);
}

bool operator==(const Query &a, const Query &b)
{
	return a.isEqual(b);
}

bool operator!=(const Query &a, const Query &b)
{
	return !a.isEqual(b);
}

bool operator<(const Query &a, const Query &b)
{
	return a.isLess(b);
}

bool operator<(const QString &a, const Query &b)
{
	return b.isMore(a);
}

bool operator<(const Query &a, const QString &b)
{
	return a.isLess(b);
}
