/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_company_db.h"

CompanyDb::CompanyDb(SqlConnection &sql) :
	sql(sql),
	companyTable(sql)
{
}

CompanyDb::~CompanyDb()
{
}

void CompanyDb::check(QStringList &tables)
{
	companyTable.check(tables);
}

void CompanyDb::insert(const Company &s)
{
	SqlTransaction tr(sql);
	companyTable.insert(s);
	tr.commit();
}

void CompanyDb::update(const Company &orig, const Company &modified)
{
	SqlTransaction tr(sql);
	companyTable.update(orig, modified);
	tr.commit();
}

void CompanyDb::del(const Company &s)
{
	SqlTransaction tr(sql);
	companyTable.del(s);
	tr.commit();
}

void CompanyDb::query(CompanySet &ss)
{
	SqlTransaction tr(sql);
	companyTable.query(ss);
	tr.commit();
}
