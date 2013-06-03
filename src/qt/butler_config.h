/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONFIG_H
#define BUTLER_CONFIG_H

#include <QLocale>
#include <QString>

#include <butler_texts.h>
#include <butler_macros.h>

namespace Config {
	extern QString dbFileName;
	extern QString defaultDbName;
	extern QLocale locale;
	extern double pxPerMM;
	extern double scaleFactor;
	extern int thresholdScrollDistance; /* Kinetic scroll, threshold for scroll. */
	extern QString psqlHost;
	extern QString psqlUsername;
	extern QString psqlPassword;
	extern unsigned psqlPort;

	void save();
	void load();

	const QString & dateTimeFormat();
}

namespace Path {
	/* Absolute path and name of the binary expected,
	 * ending as prefix/bin/binary_name. */
	void initRootPath(const char * args0);

	const QString translation(const char * lang);
	const QString icon(const char * fileName);
	const QString css(const char * fileName);
}

#endif
