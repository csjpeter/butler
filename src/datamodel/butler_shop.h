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

