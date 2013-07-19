/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_SET_H
#define BUTLER_WARE_SET_H

#include <QString>

#include <butler_ware.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

class WareSet : public csjp::SorterOwnerContainer<Ware>
{
public:
	WareSet() :
		csjp::SorterOwnerContainer<Ware>(),
		ordering(Ware::Name),
		ascending(true){}
	WareSet(const WareSet &ws) :
		csjp::SorterOwnerContainer<Ware>(ws),
		ordering(Ware::Name),
		ascending(true){}
	~WareSet() {}

	Ware& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Ware>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Ware>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Ware>::index<QString>(name);}

	virtual int compare(const Ware &a, const Ware &b) const;

	Ware::Fields ordering;
	bool ascending;
};

class WareNameSet : public csjp::OwnerContainer<QString>
{
public:
	WareNameSet() : csjp::OwnerContainer<QString>() {}
	WareNameSet(WareNameSet &wns) : csjp::OwnerContainer<QString>(wns) {}
	~WareNameSet() {}
};

#endif
