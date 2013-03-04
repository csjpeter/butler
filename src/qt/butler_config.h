/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_CONFIG_H
#define BUTLER_CONFIG_H

/* Forwards: */
class QSessionManager;

namespace Butler {

class Config
{
public:
	static Config& instance();
	virtual ~Config();

protected:
	Config();
public:
	void save();
	void load();

	const QString & dateTimeFormat();

private:
	QString _dateTimeFormat;
};

}

#endif


