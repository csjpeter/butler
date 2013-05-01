/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_TABLE_H
#define BUTLER_PARTNER_TABLE_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include <butler_sql_connection.h>
#include <butler_sqlquery.h>

class PartnerTable
{
public:
	PartnerTable(SqlConnection &sql);
	virtual ~PartnerTable();
private:
	PartnerTable();

public:
	void check(QStringList &tables);
	void insert(const Partner &);
	void update(const Partner &orig,
			const Partner &modified);
	void del(const Partner &);
	void query(PartnerSet &);

private:
	SqlConnection &sql;
};

#endif
