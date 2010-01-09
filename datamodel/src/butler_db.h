/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DB_H
#define BUTLER_DB_H

#include <QObject>
#include <QDate>
#include <QString>
#include <QList>
#include <QVariant>

#include "butler_item_db.h"
#include "butler_tag_db.h"
#include "butler_query_db.h"

namespace Butler {

	class Db : public ItemDb, public TagDb, public QueryDb
	{
		public:
			Db(){}
			virtual ~Db(){}

			virtual bool connect() = 0;
			virtual bool open() = 0;
			virtual bool close() = 0;
			virtual bool create() = 0;
			virtual bool check() = 0;
			virtual bool update() = 0;
			virtual const QString& lastError() = 0;
	};

}

#endif

