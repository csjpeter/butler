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
DECL_EXCEPTION(DbError, DbLogicError);
DECL_EXCEPTION(DbError, DbIncompatibleTableError);

class Db
{
public:

	enum UserError {
		UNSPECIFIED_USER_ERROR = 0,
		UPDATE_ERROR_OBJECT_NOT_EXISTS,
		UPDATE_ERROR_OBJECT_CHANGED,
		OPEN_ERROR_OLD_DATABASE_SCHEMA,
		INCOMPATIBLE_DATABASE_SCHEMA,
		INCOMPATIBLE_DATABASE_ENGINE,
		FAILED_TO_OPEN_DATABASE
	};

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

	/* Verifies the database schema. */
	virtual void check() = 0;

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

