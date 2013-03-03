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
