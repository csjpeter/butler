/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DB_H
#define BUTLER_DB_H

#include <QString>

#include <butler_tag_db.h>
#include <butler_query_db.h>
#include <butler_item_db.h>
#include <butler_ware_db.h>
#include <butler_shop_db.h>

DECL_EXCEPTION(csjp::ResourceError, DbError);

class Db
{
public:
	Db(){}
	virtual ~Db(){}

	/* Connection needs to be estabilished before doing
	 * anything else. */
	virtual void connect() = 0;

	/* Opening databse is required for all but database
	 * creation. */
	virtual void open() = 0;
	/* Every opened database needs to be closed. */
	virtual void close() = 0;

	/* Create needs to be called on a not yet opened
	 * database. Open not neccessarily working on a
	 * not yet created databse anyway. */
	virtual void create() = 0;

	/* Verifies the database schema. */
	virtual void check() = 0;

	/* Upgrades the database and it's tables. */
	virtual void update() = 0;

	virtual TagDb& tag() = 0;
	virtual QueryDb& query() = 0;
	virtual ItemDb& item() = 0;
	virtual WareDb& ware() = 0;
	virtual ShopDb& shop() = 0;
};

#endif

