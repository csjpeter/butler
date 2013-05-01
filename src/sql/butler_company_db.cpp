/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_company_db.h"

CompanyDb::CompanyDb(SqlConnection &_sql) :
	sql(_sql),
	companyTable(_sql)
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
	sql.transaction();
	try {
		companyTable.insert(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void CompanyDb::update(const Company &orig, const Company &modified)
{
	sql.transaction();
	try {
		companyTable.update(orig, modified);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void CompanyDb::del(const Company &s)
{
	sql.transaction();
	try {
		companyTable.del(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void CompanyDb::query(CompanySet &ss)
{
	sql.transaction();
	try {
		companyTable.query(ss);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}
