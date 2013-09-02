/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNER_H
#define BUTLER_PARTNER_H

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <QDate>
#include <QString>

//#include <butler_tag.h>

class Partner
{
public:
	QString name;		/* kertvárosi tesco */
	QDateTime lastModified; /* non editable */
	QString country;	/* Magyarország */
	QString city;		/* Pécs */
	QString postalCode;	/* 7631 */
	QString address;	/* Kincses u. 1. */
	QString company;	/* Tesco Global Áruházak ZRt. */
	QString storeName;	/* 41052 számú bolt */
	
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
	Partner() :
		name(""),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		country(""),
		city(""),
		postalCode(""),
		address(""),
		company(""),
		storeName("")
	{
	}

	Partner(const QString & _name) :
		name(_name),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		country(""),
		city(""),
		postalCode(""),
		address(""),
		company(""),
		storeName("")
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
				lastModified.toString() != s.lastModified.toString() ||
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

inline bool operator<(const QString & a, const Partner & b)
{
	return b.isMore(a);
}

inline bool operator<(const Partner & a, const QString & b)
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

	Partner& query(const QString & name) const {
		return csjp::SorterOwnerContainer<Partner>::query<QString>(name);}

	bool has(const QString & name) const {
		return csjp::SorterOwnerContainer<Partner>::has<QString>(name);}

	unsigned index(const QString & name) const {
		return csjp::SorterOwnerContainer<Partner>::index<QString>(name);}

	virtual int compare(const Partner & a, const Partner & b) const
	{
		bool ret;

		switch(ordering) {
			case Partner::Name :
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
			case Partner::StoreName :
				ret = QString::localeAwareCompare(a.storeName, b.storeName) < 0;
				break;
			case Partner::City :
				ret = QString::localeAwareCompare(a.city, b.city) < 0;
				break;
			case Partner::PostalCode :
				ret = QString::localeAwareCompare(a.postalCode, b.postalCode) < 0;
				break;
			case Partner::Address :
				ret = QString::localeAwareCompare(a.address, b.address) < 0;
				break;
			case Partner::Company :
				ret = QString::localeAwareCompare(a.company, b.company) < 0;
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

	Partner::Fields ordering;
	bool ascending;
};

typedef csjp::OwnerContainer<QString> PartnerNameSet;

#endif
