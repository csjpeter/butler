/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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
	WareSet(WareSet &ws) :
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

