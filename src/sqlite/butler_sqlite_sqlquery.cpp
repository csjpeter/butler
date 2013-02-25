/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QVariant>
#include <QMap>

#include <QStringList>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

SqlQuery::SqlQuery(Sql &_sql) :
	sql(_sql),
	qQuery(0),
	prepared(false)
{
	sql.addSqlCloseListener(*this);
}

SqlQuery::~SqlQuery()
{
	sql.removeSqlCloseListener(*this);
	if(qQuery)
		delete qQuery;
}

bool SqlQuery::exec(const QString &query)
{
	bool ret;

	if(!qQuery){
		qQuery = sql.createQuery();
		qQuery->setForwardOnly(true);
		ENSURE(qQuery, csjp::LogicError);
	}

	prepared = false;
	ret = qQuery->exec(query);
	ret = sql.reportSqlError() && ret;

	DBG("SQL: %s", qPrintable(qQuery->executedQuery()));

	return ret;
}

bool SqlQuery::isPrepared()
{
	return prepared;
}

bool SqlQuery::prepare(const QString &query)
{
	bool ret;

	if(!qQuery){
		qQuery = sql.createQuery();
		qQuery->setForwardOnly(true);
		ENSURE(qQuery, csjp::LogicError);
	}

	ret = qQuery->prepare(query);
	ret = sql.reportSqlError() && ret;
	prepared = ret;

	return ret;
}

void SqlQuery::bindValue(int pos, const QVariant &v)
{
	ENSURE(qQuery, csjp::LogicError);

	qQuery->bindValue(pos, v);
}

bool SqlQuery::exec()
{
	ENSURE(qQuery, csjp::LogicError);

	bool ret;

	ret = qQuery->exec();
	ret = sql.reportSqlError() && ret;

#ifdef DEBUG
//	DBG("SQL: %s", qPrintable(qQuery->executedQuery()));
	DBG("SQL: %s", qPrintable(qQuery->lastQuery()));
	QList<QVariant> list = qQuery->boundValues().values();
	for (int i = 0; i < list.size(); ++i)
		DBG("%d : [%s]", i, list.at(i).toString().toAscii().data());
	if(!ret)
		DBG("command failed");
#endif

	return ret;
}

bool SqlQuery::next()
{
	ENSURE(qQuery, csjp::LogicError);

	return qQuery->next();
}

unsigned SqlQuery::colIndex(const QString &name)
{
	ENSURE(qQuery, csjp::LogicError);

	int ret = qQuery->record().indexOf(name);
	if(ret < 0){
		LOG("There is no column %s in table %s.",
				qPrintable(name), "FIXME-unknown");
		ret = 0; /* FIXME throw exception */
	}

	return ret;
}

QVariant SqlQuery::value(int index)
{
	ENSURE(qQuery, csjp::LogicError);

	DBG("%s : [%s]",
			qPrintable(qQuery->record().fieldName(index)),
			qPrintable(qQuery->value(index).toString())
			);

	return qQuery->value(index);
}

void SqlQuery::finish()
{
	ENSURE(qQuery, csjp::LogicError);

	qQuery->finish();
}

void SqlQuery::sqlFinishNotification()
{
	if(qQuery)
		qQuery->finish();
}

void SqlQuery::sqlCloseNotification()
{
	if(qQuery)
		qQuery->clear();
}

}

