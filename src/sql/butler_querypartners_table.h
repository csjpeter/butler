/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYPARTNERS_TABLE_H
#define BUTLER_QUERYPARTNERS_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_sql_connection.h>

#include <butler_query_set.h>

class QueryPartnersTable
{
public:
	QueryPartnersTable(SqlConnection &_sql);
	virtual ~QueryPartnersTable();
private:
	QueryPartnersTable();

public:
	void check(QStringList &tables);

	void insert(const Query &q, const QString &partnerName);
	void del(const Query &q, const QString &partnerName);

	void insert(const Query &q);
	void update(const Query &orig, const Query &modified);
	void del(const Query &q);
	void query(const Query &q, PartnerNameSet &partners);

private:
	SqlConnection &sql;
};

#endif
