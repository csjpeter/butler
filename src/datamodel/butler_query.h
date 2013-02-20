/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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
};

#endif

