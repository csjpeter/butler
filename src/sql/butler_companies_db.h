/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_COMPANIES_DB_H
#define BUTLER_COMPANIES_DB_H

#include <butler_sql_connection.h>

class CompaniesDb
{
public:
	CompaniesDb(SqlConnection & sql);
	virtual ~CompaniesDb();
private:
	CompaniesDb();

public:
	void insert(const Company &);
	void update(const Company & orig, const Company & modified);
	void del(const Company &);
	void query(CompanySet &);

private:
	SqlConnection & sql;
};

#endif
