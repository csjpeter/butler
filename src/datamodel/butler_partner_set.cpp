/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_partner_set.h"

int PartnerSet::compare(const Partner &a, const Partner &b) const
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
