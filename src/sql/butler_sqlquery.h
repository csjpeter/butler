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

class SqlQuery : public SqlCloseListener, public SqlFinishListener
{
public:
	SqlQuery(SqlConnection &);
	~SqlQuery();
private:
	SqlQuery();

public:
	void exec(const QString &query);

	void prepare(const QString &query);
	bool isPrepared();
	void bindValue(int pos, const QVariant &v);
	void exec();
	bool next();
	unsigned colIndex(const QString &name);
	QVariant value(int index);
	void finish();

	void sqlFinishNotification();
	void sqlCloseNotification();

private:
	QString queryString();

private:
	SqlConnection &sql;
	QSqlQuery *qQuery;
	bool prepared;
};

#endif
