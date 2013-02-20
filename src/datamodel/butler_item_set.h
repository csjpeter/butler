/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_SET_H
#define BUTLER_ITEM_SET_H

#include <QString>

#include <butler_item.h>
#include <csjp_sorter_owner_container.h>

class ItemSet : public csjp::SorterOwnerContainer<Item>
{
public:
	ItemSet() :
		csjp::SorterOwnerContainer<Item>(),
		ordering(Item::Name),
		ascending(true){}
	ItemSet(ItemSet &is) :
		csjp::SorterOwnerContainer<Item>(is),
		ordering(Item::Name),
		ascending(true){}
	~ItemSet() {}

	Item& query(const QDateTime &dt) const {
		return csjp::SorterOwnerContainer<Item>::query<QDateTime>(dt);}

	bool has(const QDateTime &dt) const {
		return csjp::SorterOwnerContainer<Item>::has<QDateTime>(dt);}

	unsigned index(const QDateTime &dt) const {
		return csjp::SorterOwnerContainer<Item>::index<QDateTime>(dt);}

	virtual int compare(const Item &a, const Item &b) const;

	Item::Fields ordering;
	bool ascending;
};

#endif

