/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_BRAND_DB_H
#define BUTLER_BRAND_DB_H
#if 0

#include <butler_sql_connection.h>
#include <butler_dataclasses.h>

class BrandDb
{
public:
	BrandDb(SqlConnection & sql);
	virtual ~BrandDb();
private:
	BrandDb();

public:
	void insert(const Brand &);
	void update(const Brand & orig, const Brand & modified);
	void del(const Brand &);
	void query(BrandSet &);

private:
	SqlConnection & sql;
};

#endif
#endif
