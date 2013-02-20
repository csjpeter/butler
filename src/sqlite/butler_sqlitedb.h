/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITEDB_H
#define BUTLER_SQLITEDB_H

#include <butler_db.h>

class Private;

class SqliteDb : public Db
{
public:
	SqliteDb(const QString& path);
	virtual ~SqliteDb();

public:
	bool connect();
	bool open();
	bool close();
	bool create();
	bool check();
	bool update();
	enum Db::UserError lastUserErrorId();
	const QString& lastUserError();
	const QString& lastError();

	TagDb& tag();
	QueryDb& query();
	ItemDb& item();
	WareDb& ware();
	ShopDb& shop();

private:
	Private *priv;
};

#endif

