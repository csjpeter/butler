/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_DB_H
#define BUTLER_PARTNER_DB_H

#include <butler_partner_table.h>

class PartnerDb
{
public:
	PartnerDb(SqlConnection &sql);
	virtual ~PartnerDb();
private:
	PartnerDb();

public:
	void check(QStringList &tables);

	void insert(const Partner &);
	void update(const Partner &orig, const Partner &modified);
	void del(const Partner &);
	void query(PartnerSet &);

private:
	SqlConnection &sql;
	PartnerTable partnerTable;
};

#endif
