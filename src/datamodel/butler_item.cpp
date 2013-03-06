/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item.h"

Item::Item() :
	uploaded(QDate(0,0,0), QTime(0,0,0)),
	name(""),
	category(),
	quantity(1),
	comment(""),
	bought(false),
	purchased(QDate(0,0,0), QTime(0,0,0)),
	shop(""),
	price(0),
	onStock(false)
{
}

Item::Item(const QString &_name) :
	uploaded(QDate(0,0,0), QTime(0,0,0)),
	name(_name),
	category(""),
	quantity(1),
	comment(""),
	bought(false),
	purchased(QDate(0,0,0), QTime(0,0,0)),
	shop(""),
	price(0),
	onStock(false)
{
}

Item::Item(const Item &i)
{
	copy(i);
}

Item::~Item()
{
}

Item& Item::operator=(const Item &i)
{
	copy(i);
	return *this;
}

bool Item::isEqual(const Item &i) const
{
//	MSG("[%s] == [%s]", qPrintable(name), qPrintable(i.name));
	if(name != i.name) return false;

//	MSG("[%s] == [%s]", qPrintable(category), qPrintable(i.category));
	if(category != i.category) return false;

//	MSG("[%s] == [%s]",
//			qPrintable(uploaded.toString()),
//			qPrintable(i.uploaded.toString()));

	if(uploaded.toString() != i.uploaded.toString()) return false;
//	MSG("[%s] == [%s]",
//			qPrintable(purchased.toString()),
//			qPrintable(i.purchased.toString()));

	if(purchased.toString() != i.purchased.toString()) return false;
//	MSG("[%d] == [%d]", shop, i.shop);
	if(shop != i.shop) return false;
//	MSG("[%d] == [%d]", price, i.price);
	if(price != i.price) return false;
//	MSG("[%d] == [%d]", quantity, i.quantity);
	if(quantity != i.quantity) return false;
//	MSG("[%d] == [%d]", onStock, i.onStock);
	if(onStock != i.onStock) return false;
//	MSG("[%s] == [%s]", qPrintable(comment), qPrintable(comment));
	if(comment != i.comment) return false;

	return true;
}

bool Item::isLess(const Item &i) const
{
	return uploaded < i.uploaded;
}

bool Item::isLess(const QDateTime &dt) const
{
	return uploaded < dt;
}

bool Item::isMore(const QDateTime &dt) const
{
	return dt < uploaded;
}

void Item::copy(const Item &i)
{
	uploaded = i.uploaded;
	name = i.name;
	category = i.category;
	quantity = i.quantity;
	comment = i.comment;

	bought = i.bought;
	purchased = i.purchased;
	shop = i.shop;
	price = i.price;
	onStock = i.onStock;
}

bool operator==(const Item &a, const Item &b)
{
	return a.isEqual(b);
}

bool operator!=(const Item &a, const Item &b)
{
	return !a.isEqual(b);
}

bool operator<(const Item &a, const Item &b)
{
	return a.isLess(b);
}

bool operator<(const Item &a, const QDateTime &b)
{
	return a.isLess(b);
}

bool operator<(const QDateTime &a, const Item &b)
{
	return b.isMore(a);
}
