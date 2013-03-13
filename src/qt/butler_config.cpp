/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#define DEBUG

#include "csjp_exception.h"
#include "csjp_logger.h"

#include "config.h"
#include "butler_config.h"
#include "butler_macros.h"

static QString _dateTimeFormat;
static QString rootDir;

namespace Config {

void save()
{
}

void load()
{
}

const QString & dateTimeFormat()
{
	if (!_dateTimeFormat.size())
		_dateTimeFormat = "yyyy-MM-dd ddd hh:mm:ss";
	return _dateTimeFormat;
}

}

namespace Path {

const QString icon(const char * fileName)
{
	QString iconPath(rootDir);
	iconPath.append(ICONS_PATH);
	iconPath.append(fileName);
	DBG("Icon path: %s", C_STR(iconPath));
	return iconPath;
}

void initRootPath(const char * args0)
{
	QString prefix(PREFIX);
	int rootDepth = prefix.count("/") + 1;

	QString root(args0);
	root.replace("\\", "/");
	DBG("args0: %s", C_STR(root));

	int pos = root.lastIndexOf("/");
	root.chop(root.size() - pos - 1);
	DBG("args0 without binary name: %s", C_STR(root));

	for(int i = 0; i < rootDepth; i++)
		root.append("../");
	DBG("root: %s", C_STR(root));

	rootDir = root;
}

}
