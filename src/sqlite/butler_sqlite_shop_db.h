/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SQLITE_SHOP_DB_H
#define BUTLER_SQLITE_SHOP_DB_H

#include <butler_db.h>

#include "butler_sqlite_shop.h"

namespace Sqlite {

class ShopDb : public ::ShopDb
{
public:
	ShopDb(Sql &sql);
	virtual ~ShopDb();
private:
	ShopDb();

public:
	bool create();
	bool check(QStringList &tables);
	bool update();

	bool insert(const Shop &);
	bool update(const Shop &orig, const Shop &modified);
	bool del(const Shop &);
	bool query(ShopSet &);

private:
	Sql &sql;
	ShopTable shopTable;
};

}

#endif

