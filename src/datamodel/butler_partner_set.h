/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_SET_H
#define BUTLER_PARTNER_SET_H

#include <QString>

#include <butler_partner.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

class PartnerSet : public csjp::SorterOwnerContainer<Partner>
{
public:
	PartnerSet() :
		csjp::SorterOwnerContainer<Partner>(),
		ordering(Partner::Name),
		ascending(true){}
	PartnerSet(PartnerSet &ss) :
		csjp::SorterOwnerContainer<Partner>(ss),
		ordering(Partner::Name),
		ascending(true){}
	~PartnerSet() {}

	Partner& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Partner>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Partner>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Partner>::index<QString>(name);}

	virtual int compare(const Partner &a, const Partner &b) const;

	Partner::Fields ordering;
	bool ascending;
};

class PartnerNameSet : public csjp::OwnerContainer<QString>
{
public:
	PartnerNameSet() : csjp::OwnerContainer<QString>() {}
	PartnerNameSet(PartnerNameSet &sns) : csjp::OwnerContainer<QString>(sns) {}
	~PartnerNameSet() {}
};

#endif
