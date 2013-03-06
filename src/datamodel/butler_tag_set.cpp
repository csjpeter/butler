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
