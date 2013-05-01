/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_partner_db.h"

PartnerDb::PartnerDb(SqlConnection &_sql) :
	sql(_sql),
	partnerTable(_sql)
{
}

PartnerDb::~PartnerDb()
{
}

void PartnerDb::check(QStringList &tables)
{
	partnerTable.check(tables);
}

void PartnerDb::insert(const Partner &s)
{
	sql.transaction();
	try {
		partnerTable.insert(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void PartnerDb::update(const Partner &orig, const Partner &modified)
{
	sql.transaction();
	try {
		partnerTable.update(orig, modified);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void PartnerDb::del(const Partner &s)
{
	sql.transaction();
	try {
		partnerTable.del(s);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}

void PartnerDb::query(PartnerSet &ss)
{
	sql.transaction();
	try {
		partnerTable.query(ss);
		sql.commit();
	} catch (...) {
		sql.rollback();
		throw;
	}
}
