/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <csjp_logger.h>
#include <csjp_object.h>

#include "config.h"
#include "butler_config.h"
#include "butler_macros.h"

#include <butler_texts.h>

static QString _dateTimeFormat;
static QString rootDir;
QLocale locale;

namespace Config {

void save()
{
}

void load()
{
}

void loadTranslation(const char * langCode)
{
	QString lang(langCode);
	if(!lang.length())
		lang = "en" ; //locale.name();
	lang.truncate(2);
	QString trFile(Path::translation(C_STR(lang)));
	DBG("Translation file to load: %s", C_STR(trFile));

	csjp::Object<QTranslator> translator(new QTranslator);
	if(!translator->load(trFile))
		throw csjp::ResourceError("Can not load language file %s", C_STR(trFile));

	qApp->installTranslator(translator.release());
}

const QString & dateTimeFormat()
{
	if (!_dateTimeFormat.size())
		_dateTimeFormat = "yyyy-MM-dd ddd hh:mm:ss";
	return _dateTimeFormat;
}

}

namespace Path {

const QString translation(const char * lang)
{
	QString path;
#ifdef RELATIVE_PATH
	path += rootDir;
#endif
	path.append(TRANSLATIONS_PATH);
	DBG("Translation path: %s", C_STR(path));
	path += lang;
	path += ".qm";
	return path;
}

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
