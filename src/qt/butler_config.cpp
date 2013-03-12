/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"

Config& Config::instance()
{
	static Config _instance;
	return _instance;
}

Config::Config() :
	_dateTimeFormat("yyyy-MM-dd ddd hh:mm:ss")
{
}

Config::~Config()
{
}

void Config::save()
{
}

void Config::load()
{
}

const QString &Config::dateTimeFormat()
{
	return _dateTimeFormat;
}
