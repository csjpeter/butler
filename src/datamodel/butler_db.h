/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

class Db
{
public:

	enum UserError {
		UNSPECIFIED_USER_ERROR = 0,
		UPDATE_ERROR_OBJECT_NOT_EXISTS,
		UPDATE_ERROR_OBJECT_CHANGED,
		OPEN_ERROR_OLD_DATABASE_SCHEMA,
		INCOMPATIBLE_DATABASE_SCHEMA,
		INCOMPATIBLE_DATABASE_ENGINE
	};

	Db(){}
	virtual ~Db(){}

	/* Connection needs to be estabilished before doing
	 * anything else. */
	virtual bool connect() = 0;

	/* Opening databse is required for all but database
	 * creation. */
	virtual bool open() = 0;
	/* Every opened database needs to be closed. */
	virtual bool close() = 0;

	/* Create needs to be called on a not yet opened
	 * database. Open not neccessarily working on a
	 * not yet created databse anyway. */
	virtual bool create() = 0;

	/* Verifies the database schema. */
	virtual bool check() = 0;

	/* Upgrades the database and it's tables. */
	virtual bool update() = 0;

	/* Use this to identify what kind of error happened. */
	virtual enum UserError lastUserErrorId() = 0;

	/* If lastUserErrorId reports other than unspecified,
	 * this will return the explanation of the error. */
	virtual const QString& lastUserError() = 0;

	/* In case lastUserErrorId reports unspecified, an
	 * unexpected db error happened that might be because
	 * of a database backend implementation error. The
	 * original databaseengine/database server reported
	 * error is given by this method. */
	virtual const QString& lastError() = 0;

	virtual TagDb& tag() = 0;
	virtual QueryDb& query() = 0;
	virtual ItemDb& item() = 0;
	virtual WareDb& ware() = 0;
	virtual ShopDb& shop() = 0;
};

#endif

