/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_H
#define BUTLER_PARTNER_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

class Partner
{
public:
	Text name;		/* kertvárosi tesco */
	DateTime lastModified;	/* non editable */
	Text country;		/* Magyarország */
	Text city;		/* Pécs */
	Text postalCode;	/* 7631 */
	Text address;		/* Kincses u. 1. */
	Text company;		/* Tesco Global Áruházak ZRt. */
	Text storeName;		/* 41052 számú bolt */
	
	enum Fields {
		Name = 0,
		Country,
		City,
		PostalCode,
		Address,
		Company,
		StoreName,
		LastModified,
		NumOfFields
	};

public:
	Partner()
	{
	}

	Partner(const Text & _name) :
		name(_name)
	{
	}

	Partner(const Partner & s)
	{
		copy(s);
	}

	~Partner()
	{
	}

	Partner& operator=(const Partner & s)
	{
		copy(s);
		return *this;
	}

	bool isEqual(const Partner & s) const
	{
		if(		name != s.name ||
				lastModified != s.lastModified ||
				country != s.country ||
				city != s.city ||
				postalCode != s.postalCode ||
				address != s.address ||
				company != s.company ||
				storeName != s.storeName)
			return false;
		return true;
	}

	bool isLess(const Partner & s) const
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
	void copy(const Partner & s)
	{
		name = s.name;
		lastModified = s.lastModified;
		country = s.country;
		city = s.city;
		postalCode = s.postalCode;
		address = s.address;
		company = s.company;
		storeName = s.storeName;
	}
};

inline bool operator==(const Partner & a, const Partner & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Partner & a, const Partner & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Partner & a, const Partner & b)
{
	return a.isLess(b);
}

inline bool operator<(const Text & a, const Partner & b)
{
	return b.isMore(a);
}

inline bool operator<(const Partner & a, const Text & b)
{
	return a.isLess(b);
}

class PartnerSet : public csjp::SorterOwnerContainer<Partner>
{
public:
	PartnerSet() :
		csjp::SorterOwnerContainer<Partner>(),
		ordering(Partner::Name),
		ascending(true){}
	PartnerSet(const PartnerSet & ss) :
		csjp::SorterOwnerContainer<Partner>(ss),
		ordering(Partner::Name),
		ascending(true){}
	~PartnerSet() {}

	virtual int compare(const Partner & a, const Partner & b) const
	{
		bool ret;

		switch(ordering) {
			case Partner::Name :
				ret = a.name < b.name;
				break;
			case Partner::StoreName :
				ret = a.storeName < b.storeName;
				break;
			case Partner::City :
				ret = a.city < b.city;
				break;
			case Partner::PostalCode :
				ret = a.postalCode < b.postalCode;
				break;
			case Partner::Address :
				ret = a.address < b.address;
				break;
			case Partner::Company :
				ret = a.company < b.company;
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

	Partner::Fields ordering;
	bool ascending;
};

typedef csjp::OwnerContainer<Text> PartnerNameSet;

#endif
