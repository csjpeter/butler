/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include "butler_database_descriptor.h"
#include "butler_database_descriptor_set.h"

int DatabaseDescriptorSet::compare(const DatabaseDescriptor &a, const DatabaseDescriptor &b) const
{
	bool ret;

	switch(ordering) {
		case DatabaseDescriptor::Name :
			ret = QString::localeAwareCompare(a.name, b.name) < 0;
			break;
		case DatabaseDescriptor::Driver :
			ret = QString::localeAwareCompare(a.driver, b.driver) < 0;
			break;
		case DatabaseDescriptor::DatabaseName :
			ret = a.databaseName < b.databaseName;
			break;
		case DatabaseDescriptor::Username :
			ret = QString::localeAwareCompare(a.username, b.username) < 0;
			break;
		case DatabaseDescriptor::Password :
			ret = a.password < b.password;
			break;
		case DatabaseDescriptor::Host :
			ret = a.host < b.host;
			break;
		case DatabaseDescriptor::Port :
			ret = a.port < b.port;
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
