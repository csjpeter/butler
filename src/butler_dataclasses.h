/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

#include <QString>

#include <csjp_array.h>

#include <butler_sql_connection.h>



class TypeNameSet : public csjp::SorterOwnerContainer<Text>
{
public:
		TypeNameSet() : csjp::SorterOwnerContainer<Text>(), ascending(true){}
		TypeNameSet(const TypeNameSet & cns) :
				csjp::SorterOwnerContainer<Text>(cns),
				ascending(cns.ascending)
		{}
		TypeNameSet(TypeNameSet && temp) :
				csjp::SorterOwnerContainer<Text>(csjp::move_cast(temp)),
				ascending(temp.ascending)
		{
				temp.ascending = true;
		}
		~TypeNameSet() {}

		const TypeNameSet & operator=(TypeNameSet && temp)
		{
				csjp::SorterOwnerContainer<Text>::operator=(csjp::move_cast(temp));
				ascending = temp.ascending;
				temp.ascending = true;
				return *this;
		}

		int compare(const Text & a, const Text & b) const
		{
				bool ret = a < b;
				if(!ascending)
						ret = !ret;
				return ret ? -1 : 1;
		}
		bool ascending;
};

typedef csjp::OwnerContainer<Text> TagNameSet;
typedef csjp::OwnerContainer<Text> WareNameSet;
typedef csjp::OwnerContainer<Text> PartnerNameSet;
//typedef csjp::OwnerContainer<Text> CompanyNameSet;
//typedef csjp::OwnerContainer<Text> AccountNameSet;

class QueryStat{
public:
	unsigned itemCount;
	double sumQuantity;
	double sumPrice;
	double avgPrice;
	double cheapestUnitPrice;
	double mostExpUnitPrice;
	double queryTime; /** How much time the query took. */
};

/*@BeginDecl@
	Class Tag
	Fields {
		Text name; key			; TEXT
		Text description;		; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Tag
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h
@include@ dbclass.h

/*@BeginDecl@
	Class Ware
	Fields {
		Text name; key			; TEXT
		Text unit;				; TEXT NOT NULL DEFAULT ''
		TypeNameSet types;
		TagNameSet tags;
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Ware
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Company
	Fields {
		Text name; key			; TEXT									// Tesco Global Áruházak
		Text country;			; TEXT NOT NULL DEFAULT ''				// Magyarország
		Text city;														// Budaörs
		Text postalCode;												// 2040
		Text address;													// Kinizsi út 1-3.
		Text taxId;														// 10307078-2-44
		Text icon;														// base64 repr of an image
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Company
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Brand
	Fields {
		Text name; key			; TEXT
		Text company;			; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Brand
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Inventory
	Fields {
		Text name; key			; TEXT
		Text comment;			; TEXT NOT NULL DEFAULT ''
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Inventory
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Item
	Fields {
		DateTime uploadDate; key	// non editable
		Text name;
		Text unit;
		Text type;
		Text brand;
		double quantity;			// amoutn to buy or not
		double price;				// price of gross piece/amount quantity
		Text currency;
		Text account;
		Text partner;
		Text inventory;
		Text comment;
		DateTime invChangeDate;
		DateTime lastModified;		// non editable
		}
	Constraints {
		PRIMARY KEY (uploadDate)
		}
@EndDecl@*/

class Item
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Partner
	Fields {
		Text name; key				// Kertvárosi tesco
		Text country;				// Magyarország
		Text city;			// Pécs
		Text postalCode;			// 7631
		Text address;				// Kincses út 1.
		Text company;				// Tesco Global Áruházak Zrt.
		Text storeName;				// 41052 számú bolt
		DateTime lastModified;		// non-editable
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Partner
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Account
	Fields {
		Text name; key
		Text currency;
		Text bankOffice;
		Text iban;
		Text swiftCode;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Account
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Payment
	Fields {
		DateTime uploadDate; key
		Text account;
		Text partner;
		double amount;
		DateTime subject;
		DateTime payDate;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (uploadDate)
		}
@EndDecl@*/

class Payment
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

/*@BeginDecl@
	Class Query
	Fields {
		Text name; key
		DateTime startDate;
		DateTime endDate;
		enum StockOptions stockOption;
		enum TagOptions tagOption;
		TagNameSet withTags;
		TagNameSet withoutTags;
		WareNameSet wares;
		PartnerNameSet partners;
		DateTime lastModified;
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@*/

class Query
{
public:
	enum class StockOptions {
		AllItemChanges,
		Gains,
		Looses
	};

	enum class TagOptions {
		MatchAll,
		MatchAny
	};

	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

#endif
