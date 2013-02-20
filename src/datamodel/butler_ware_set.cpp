/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "csjp_debug.h"

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
	
//	MSG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

	if(!ascending)
		ret = !ret;

	return ret ? -1 : 1;
}

