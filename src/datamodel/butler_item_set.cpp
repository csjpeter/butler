/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include "butler_item.h"
#include "butler_item_set.h"

int ItemSet::compare(const Item &a, const Item &b) const
{
	bool ret;

	switch(ordering) {
		case Item::Uploaded :
			ret = a.uploaded < b.uploaded;
			break;
		case Item::Name :
			ret = QString::localeAwareCompare(a.name, b.name) < 0;
			break;
		case Item::Category :
			ret = QString::localeAwareCompare(a.category, b.category) < 0;
			break;
		case Item::Quantity :
			ret = a.quantity < b.quantity;
			break;
		case Item::Comment :
			ret = QString::localeAwareCompare(a.comment, b.comment) < 0;
			break;
		case Item::Bought :
			ret = a.bought < b.bought;
			break;
		case Item::Purchased :
			ret = a.purchased < b.purchased;
			break;
		case Item::Shop :
			ret = QString::localeAwareCompare(a.shop, b.shop) < 0;
			break;
		case Item::Price :
			ret = a.price < b.price;
			break;
		case Item::OnStock :
			ret = a.onStock < b.onStock;
			break;
		case Item::UnitPrice :
			{
			double aVal = 0, bVal = 0;
			if(DBL_EPSILON <= a.quantity)
				aVal = a.price / a.quantity;
			if(DBL_EPSILON <= b.quantity)
				bVal = b.price / b.quantity;
			ret = aVal < bVal;
			}
			break;
		default:
			ret = a.uploaded < b.uploaded;
			break;
	}
	
//	MSG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

	if(!ascending)
		ret = !ret;

	return ret ? -1 : 1;
}
