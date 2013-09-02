/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <csjp_map.h>
#include "butler_databases.h"
#include "butler_database_descriptor.h"

#include <QDir>

DatabaseDescriptorSet descriptorSet;

class Database
{
public:
	Database(const QString & dbname) :
		dbname(dbname),
		database(0),
		shoppingModel(0),
		tagsModel(0),
		partnersModel(0),
		companyModel(0),
		waresModel(0),
		queriesModel(0)
	{
	}

	explicit Database(const Database &) = delete;
	~Database()
	{
		delete queriesModel;
		delete shoppingModel;
		delete tagsModel;
		delete partnersModel;
		delete companyModel;
		delete waresModel;
		delete database;
	}

	Database& operator=(const Database &) = delete;

	bool isEqual(const Database & dm) const { return dbname == dm.dbname; }

	bool isLess(const Database & dm) const { return dbname < dm.dbname; }
	
	bool isLess(const QString & s) const { return dbname < s; }
	bool isMore(const QString & s) const { return s < dbname; }

private:
	Db & db()
	{
		if(!database)
			database = new Db(descriptorSet.query(dbname));
		if(!database->isOpen())
			database->desc = descriptorSet.query(dbname);
		return *database;
	}

public:
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

	TagsModel & tags()
	{
		if(!tagsModel)
			tagsModel = new TagsModel(db());
		return *tagsModel;
	}

	PartnersModel & partners()
	{
		if(!partnersModel)
			partnersModel = new PartnersModel(db());
		return *partnersModel;
	}

	CompanyModel & company()
	{
		if(!companyModel)
			companyModel = new CompanyModel(db());
		return *companyModel;
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
	QString dbname;
	Db * database;
	ShoppingModel * shoppingModel;
	TagsModel * tagsModel;
	PartnersModel * partnersModel;
	CompanyModel * companyModel;
	WaresModel * waresModel;
	QueriesModel * queriesModel;

private:
	void equal(const Database & tag);
};

inline bool operator==(const Database & a, const Database & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Database & a, const Database & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Database & a, const Database & b)
{
	return a.isLess(b);
}

inline bool operator<(const QString & a, const Database & b)
{
	return b.isMore(a);
}

inline bool operator<(const Database & a, const QString & b)
{
	return a.isLess(b);
}



csjp::OwnerContainer<Database> databases;



QString defaultSQLiteDbFileName()
{
	QDir dir(QDir::homePath());
	if(!dir.exists(".butler"))
		dir.mkdir(".butler");
	return QDir::toNativeSeparators( QDir::homePath() + QString("/.butler/db.sqlite") );
}

void loadDatabaseConfigs()
{
	descriptorSet.clear();
	csjp::ObjectTree & tree = config["database-connections"];
	unsigned s = tree.objects.size();
	for(unsigned i = 0; i < s; i++){
		csjp::Object<DatabaseDescriptor> desc(new DatabaseDescriptor);
		csjp::ObjectTree & dbt = tree.objects.queryAt(i);
		desc->name <<= dbt.name;
		csjp::Map<csjp::String, csjp::String> & props = dbt.properties;
		desc->driver <<= props["driver"];
		desc->databaseName <<= props["databaseName"];
		desc->username <<= props["username"];
		desc->password <<= props["password"];
		if(!desc->password.isEmpty())
			desc->savePassword = true;
		desc->host <<= props["host"];
		desc->port <<= props["port"];
		descriptorSet.add(desc);
	}

	/* Init the local database if it is not defined already */
	if(!descriptorSet.has("localdb")){
		csjp::Object<DatabaseDescriptor> sqlitedb(new DatabaseDescriptor);
		sqlitedb->name = "localdb";
		sqlitedb->driver = "QSQLITE";
		sqlitedb->databaseName = defaultSQLiteDbFileName();
		LOG("Sqlite db file path: %s", C_STR(sqlitedb->databaseName));
		descriptorSet.add(sqlitedb);
	}

	/* Init default postgre database */
//echo "\\d" | psql -h csjpeter.dyndns.org -p 5432 -d butler
//psql -h csjpeter.dyndns.org -p 5432
//echo "CREATE ROLE csjpeter LOGIN PASSWORD 'password' VALID UNTIL 'infinity';" | sudo -i -u postgres psql
//echo "SELECT rolname FROM pg_roles;" | sudo -i -u postgres psql
//echo "CREATE DATABASE csjpeter WITH ENCODING='UTF8' OWNER=csjpeter;" | sudo -i -u postgres psql
/*	if(!descriptorSet.has("postgredb")){
		csjp::Object<DatabaseDescriptor> dbDesc(new DatabaseDescriptor);
		dbDesc->name = "postgredb";
		dbDesc->driver = "QPSQL";
		dbDesc->port = 5432;
		descriptorSet.add(dbDesc);
	}*/
}

void saveDatabaseConfigs()
{
	csjp::ObjectTree tree("database-connections");
	unsigned s = descriptorSet.size();
	for(unsigned i = 0; i < s; i++){
		const DatabaseDescriptor & desc = descriptorSet.queryAt(i);
		csjp::String name(C_STR(desc.name));
		csjp::Map<csjp::String, csjp::String> & props = tree[name].properties;
		props["driver"] <<= desc.driver; // for example "QSQLITE"
		props["databaseName"] <<= desc.databaseName; //file name in case of sqlite
		props["username"] <<= desc.username;
		/* DO NOT SAVE THE VALUE OF desc.savePassword */
		if(desc.savePassword)
			props["password"] <<= desc.password;
		props["host"] <<= desc.host; // domain name or ip
		props["port"] = desc.port;
	}
	csjp::ObjectTree & origTree = config["database-connections"];
	origTree = move_cast(tree);
}

Database & loadDatabase(const QString & name)
{
	if(!databases.has(name)){
		if(!descriptorSet.has(name))
			throw DbError("Database connection '%s' is not yet specified.",
					C_STR(name));
		csjp::Object<Database> db(new Database(name));
		databases.add(db);
	}

	return databases.query(name);
}

csjp::Object<CustomModel> customModel(const QString & dbname)
{
	return loadDatabase(dbname).customItems();
}
/*
DatabaseDescriptor & databaseDescriptor(const QString & dbname)
{
	return loadDatabase(dbname).databaseDescriptor();
}
*/
ShoppingModel & shoppingModel(const QString & dbname)
{
	return loadDatabase(dbname).shoppingItems();
}

TagsModel & tagsModel(const QString & dbname)
{
	return loadDatabase(dbname).tags();
}

PartnersModel & partnersModel(const QString & dbname)
{
	return loadDatabase(dbname).partners();
}

CompanyModel & companyModel(const QString & dbname)
{
	return loadDatabase(dbname).company();
}

WaresModel & waresModel(const QString & dbname)
{
	return loadDatabase(dbname).wares();
}

QueriesModel & queriesModel(const QString & dbname)
{
	return loadDatabase(dbname).queries();
}
