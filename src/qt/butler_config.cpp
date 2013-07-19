/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <csjp_logger.h>
#include <csjp_object.h>
#include <csjp_json.h>
#include <csjp_file.h>

#include "config.h"
#include "butler_config.h"
#include "butler_macros.h"

#include <butler_texts.h>

static QString _dateTimeFormat;
static QString rootDir;

csjp::ObjectTree config;

namespace Config {

QString configFileName;
QString defaultDbName = "localdb";
QLocale locale;
double pxPerMM = 0;
double scaleFactor = 1;
int thresholdScrollDistance = 0;

void save()
{
	QString prefix("Config");
	QSettings settings;

	settings.setValue(prefix + "/locale", locale.bcp47Name());

	config.set("defaultDb", C_STR(Config::defaultDbName));

	/* JSON */
	csjp::Json json;
	json <<= config;
	csjp::File file(C_STR(configFileName));
	file.overWrite(json.data);
}

void load()
{
	QString prefix("Config");
	QSettings settings;

	QString l = settings.value(prefix + "/locale", "").toString();
	if(l.size())
		locale = QLocale(l);

	/* JSON */
	csjp::Json json;
	csjp::File file(C_STR(configFileName));
	if(file.exists()){
		json.data = file.readAll();
		config <<= json;
	}

	Config::defaultDbName <<= config.get("defaultDb", csjp::String(C_STR(Config::defaultDbName)));
}

const QString & dateTimeFormat()
{
	if (!_dateTimeFormat.size())
		_dateTimeFormat = "yyyy-MM-dd ddd hh:mm:ss";
	return _dateTimeFormat;
}

}

namespace Path {
/*
	app.addLibraryPath("/data/data/org.kde.necessitas.ministro/files");
void initLibPath(QString path)
{
	QDir dir(path);
	QDir::entryList(QDir::AllDirs);
	LOG();
	LOG("QT_PLUGIN_PATH=%s",
			C_STR(QProcessEnvironment::systemEnvironment().value("QT_PLUGIN_PATH")));

}
*/
const QString translation(const char * lang)
{
	QString path;
#ifdef RELATIVE_PATH
	path += rootDir;
#endif
	path.append(TRANSLATIONS_PATH);
	LOG("Translation path: %s", C_STR(path));
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
	LOG("Icon path: %s", C_STR(iconPath));
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
	LOG("CSS path: %s", C_STR(cssPath));
	return cssPath;
}

void initRootPath(const char * args0)
{
	QString prefix(PREFIX);
	int rootDepth = prefix.count("/") + 1; /* +1 for bin */

	QString root(QDir::fromNativeSeparators(args0));
	LOG("args0: %s", C_STR(root));

	int pos = root.lastIndexOf("/");
	root.chop(root.size() - pos - 1);

	for(int i = 0; i < rootDepth; i++)
		root.append("../");

	rootDir = QDir::cleanPath(root) + "/";

	LOG("root: %s", C_STR(rootDir));
	LOG("QDir::homePath(): %s", C_STR(QDir::homePath()));
}

void initConfigFileName()
{
	QDir dir(QDir::homePath());

	if(!dir.exists(".butler"))
		dir.mkdir(".butler");
	Config::configFileName = QDir::toNativeSeparators(
			QDir::homePath() + QString("/.butler/config.json")
			);
}

}
