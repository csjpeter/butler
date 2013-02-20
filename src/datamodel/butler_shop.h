/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOP_H
#define BUTLER_SHOP_H

#include <QString>

#include <butler_tag_set.h>

class Tag;

class Shop
{
public:
	Shop();
	explicit Shop(const QString &_name);
	explicit Shop(const Shop &);
	~Shop();
	
	Shop& operator=(const Shop &);

	bool isEqual(const Shop &) const;

	bool isLess(const Shop &) const;

	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

public:
	QString name;      /* uránvárosi tesco */
	QString storeName; /* number 41470 shop */
	QString city;      /* Pécs */
	QString address;   /* 7634, Makai út 5. */
	QString company;   /* Tesco Global Áruházak ZRt. */
	
	enum Fields {
		Name = 0,
		StoreName,
		City,
		Address,
		Company,
		NumOfFields
	};

private:
	void copy(const Shop &);
};

bool operator==(const Shop &a, const Shop &b);
bool operator!=(const Shop &a, const Shop &b);

bool operator<(const Shop &a, const Shop &b);
bool operator<(const QString &a, const Shop &b);
bool operator<(const Shop &a, const QString &b);

#endif

