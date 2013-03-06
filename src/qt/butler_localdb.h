/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_LOCALDB_H
#define BUTLER_LOCALDB_H

#include <csjp_exception.h>

#include <butler_db.h>
#include <butler_item_set.h>

/* Forwards: */
class QSessionManager;

namespace Butler {

class LocalDb
{
public:
	LocalDb();
	virtual ~LocalDb();

	QString error(QString err = QString());
public:
	static QString dbFileName;

protected:
	Db& db();

private:
	static int refCount;
	static Db* _db;
};

}

#endif



