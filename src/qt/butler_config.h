/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONFIG_H
#define BUTLER_CONFIG_H

#include <QLocale>
#include <QString>

namespace Config {
	enum class ToolBarPosition
	{
		Top = 0,
		Bottom,
		Left,
		Right
	};

	extern QString dbFileName;
	extern const char * defaultDbName;
	extern QLocale locale;
	extern double pxPerMM;
	extern double scaleFactor;
	extern int thresholdScrollDistance; /* Kinetic scroll, threshold for scroll. */
	extern ToolBarPosition toolBarPosition;

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
