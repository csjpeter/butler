/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_shop_set.h"

int ShopSet::compare(const Shop &a, const Shop &b) const
{
	bool ret;

	switch(ordering) {
		case Shop::Name :
			ret = QString::localeAwareCompare(a.name, b.name) < 0;
			break;
		case Shop::StoreName :
			ret = QString::localeAwareCompare(a.storeName, b.storeName) < 0;
			break;
		case Shop::City :
			ret = QString::localeAwareCompare(a.city, b.city) < 0;
			break;
		case Shop::Address :
			ret = QString::localeAwareCompare(a.address, b.address) < 0;
			break;
		case Shop::Company :
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
