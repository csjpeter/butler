/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <csjp_logger.h>

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
	QString iconPath;
#ifdef RELATIVE_PATH
	iconPath += rootDir;
#endif
	iconPath.append(ICONS_PATH);
	iconPath.append(fileName);
	DBG("Icon path: %s", C_STR(iconPath));
	return iconPath;
}

const QString css(const char * fileName)
{
	QString cssPath;
#ifdef RELATIVE_PATH
	cssPath += rootDir;
#endif
	cssPath.append(CSS_PATH);
	cssPath.append(fileName);
	DBG("CSS path: %s", C_STR(cssPath));
	return cssPath;
}

void initRootPath(const char * args0)
{
	QString prefix(PREFIX);
	int rootDepth = prefix.count("/") + 1; /* +1 for bin */

	QString root(QDir::fromNativeSeparators(args0));
	DBG("args0: %s", C_STR(root));

	int pos = root.lastIndexOf("/");
	root.chop(root.size() - pos - 1);

	for(int i = 0; i < rootDepth; i++)
		root.append("../");

	rootDir = QDir::cleanPath(root) + "/";

	DBG("root: %s", C_STR(rootDir));
	DBG("QDir::homePath(): %s", C_STR(QDir::homePath()));
}

}
