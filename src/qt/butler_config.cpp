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

namespace Config {

QString dbFileName;
QString defaultDbName;
QLocale locale;
double pxPerMM = 0;
double scaleFactor = 1;
int thresholdScrollDistance = 0;
QString psqlHost;
QString psqlButlerDbName;
QString psqlUsername;
QString psqlPassword;
unsigned psqlPort = 0;

void save()
{
	QString prefix("Config");
	QSettings settings;

	settings.setValue(prefix + "/locale", locale.bcp47Name());

	settings.setValue(prefix + "/dbfile", dbFileName);

	settings.setValue(prefix + "/psql-host", psqlHost);
	settings.setValue(prefix + "/psql-dbname", psqlButlerDbName);
	settings.setValue(prefix + "/psql-user", psqlUsername);
	settings.setValue(prefix + "/psql-pwd", psqlPassword);
	settings.setValue(prefix + "/psql-port", psqlPort);

	settings.setValue(prefix + "/defaultDbName", defaultDbName);
}

void load()
{
	QString prefix("Config");
	QSettings settings;

	QString l = settings.value(prefix + "/locale", "").toString();
	if(l.size())
		locale = QLocale(l);

	if(dbFileName.isEmpty())
		dbFileName = settings.value(prefix + "/dbfile", QString()).toString();
	if(dbFileName.isEmpty()){
		QDir dir(QDir::homePath());

		if(!dir.exists(".butler"))
			dir.mkdir(".butler");
		Config::dbFileName = QDir::toNativeSeparators(
				QDir::homePath() + QString("/.butler/db.sqlite")
				);
	}

	psqlHost = settings.value(prefix + "/psql-host", QString()).toString();
	if(psqlHost.isEmpty())
		psqlHost = "localhost";

	psqlButlerDbName = settings.value(prefix + "/psql-dbname", QString()).toString();
	if(psqlButlerDbName.isEmpty())
		psqlButlerDbName = "butler-db";

	psqlUsername = settings.value(prefix + "/psql-user", QString()).toString();
	if(psqlUsername.isEmpty())
		psqlUsername = "username";

	psqlPassword = settings.value(prefix + "/psql-pwd", QString()).toString();
	if(psqlPassword.isEmpty())
		psqlPassword = "password";

	psqlPort = settings.value(prefix + "/psql-port", QString()).toInt();
	if(psqlPort < 1)
		psqlPort = 5432;

	defaultDbName = settings.value(prefix + "/defaultDbName", QString()).toString();
	if(defaultDbName.isEmpty())
		defaultDbName = "localdb";
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

}
