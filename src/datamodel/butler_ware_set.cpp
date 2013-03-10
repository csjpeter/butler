/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_ware_set.h"

int WareSet::compare(const Ware &a, const Ware &b) const
{
	bool ret;

	switch(ordering) {
		case Ware::Name :
			ret = QString::localeAwareCompare(a.name, b.name) < 0;
			break;
		case Ware::Unit :
			ret = QString::localeAwareCompare(a.unit, b.unit) < 0;
			break;
		case Ware::Categories :
			ret = a.categories.size() < b.categories.size();
			break;
		case Ware::Tags :
			ret = a.tags.size() < b.tags.size();
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
