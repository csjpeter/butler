/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_tag_set.h"

int TagSet::compare(const Tag &a, const Tag &b) const
{
	bool ret;

	switch(ordering) {
		case Tag::Name :
			ret = QString::localeAwareCompare(a.name, b.name) < 0;
			break;
		case Tag::Description :
			ret = QString::localeAwareCompare(a.description, b.description) < 0;
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
