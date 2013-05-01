/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_company_set.h"

int CompanySet::compare(const Company &a, const Company &b) const
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
