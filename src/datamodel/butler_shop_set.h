/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOP_SET_H
#define BUTLER_SHOP_SET_H

#include <QString>

#include <butler_shop.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

class ShopSet : public csjp::SorterOwnerContainer<Shop>
{
public:
	ShopSet() :
		csjp::SorterOwnerContainer<Shop>(),
		ordering(Shop::Name),
		ascending(true){}
	ShopSet(ShopSet &ss) :
		csjp::SorterOwnerContainer<Shop>(ss),
		ordering(Shop::Name),
		ascending(true){}
	~ShopSet() {}

	Shop& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Shop>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Shop>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Shop>::index<QString>(name);}

	virtual int compare(const Shop &a, const Shop &b) const;

	Shop::Fields ordering;
	bool ascending;
};

class ShopNameSet : public csjp::OwnerContainer<QString>
{
public:
	ShopNameSet() : csjp::OwnerContainer<QString>() {}
	ShopNameSet(ShopNameSet &sns) : csjp::OwnerContainer<QString>(sns) {}
	~ShopNameSet() {}
};

#endif
