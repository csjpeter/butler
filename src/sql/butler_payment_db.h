/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PAYMENT_DB_H
#define BUTLER_PAYMENT_DB_H
#if 0

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class PaymentDb
{
public:
	PaymentDb(SqlConnection & sql);
	virtual ~PaymentDb();
private:
	PaymentDb();

public:
	void insert(const Payment &);
	void update(const Payment & orig, const Payment & modified);
	void del(const Payment &);
	void query(const TagNameSet &, PaymentSet &);
	void query(const Query &, QueryStat &, PaymentSet &);

private:
	SqlConnection & sql;
};

#endif
#endif
