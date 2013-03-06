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
	void connect();
	void open();
	void close();
	void create();
	void check();
	void update();

	TagDb& tag();
	QueryDb& query();
	ItemDb& item();
	WareDb& ware();
	ShopDb& shop();

private:
	Private *priv;
};

#endif
