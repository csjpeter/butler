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

#include <butler_texts.h>

static QString _dateTimeFormat;
static QString rootDir;

csjp::Json config;

namespace Config {

csjp::String configFileName;
csjp::String defaultDbName("localdb");
QLocale locale;
double pxPerMM = 0;
double scaleFactor = 1;
int thresholdScrollDistance = 0;

void save()
{
	QString prefix("Config");
	QSettings settings;

	settings.setValue(prefix + "/locale", locale.bcp47Name());

	config["defaultDb"] = Config::defaultDbName;

	csjp::File file(configFileName);
	file.overWrite(config.toString());
}

void load()
{
	QString prefix("Config");
	QSettings settings;

	QString l = settings.value(prefix + "/locale", "").toString();
	if(l.size())
		locale = QLocale(l);

	csjp::File file(configFileName);
	if(file.exists())
		config <<= file.readAll();

	if(config.properties.has("defaultDb"))
		Config::defaultDbName = config["defaultDb"];
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
	LOG("QT_PLUGIN_PATH=", QProcessEnvironment::systemEnvironment().value("QT_PLUGIN_PATH"));

}
*/
const QString translation(const char * lang)
{
	QString path;
#ifdef RELATIVE_PATH
	path += rootDir;
#endif
	path.append(TRANSLATIONS_PATH);
	LOG("Translation path: ", path);
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
	LOG("Icon path: ", iconPath);
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
	LOG("CSS path: ", cssPath);
	return cssPath;
}

void initRootPath(const char * args0)
{
	LOG("args0: ", args0);
	csjp::String cmd(C_STR(QDir::fromNativeSeparators(args0)));

	csjp::String prefix(PREFIX);
	LOG("Compile time prefix: ", prefix);

	csjp::String current(C_STR(QDir::currentPath()));
	LOG("Current path: %", current);

	csjp::String path;
	if(cmd.startsWith("/"))
		path << cmd;
	else if(!cmd.count("/"))
		path << "/" << prefix << "bin/" << cmd;
	else
		path << current << "/" << cmd;
	path <<= QDir::cleanPath(QString(path.str));
	csjp::unint pos;
	path.findLastOf(pos, "/");
	path.chopBack(path.length - pos);
	LOG("Computed binary path: %", path);

	csjp::String root(path);
	root.findLastOf(pos, "/");
	root.chopBack(root.length - pos);
	root.findLastOf(pos, "/");
	root.chopBack(root.length - pos);
	root << "/";

	rootDir = root.str;
/*
	int rootDepth = path.count("/");

	for(int i = 0; i < rootDepth; i++)
		rootDir.append("../");
	rootDir.append(path.str);
	rootDir.append("/");
*/
	LOG("rootDir: %", rootDir);
	LOG("QDir::homePath(): %", QDir::homePath());
}

void initConfigFileName()
{
	QDir dir(QDir::homePath());

	if(!dir.exists(".butler"))
		dir.mkdir(".butler");
	Config::configFileName <<= QDir::toNativeSeparators(
			QDir::homePath() + QString("/.butler/config.json")
			);
}

}
