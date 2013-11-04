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


typedef csjp::OwnerContainer<Text> TagNameSet;

class CategoryNameSet : public csjp::SorterOwnerContainer<Text>
{
public:
		CategoryNameSet() : csjp::SorterOwnerContainer<Text>(), ascending(true){}
		CategoryNameSet(const CategoryNameSet & cns) :
				csjp::SorterOwnerContainer<Text>(cns),
				ascending(cns.ascending)
		{}
		CategoryNameSet(CategoryNameSet && temp) :
				csjp::SorterOwnerContainer<Text>(csjp::move_cast(temp)),
				ascending(temp.ascending)
		{
				temp.ascending = true;
		}
		~CategoryNameSet() {}

		const CategoryNameSet & operator=(CategoryNameSet && temp)
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

typedef csjp::OwnerContainer<Text> WareNameSet;
typedef csjp::OwnerContainer<Text> CompanyNameSet;
typedef csjp::OwnerContainer<Text> PartnerNameSet;

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


#include <../tmp/src/butler_dataclasses>

#endif
