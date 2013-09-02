/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_H
#define BUTLER_QUERY_H

#include <csjp_sorter_owner_container.h>

#include <butler_tag.h>
#include <butler_ware.h>
#include <butler_partner.h>

#include <QString>
#include <QDate>

class QueryStat{
public:
	unsigned itemCount;
	double sumQuantity;
	double sumPrice;
	double avgPrice;
	double cheapestUnitPrice;
	double mostExpUnitPrice;
	double queryTime; /** How much time the query took. */
};

class Query
{
public:
	enum class StockOptions {
		AllBoughtItem = 0,
		ItemsOnStock,
		ItemsUsedUp
	};
	
	enum class TagOptions {
		MatchAll = 0,
		MatchAny
	};

	QString name;
	QDateTime lastModified; /* non editable */
	QDateTime startDate;
	QDateTime endDate;
	enum StockOptions stockOption;
	enum TagOptions tagOption;
	TagNameSet withTags;
	TagNameSet withoutTags;
	WareNameSet wares;
	PartnerNameSet partners;
	
	enum Fields {
		Name = 0,
		StartDate,
		EndDate,
		LastModified,
		NumOfFields
	};

public:
	Query() :
		name(""),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		startDate(QDate(1900, 1, 1)),
		endDate(QDateTime::currentDateTime()),
		stockOption(StockOptions::AllBoughtItem),
		tagOption(TagOptions::MatchAny)
	{
	}

	Query(const QString & name) :
		name(name),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		startDate(QDate(1900, 1, 1)),
		endDate(QDateTime::currentDateTime()),
		stockOption(StockOptions::AllBoughtItem),
		tagOption(TagOptions::MatchAny)
	{
	}

	Query(const Query & qo)
	{
		equal(qo);
	}

	~Query()
	{
	}

	Query& operator=(const Query& qo)
	{
		equal(qo);
		return *this;
	}

	bool isEqual(const Query & qo) const
	{
		if(		name != qo.name ||
				lastModified.toString() != qo.lastModified.toString() ||
				startDate.toString() != qo.startDate.toString() ||
				endDate.toString() != qo.endDate.toString() ||
				stockOption != qo.stockOption ||
				tagOption != qo.tagOption ||
				withTags != qo.withTags ||
				withoutTags != qo.withoutTags ||
				wares != qo.wares ||
				partners != qo.partners)
			return false;
		return true;
	}

	bool isLess(const Query & qo) const
	{
		return QString::localeAwareCompare(name, qo.name) < 0;
	}

	bool isLess(const QString & s) const
	{
		return QString::localeAwareCompare(name, s) < 0;
	}

	bool isMore(const QString & s) const
	{
		return 0 < QString::localeAwareCompare(name, s);
	}

private:
	void equal(const Query & qo)
	{
		name = qo.name;
		lastModified = qo.lastModified;
		startDate = qo.startDate;
		endDate = qo.endDate;
		stockOption = qo.stockOption;
		tagOption = qo.tagOption;
		withTags.copy(qo.withTags);
		withoutTags.copy(qo.withoutTags);
		wares.copy(qo.wares);
		partners.copy(qo.partners);
	}
};

inline bool operator==(const Query & a, const Query & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Query & a, const Query & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Query & a, const Query & b)
{
	return a.isLess(b);
}

inline bool operator<(const QString & a, const Query & b)
{
	return b.isMore(a);
}

inline bool operator<(const Query & a, const QString & b)
{
	return a.isLess(b);
}

class QuerySet : public csjp::SorterOwnerContainer<Query>
{
public:
	QuerySet() : csjp::SorterOwnerContainer<Query>() {}
	QuerySet(const QuerySet & qs) : csjp::SorterOwnerContainer<Query>(qs) {}
	~QuerySet() {}

	Query& query(const QString & name) const {
		return csjp::SorterOwnerContainer<Query>::query<QString>(name);}

	bool has(const QString & name) const {
		return csjp::SorterOwnerContainer<Query>::has<QString>(name);}

	unsigned index(const QString & name) const {
		return csjp::SorterOwnerContainer<Query>::index<QString>(name);}

	Query::Fields ordering;
	bool ascending;
};

#endif
