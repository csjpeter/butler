/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONFIG_H
#define BUTLER_CONFIG_H

#include <QLocale>
#include <QString>
#include <csjp_object_tree.h>

#include <butler_texts.h>
#include <butler_macros.h>
#include <butler_conversions.h>

extern csjp::ObjectTree config;

namespace Config {
	extern QString configFileName;
	extern QString defaultDbName;
	extern QLocale locale;
	extern double pxPerMM;
	extern double scaleFactor;
	extern int thresholdScrollDistance; /* Kinetic scroll, threshold for scroll. */

	void save();
	void load();

	const QString & dateTimeFormat();
}

namespace Path {
	/* Absolute path and name of the binary expected,
	 * ending as prefix/bin/binary_name. */
	void initRootPath(const char * args0);

	void initConfigFileName();

	const QString translation(const char * lang);
	const QString icon(const char * fileName);
	const QString css(const char * fileName);
}

#endif
