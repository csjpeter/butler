/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERY_H
#define BUTLER_QUERY_H

#include <QString>
#include <QDate>

#include <butler_tag_set.h>
#include <butler_ware_set.h>
#include <butler_shop_set.h>

class Query
{
public:
	enum StockOptions {
		ALL_BOUGHT_ITEM = 0,
		ITEMS_ON_STOCK,
		ITEMS_USED_UP
	};
	
	enum TagOptions {
		MATCH_ALL_TAGS = 0,
		MATCH_ANY_TAGS
	};

	Query();
	explicit Query(const QString &_name);
	explicit Query(const Query &qo);
	~Query();

	Query& operator=(const Query& qo);

	bool isEqual(const Query &qo) const;

	bool isLess(const Query &qo) const;
	
	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

public:
	QString name;
	QDateTime startDate;
	QDateTime endDate;
	enum StockOptions stockOption;
	enum TagOptions tagOption;
	TagNameSet withTags;
	TagNameSet withoutTags;
	WareNameSet wares;
	ShopNameSet shops;
	
	enum Fields {
		Name = 0,
		StartDate,
		EndDate,
		NumOfFields
	};

private:
	void equal(const Query &qo);
};

bool operator==(const Query &a, const Query &b);
bool operator==(const Query &a, const Query &b);
bool operator!=(const Query &a, const Query &b);

bool operator<(const Query &a, const Query &b);
bool operator<(const QString &a, const Query &b);
bool operator<(const Query &a, const QString &b);

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

#endif
