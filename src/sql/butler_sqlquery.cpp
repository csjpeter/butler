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

#include "butler_sqlquery.h"

SqlQuery::SqlQuery(SqlConnection & sql) :
	sql(sql),
	qQuery(0),
	prepared(false)
{
}

SqlQuery::~SqlQuery()
{
	delete qQuery;
}

void SqlQuery::exec(const QString &query)
{
	if(!qQuery){
		qQuery = sql.createQuery();
		qQuery->setForwardOnly(true);
	}

	prepared = false;

	DBG("%s", C_STR(query));
	if(!qQuery->exec(query))
		throw DbError("The below sql query failed:\n%swith error: %s",
				C_STR(qQuery->executedQuery()),
				C_STR(sql.dbErrorString()));
}

bool SqlQuery::isPrepared()
{
	return prepared;
}

void SqlQuery::prepare(const QString &query)
{
	if(!qQuery){
		qQuery = sql.createQuery();
		qQuery->setForwardOnly(true);
	}

	if(!qQuery->prepare(query))
		throw DbError("Failed to prepare query:\n%serror: %s",
				C_STR(query),
				C_STR(sql.dbErrorString()));
}

void SqlQuery::bindValue(int pos, const QVariant &v)
{
	ENSURE(qQuery, csjp::LogicError);

	qQuery->bindValue(pos, v);
}

void SqlQuery::exec()
{
	ENSURE(qQuery, csjp::LogicError);

	DBG("%s", C_STR(qQuery->lastQuery()));
	if(!qQuery->exec())
		throw DbError("The below sql query failed:\n%swith error: %s",
			C_STR(queryString()), C_STR(sql.dbErrorString()));
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
	if(ret < 0)
		throw DbIncompatibleTableError(
				"There is no column '%s' in the result for query:\n%s.",
				C_STR(name), C_STR(queryString()));

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
	if(!qQuery)
		return;

	qQuery->finish();
	qQuery->clear();
}

QString SqlQuery::queryString()
{
	ENSURE(qQuery, csjp::LogicError);

	QString str;
	str += qQuery->executedQuery();
	str += "\n";
	str += qQuery->lastQuery();
	str += "\n";
	QList<QVariant> list = qQuery->boundValues().values();
	for (int i = 0; i < list.size(); ++i){
		str += i;
		str += " : ";
		str += list.at(i).toString().toAscii().data();
		str += "\n";
	}

	return str;
}
