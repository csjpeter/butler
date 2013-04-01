/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONFIG_H
#define BUTLER_CONFIG_H

extern QLocale locale;

namespace Config {
	void save();
	void load();

	const QString & dateTimeFormat();
}

namespace Path {
	/* Absolute path and name of the binary expected,
	 * ending as prefix/bin/binary_name. */
	void initRootPath(const char * args0);

	const QString icon(const char * fileName);
	const QString css(const char * fileName);
}

#endif
