/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_ACCOUNT_DB_H
#define BUTLER_ACCOUNT_DB_H

#include <butler_sql_connection.h>

class AccountDb
{
public:
	AccountDb(SqlConnection & sql);
	virtual ~AccountDb();
private:
	AccountDb();

public:
	void insert(const Account &);
	void update(const Account & orig, const Account & modified);
	void del(const Account &);
	void query(AccountSet &);

private:
	SqlConnection & sql;
};

#endif
