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

