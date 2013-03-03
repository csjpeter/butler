/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

DECL_EXCEPTION(csjp::ResourceError, DbError);

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


