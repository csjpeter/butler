/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

/*#include <iostream>*/

#include <QSessionManager>
#include <QMessageBox>
#include <QDir>

#include "butler_broadcaster.h"

Broadcaster& Broadcaster::instance()
{
	static Broadcaster _instance;
	return _instance;
}

Broadcaster::Broadcaster()
{
}

Broadcaster::~Broadcaster()
{
}
