/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <QtGui>

#include <butler_datamodel.h>

SCC TidTagFieldName			= QT_TRANSLATE_NOOP("TagModel", "Name");
SCC TidTagFieldDescription	= QT_TRANSLATE_NOOP("TagModel", "Description");
SCC TidTagFieldLastModified	= QT_TRANSLATE_NOOP("TagModel", "Last modification date");
SCC TidTagFieldDeleted		= QT_TRANSLATE_NOOP("TagModel", "Is deleted");

@declare@ Tag
@include@ datamodel.cpp

