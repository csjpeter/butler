/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_ITEM_H
#define BUTLER_SQLITE_ITEM_H

#include <QObject>
#include <QStringList>
#include <QSqlDatabase>

#include "butler_sqlite_sql.h"
#include "butler_sqlite_sqlquery.h"

namespace Sqlite {

class ItemTable
{
public:
	ItemTable(Sql &sql);
	virtual ~ItemTable();
private:
	ItemTable();

public:
	bool create();
	bool check(QStringList &tables);
	bool insert(const Item &);
	bool update(const Item &orig,
			const Item &modified);
	bool del(const Item &);
	bool query(Item &);
	/* results only with unbought items */
	bool query(const TagNameSet &, ItemSet &);

private:
	Sql &sql;
	SqlQuery insertQuery;
	SqlQuery updateQuery;
	SqlQuery deleteQuery;
	SqlQuery selectQuery;
	SqlQuery selectNamesQuery;
};

}

#endif

