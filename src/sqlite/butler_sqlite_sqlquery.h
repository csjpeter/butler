/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SQLQUERY_H
#define BUTLER_SQLITE_SQLQUERY_H

#include <QObject>
#include <QSqlDatabase>

#include <butler_db.h>
#include <butler_query.h>
#include <butler_query_set.h>
#include <butler_item.h>
#include <butler_item_set.h>

#include "butler_sqlite_sql.h"

namespace Sqlite {

class SqlQuery : public SqlCloseListener, public SqlFinishListener
{
public:
	SqlQuery(Sql &);
	~SqlQuery();
private:
	SqlQuery();

public:
	bool exec(const QString &query);

	bool prepare(const QString &query);
	bool isPrepared();
	void bindValue(int pos, const QVariant &v);
	bool exec();
	bool next();
	unsigned colIndex(const QString &name);
	QVariant value(int index);
	void finish();

	void sqlFinishNotification();
	void sqlCloseNotification();

private:
	Sql &sql;
	QSqlQuery *qQuery;
	bool prepared;
};

}

#endif

