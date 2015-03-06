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

@declare@ Tag

class Tag
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h
@include@ dbclass.h

@declare@ Ware

class Ware
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Company

class Company
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Brand

class Brand
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Inventory

class Inventory
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Item

class Item
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Partner

class Partner
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Account

class Account
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Payment

class Payment
{
	@include@ dataclass_members.h
};

@include@ dataclass_nonmembers.h
@include@ dataclass_set.h

@declare@ Query

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
