/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_partner_db.h"

PartnerDb::PartnerDb(SqlConnection &sql) :
	sql(sql),
	partnerTable(sql)
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
	SqlTransaction tr(sql);
	partnerTable.insert(s);
	tr.commit();
}

void PartnerDb::update(const Partner &orig, const Partner &modified)
{
	SqlTransaction tr(sql);
	partnerTable.update(orig, modified);
	tr.commit();
}

void PartnerDb::del(const Partner &s)
{
	SqlTransaction tr(sql);
	partnerTable.del(s);
	tr.commit();
}

void PartnerDb::query(PartnerSet &ss)
{
	SqlTransaction tr(sql);
	partnerTable.query(ss);
	tr.commit();
}
