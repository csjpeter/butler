/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_query.h"

Query::Query() :
	name("")
{
}

Query::Query(const QString &_name)
{
	name = _name;
	stockOption = ALL_BOUGHT_ITEM;
	tagOption = MATCH_ANY_TAGS;
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
	bool ret = true;
	if(name != qo.name){
//		MSG("[%s] == [%s]", qPrintable(name), qPrintable(qo.name));
		ret = false;
	}
	if(startDate != qo.startDate){
//		MSG("[%s] == [%s]",
//				qPrintable(startDate.toString()),
//				qPrintable(qo.startDate.toString()));
		ret = false;
	}
	if(endDate != qo.endDate){
//		MSG("[%s] == [%s]",
//				qPrintable(endDate.toString()),
//				qPrintable(qo.endDate.toString()));
		ret = false;
	}
	if(stockOption != qo.stockOption){
//		MSG("[%d] == [%d]", stockOption, qo.stockOption);
		ret = false;
	}
	if(tagOption != qo.tagOption){
//		MSG("[%s] == [%s]", tagOtpion, qo.tagOption);
		ret = false;
	}
	if(withTags != qo.withTags){
//		MSG("[%d] == [%d]", withTags.size(), qo.withTags.size());
		ret = false;
	}
	if(withoutTags != qo.withoutTags){
//		MSG("[%d] == [%d]", withoutTags.size(), qo.withoutTags.size());
		ret = false;
	}
	if(wares != qo.wares){
//		MSG("[%d] == [%d]", wares.size(), qo.wares.size());
		ret = false;
	}
	if(shops != qo.shops){
//		MSG("[%d] == [%d]", shops.size(), qo.shops.size());
		ret = false;
	}
	return ret;
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
	shops.copy(qo.shops);
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

