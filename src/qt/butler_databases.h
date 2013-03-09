/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASES_H
#define BUTLER_DATABASES_H

#include <csjp_exception.h>

#include <butler_database_descriptor.h>

#include <butler_db.h>
#include <butler_tagsmodel.h>
#include <butler_shopsmodel.h>
#include <butler_waresmodel.h>

class Database
{
public:
	Database() :
		database(0),
		tagsModel(0),
		shopsModel(0),
		waresModel(0)
	{}

	explicit Database(const Database &) = delete;
	~Database()
	{
		delete tagsModel;
		delete shopsModel;
		delete waresModel;
		delete database;
	}

	Database& operator=(const Database &) = delete;

	bool isEqual(const Database &dm) const { return desc == dm.desc; }

	bool isLess(const Database &dm) const { return desc < dm.desc; }
	
	bool isLess(const QString &s) const { return desc < s; }
	bool isMore(const QString &s) const { return desc.isMore(s); }

	Db & db()
	{
		if(database)
			return *database;

		if(desc.driver == "QSQLITE") {
			database = new Db(desc);
			//if(!QFile::exists(filepath))
		} else
			throw DbError("Driver '%s' is not yet supported.", C_STR(desc.driver));

		return *database;
	}

	TagsModel & tags()
	{
		if(!tagsModel){
			tagsModel = new TagsModel(db());
		}
		return *tagsModel;
	}

	ShopsModel & shops()
	{
		if(!shopsModel){
			shopsModel = new ShopsModel(db());
		}
		return *shopsModel;
	}

	WaresModel & wares()
	{
		if(!waresModel){
			waresModel = new WaresModel(db());
		}
		return *waresModel;
	}

public:
	DatabaseDescriptor desc;

private:
	Db * database;
	TagsModel * tagsModel;
	ShopsModel * shopsModel;
	WaresModel * waresModel;

private:
	void equal(const Database &tag);
};

inline bool operator==(const Database &a, const Database &b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Database &a, const Database &b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Database &a, const Database &b)
{
	return a.isLess(b);
}

inline bool operator<(const QString &a, const Database &b)
{
	return b.isMore(a);
}

inline bool operator<(const Database &a, const QString &b)
{
	return a.isLess(b);
}

extern csjp::OwnerContainer<Database> databases;

inline TagsModel & tagsModel(const QString dbname)
{
	return databases.query(dbname).tags();
}

inline ShopsModel & shopsModel(const QString dbname)
{
	return databases.query(dbname).shops();
}

inline WaresModel & waresModel(const QString dbname)
{
	return databases.query(dbname).wares();
}

#endif
