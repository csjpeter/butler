/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

