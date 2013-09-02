/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANY_H
#define BUTLER_COMPANY_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <QDate>
#include <QString>

class Company
{
public:
	QString name;		/* Tesco Global Áruházak ZRt. */
	QDateTime lastModified; /* non editable */
	QString country;	/* Magyarország */
	QString city;		/* Budaörs */
	QString postalCode;	/* 2040 */
	QString address;	/* Kinizsi út 1-3. */
	QString taxId;		/* 10307078-2-44 */
	QString icon;		/* base64 representation of an image */
	
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
	Company() :
		name(""),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		country(""),
		city(""),
		postalCode(""),
		address(""),
		taxId(""),
		icon("")
	{
	}

	Company(const QString & _name) :
		name(_name),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		country(""),
		postalCode(""),
		address(""),
		taxId(""),
		icon("")
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
				lastModified.toString() != s.lastModified.toString() ||
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
		return QString::localeAwareCompare(name, s.name) < 0;
	}

	bool isLess(const QString & s) const
	{
		return QString::localeAwareCompare(name, s) < 0;
	}

	bool isMore(const QString & s) const
	{
		return 0 < QString::localeAwareCompare(name, s);
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

inline bool operator<(const QString & a, const Company & b)
{
	return b.isMore(a);
}

inline bool operator<(const Company & a, const QString & b)
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

	Company& query(const QString & name) const {
		return csjp::SorterOwnerContainer<Company>::query<QString>(name);}

	bool has(const QString & name) const {
		return csjp::SorterOwnerContainer<Company>::has<QString>(name);}

	unsigned index(const QString & name) const {
		return csjp::SorterOwnerContainer<Company>::index<QString>(name);}

	virtual int compare(const Company & a, const Company & b) const
	{
		bool ret;

		switch(ordering) {
			case Company::Name :
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
			case Company::Country :
				ret = QString::localeAwareCompare(a.country, b.country) < 0;
				break;
			case Company::City :
				ret = QString::localeAwareCompare(a.city, b.city) < 0;
				break;
			case Company::PostalCode :
				ret = QString::localeAwareCompare(a.postalCode, b.postalCode) < 0;
				break;
			case Company::Address :
				ret = QString::localeAwareCompare(a.address, b.address) < 0;
				break;
			case Company::TaxId :
				ret = QString::localeAwareCompare(a.taxId, b.taxId) < 0;
				break;
			default:
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
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

typedef csjp::OwnerContainer<QString> CompanyNameSet;

#endif
