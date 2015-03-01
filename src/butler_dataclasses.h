/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

#include <QString>

#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>
#include <csjp_exception.h>

#include <butler_datetime.h>
// #include <butler_sql_connection.h>



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

/*
@BeginDecl@
	Class DatabaseDescriptor
	Begin {
		QString name;	key		// will be the connection name
		QString driver;			// for example QSQLITE
		QString databaseName;	// file name in case of sqlite
		QString username;
		QString password;
		bool savePassword;
		QString host;			// host name or ip
		unsigned port;
		}
@EndDecl@

class DatabaseDescriptor
{
	@include@ defaults_for_dataclass
};

@include@ non-member_defaults_for_dataclass
@include@ set_class_for_dataclass

*/

class DatabaseDescriptor	//# ClassName
{
public:
	QString name;		//# KeyField	// will be the connection name
	QString driver;		//# Field		// for example QSQLITE
	QString databaseName;	//# Field	// file name in case of sqlite
	QString username;	//# Field
	QString password;	//# Field
	bool savePassword;	//# Field
	QString host;		//# Field		// host name or ip
	unsigned port;		//# Field

	//# include defaults_for_dataclass

};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

/*
@BeginDecl@
	Class Tag
	Fields {
		Text name; key			; TEXT
		Text description;		; TEXT NOT NULL DEFAULT ''
		// non-editable
		DateTime lastModified;	; TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP
		bool deleted;			; CHAR(1) NOT NULL DEFAULT 'N'
		}
	Constraints {
		PRIMARY KEY (name)
		}
@EndDecl@

class Tag
{
	@include@ defaults_for_dataclass
};

@include@ non-member_defaults_for_dataclass
@include@ set_class_for_dataclass

*/

class Tag			//# ClassName
{
public:
	Text name;		//# KeyField
	Text description;	//# Field
	DateTime lastModified;	//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Ware			//# ClassName
{
public:
	Text name;		//# KeyField
	Text unit;		//# Field
	TypeNameSet types;	//# Field
	TagNameSet tags;	//# Field
	DateTime lastModified;	//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Company			//# ClassName
{
public:
	Text name;		//# KeyField		// Tesco Global Áruházak ZRt.
	Text country;		//# Field		// Magyarország
	Text city;		//# Field	// Budaörs
	Text postalCode;	//# Field		// 2040
	Text address;		//# Field		// Kinizsi út 1-3.
	Text taxId;		//# Field	// 10307078-2-44
	Text icon;		//# Field	// base64 representation of an image
	DateTime lastModified;	//# Field	// non editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Brand			//# ClassName
{
public:
	Text name;		//# KeyField
	Text company;		//# Field
	DateTime lastModified;	//# Field	// non editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Inventory			//# ClassName
{
public:
	Text name;		//# KeyField
	Text comment;		//# Field
	DateTime lastModified;	//# Field	// non editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Item			//# ClassName
{
public:
	DateTime uploadDate;	//# KeyField	// non editable
	Text name;		//# Field
	Text unit;		//# Field
	Text type;		//# Field
	Text brand;		//# Field
	double quantity;	//# Field		// amoutn to buy or not
	double price;		//# Field		// price of gross piece/amount quantity
	Text currency;		//# Field
	Text account;		//# Field
	Text partner;		//# Field
	Text inventory;		//# Field
	Text comment;		//# Field
	DateTime invChangeDate;	//# Field
	DateTime lastModified;	//# Field	// non editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Partner			//# ClassName
{
public:
	Text name;		//# KeyField		// Kertvárosi tesco
	Text country;		//# Field		// Magyarország
	Text city;		//# Field	// Pécs
	Text postalCode;	//# Field		// 7631
	Text address;		//# Field		// Kincses út 1.
	Text company;		//# Field		// Tesco Global Áruházak Zrt.
	Text storeName;		//# Field		// 41052 számú bolt
	DateTime lastModified;	//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Account			//# ClassName
{
public:
	Text name;		//# KeyField
	Text currency;		//# Field
	Text bankOffice;	//# Field
	Text iban;		//# Field
	Text swiftCode;		//# Field
	DateTime lastModified;	//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Payment			//# ClassName
{
public:
	DateTime uploadDate;	//# KeyField
	Text account;		//# Field
	Text partner;		//# Field
	double amount;		//# Field
	DateTime subject;	//# Field
	DateTime payDate;	//# Field
	DateTime lastModified;	//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

class Query				//# ClassName
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

public:
	Text name;			//# KeyField
	DateTime startDate;		//# Field
	DateTime endDate;		//# Field
	enum StockOptions stockOption;	//# Field
	enum TagOptions tagOption;	//# Field
	TagNameSet withTags;		//# Field
	TagNameSet withoutTags;		//# Field
	WareNameSet wares;		//# Field
	PartnerNameSet partners;	//# Field
	DateTime lastModified;		//# Field	// non-editable

	//# include defaults_for_dataclass
};

//# include non-member_defaults_for_dataclass
//# include set_class_for_dataclass

#endif
