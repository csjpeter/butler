/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_databases.h"

class Database
{
public:
	Database(csjp::Object<DatabaseDescriptor> & dd) :
		desc(0),
		database(0),
		shoppingModel(0),
		stockModel(0),
		tagsModel(0),
		shopsModel(0),
		waresModel(0),
		queriesModel(0)
	{
		desc = dd.release();
		ENSURE(desc, csjp::LogicError);
	}

	explicit Database(const Database &) = delete;
	~Database()
	{
		delete queriesModel;
		delete shoppingModel;
		delete stockModel;
		delete tagsModel;
		delete shopsModel;
		delete waresModel;
		delete database;
		delete desc;
	}

	Database& operator=(const Database &) = delete;

	bool isEqual(const Database &dm) const { return *desc == *dm.desc; }

	bool isLess(const Database &dm) const { return *desc < *dm.desc; }
	
	bool isLess(const QString &s) const { return *desc < s; }
	bool isMore(const QString &s) const { return desc->isMore(s); }

private:
	Db & db()
	{
		if(database)
			return *database;

		if(desc->driver == "QSQLITE") {
			database = new Db(*desc);
			//if(!QFile::exists(filepath))
		} else
			throw DbError("Driver '%s' is not yet supported.", C_STR(desc->driver));

		return *database;
	}

public:
	const DatabaseDescriptor & databaseDescriptor()
	{
		return *desc;
	}

	csjp::Object<CustomModel> customItems()
	{
		/* Each custom view shall have its own special custom model. */
		return csjp::Object<CustomModel>(new CustomModel(db(), wares()));
	}

	ShoppingModel & shoppingItems()
	{
		if(!shoppingModel)
			shoppingModel = new ShoppingModel(db(), wares());
		return *shoppingModel;
	}

	StockModel & stockItems()
	{
		if(!stockModel)
			stockModel = new StockModel(db(), wares());
		return *stockModel;
	}

	TagsModel & tags()
	{
		if(!tagsModel)
			tagsModel = new TagsModel(db());
		return *tagsModel;
	}

	ShopsModel & shops()
	{
		if(!shopsModel)
			shopsModel = new ShopsModel(db());
		return *shopsModel;
	}

	WaresModel & wares()
	{
		if(!waresModel)
			waresModel = new WaresModel(db());
		return *waresModel;
	}

	QueriesModel & queries()
	{
		if(!queriesModel)
			queriesModel = new QueriesModel(db());
		return *queriesModel;
	}

private:
	DatabaseDescriptor * desc;
	Db * database;
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



static csjp::OwnerContainer<Database> databases;



void registerDatabase(csjp::Object<DatabaseDescriptor> & desc)
{
	csjp::Object<Database> db(new Database(desc));
	databases.add(db);
}

csjp::Object<CustomModel> customModel(const QString & dbname)
{
	return databases.query(dbname).customItems();
}

const DatabaseDescriptor & databaseDescriptor(const QString & dbname)
{
	return databases.query(dbname).databaseDescriptor();
}

ShoppingModel & shoppingModel(const QString & dbname)
{
	return databases.query(dbname).shoppingItems();
}

StockModel & stockModel(const QString & dbname)
{
	return databases.query(dbname).stockItems();
}

TagsModel & tagsModel(const QString & dbname)
{
	return databases.query(dbname).tags();
}

ShopsModel & shopsModel(const QString & dbname)
{
	return databases.query(dbname).shops();
}

WaresModel & waresModel(const QString & dbname)
{
	return databases.query(dbname).wares();
}

QueriesModel & queriesModel(const QString & dbname)
{
	return databases.query(dbname).queries();
}
