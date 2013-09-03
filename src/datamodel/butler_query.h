/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_H
#define BUTLER_QUERY_H

#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_tag.h>
#include <butler_ware.h>
#include <butler_partner.h>

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

	Text name;
	DateTime lastModified; /* non editable */
	DateTime startDate;
	DateTime endDate;
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
		startDate("1900-01-01T00:00:00Z"),
		endDate("2200-01-01T00:00:00Z"),
		stockOption(StockOptions::AllBoughtItem),
		tagOption(TagOptions::MatchAny)
	{
	}

	Query(const Text & name) :
		name(name),
		startDate("1900-01-01T00:00:00Z"),
		endDate("2200-01-01T00:00:00Z"),
		stockOption(StockOptions::AllBoughtItem),
		tagOption(TagOptions::MatchAny)
	{
	}

	Query(const Query & qo)
	{
		copy(qo);
	}

	~Query()
	{
	}

	Query& operator=(const Query& qo)
	{
		copy(qo);
		return *this;
	}

	bool isEqual(const Query & qo) const
	{
		if(		name != qo.name ||
				lastModified != qo.lastModified ||
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

	bool isLess(const Query & qo) const
	{
		return name < qo.name;
	}

	bool isLess(const QString & s) const
	{
		return name < s;
	}

	bool isMore(const QString & s) const
	{
		return s < name;
	}

private:
	void copy(const Query & qo)
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

	Query::Fields ordering;
	bool ascending;
};

#endif
