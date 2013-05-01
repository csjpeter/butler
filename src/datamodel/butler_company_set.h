/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANY_SET_H
#define BUTLER_COMPANY_SET_H

#include <QString>

#include <butler_company.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

class CompanySet : public csjp::SorterOwnerContainer<Company>
{
public:
	CompanySet() :
		csjp::SorterOwnerContainer<Company>(),
		ordering(Company::Name),
		ascending(true){}
	CompanySet(CompanySet &ss) :
		csjp::SorterOwnerContainer<Company>(ss),
		ordering(Company::Name),
		ascending(true){}
	~CompanySet() {}

	Company& query(const QString &name) const {
		return csjp::SorterOwnerContainer<Company>::query<QString>(name);}

	bool has(const QString &name) const {
		return csjp::SorterOwnerContainer<Company>::has<QString>(name);}

	unsigned index(const QString &name) const {
		return csjp::SorterOwnerContainer<Company>::index<QString>(name);}

	virtual int compare(const Company &a, const Company &b) const;

	Company::Fields ordering;
	bool ascending;
};

class CompanyNameSet : public csjp::OwnerContainer<QString>
{
public:
	CompanyNameSet() : csjp::OwnerContainer<QString>() {}
	CompanyNameSet(CompanyNameSet &sns) : csjp::OwnerContainer<QString>(sns) {}
	~CompanyNameSet() {}
};

#endif
