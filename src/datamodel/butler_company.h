/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANY_H
#define BUTLER_COMPANY_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

class Company
{
public:
	Text name;		/* Tesco Global Áruházak ZRt. */
	DateTime lastModified;	/* non editable */
	Text country;		/* Magyarország */
	Text city;		/* Budaörs */
	Text postalCode;	/* 2040 */
	Text address;		/* Kinizsi út 1-3. */
	Text taxId;		/* 10307078-2-44 */
	Text icon;		/* base64 representation of an image */
	
	enum Fields {
		Name = 0,
		Country,
		City,
		PostalCode,
		Address,
		TaxId,
		Icon,
		LastModified,
		NumOfFields
	};

public:
	Company()
	{
	}

	Company(const Text & _name) :
		name(_name)
	{
	}

	Company(const Company & s)
	{
		copy(s);
	}

	~Company()
	{
	}

	Company& operator=(const Company & s)
	{
		copy(s);
		return *this;
	}

	bool isEqual(const Company & s) const
	{
		if(		name != s.name ||
				lastModified != s.lastModified ||
				country != s.country ||
				city != s.city ||
				postalCode != s.postalCode ||
				address != s.address ||
				taxId != s.taxId ||
				icon != s.icon)
			return false;
		return true;
	}

	bool isLess(const Company & s) const
	{
		return name < s.name;
	}

	bool isLess(const Text & s) const
	{
		return name < s;
	}

	bool isMore(const Text & s) const
	{
		return s < name;
	}

private:
	void copy(const Company & s)
	{
		name = s.name;
		lastModified = s.lastModified;
		country = s.country;
		city = s.city;
		postalCode = s.postalCode;
		address = s.address;
		taxId = s.taxId;
		icon = s.icon;
	}
};

inline bool operator==(const Company & a, const Company & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Company & a, const Company & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Company & a, const Company & b)
{
	return a.isLess(b);
}

inline bool operator<(const Text & a, const Company & b)
{
	return b.isMore(a);
}

inline bool operator<(const Company & a, const Text & b)
{
	return a.isLess(b);
}

class CompanySet : public csjp::SorterOwnerContainer<Company>
{
public:
	CompanySet() :
		csjp::SorterOwnerContainer<Company>(),
		ordering(Company::Name),
		ascending(true){}
	CompanySet(const CompanySet & ss) :
		csjp::SorterOwnerContainer<Company>(ss),
		ordering(Company::Name),
		ascending(true){}
	~CompanySet() {}

	virtual int compare(const Company & a, const Company & b) const
	{
		bool ret;

		switch(ordering) {
			case Company::Name :
				ret = a.name < b.name;
				break;
			case Company::Country :
				ret = a.country < b.country;
				break;
			case Company::City :
				ret = a.city < b.city;
				break;
			case Company::PostalCode :
				ret = a.postalCode < b.postalCode;
				break;
			case Company::Address :
				ret = a.address < b.address;
				break;
			case Company::TaxId :
				ret = a.taxId < b.taxId;
				break;
			default:
				ret = a.name < b.name;
				break;
		}

		DBG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

	Company::Fields ordering;
	bool ascending;
};

typedef csjp::OwnerContainer<Text> CompanyNameSet;

#endif
