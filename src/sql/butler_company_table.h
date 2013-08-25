/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANY_TABLE_H
#define BUTLER_COMPANY_TABLE_H

#include <QObject>
#include <QStringList>

#include <butler_company_set.h>

#include <butler_sql_connection.h>

class CompanyTable
{
public:
	CompanyTable(SqlConnection &sql);
	virtual ~CompanyTable();
private:
	CompanyTable();

public:
	void check(QStringList &tables);
	void insert(const Company &);
	void update(const Company &orig,
			const Company &modified);
	void del(const Company &);
	void query(CompanySet &);

private:
	SqlConnection &sql;
};

#endif
