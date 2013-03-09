/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASES_H
#define BUTLER_DATABASES_H

#include <csjp_exception.h>

#include <butler_database_descriptor.h>

#include <butler_db.h>
#include <butler_itemsmodel.h>
#include <butler_custommodel.h>
#include <butler_shoppingmodel.h>
#include <butler_stockmodel.h>
#include <butler_tagsmodel.h>
#include <butler_shopsmodel.h>
#include <butler_waresmodel.h>
#include <butler_queriesmodel.h>

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

private:
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

public:
	CustomModel & customItems()
	{
		if(!customModel){
			customModel = new CustomModel(db());
		}
		return *customModel;
	}

	ShoppingModel & shoppingItems()
	{
		if(!shoppingModel){
			shoppingModel = new ShoppingModel(db());
		}
		return *shoppingModel;
	}

	StockModel & stockItems()
	{
		if(!stockModel){
			stockModel = new StockModel(db());
		}
		return *stockModel;
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

	QueriesModel & queries()
	{
		if(!queriesModel){
			queriesModel = new QueriesModel(db());
		}
		return *queriesModel;
	}

public:
	DatabaseDescriptor desc;

private:
	Db * database;
	CustomModel * customModel;
	ShoppingModel * shoppingModel;
	StockModel * stockModel;
	TagsModel * tagsModel;
	ShopsModel * shopsModel;
	WaresModel * waresModel;
	QueriesModel * queriesModel;

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

inline CustomModel & customModel(const QString dbname)
{
	return databases.query(dbname).customItems();
}

inline ShoppingModel & shoppingModel(const QString dbname)
{
	return databases.query(dbname).shoppingItems();
}

inline StockModel & stockModel(const QString dbname)
{
	return databases.query(dbname).stockItems();
}

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
