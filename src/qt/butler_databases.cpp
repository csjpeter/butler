/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <csjp_map.h>

#include "butler_databases.h"

#include <QDir>

DatabaseDescriptorSet descriptorSet;

class Database
{
private:
	QString dbname;
	SqlConnection * sql;
	TagModel * tagModel;
	WareModel * wareModel;
	CompanyModel * companyModel;
	BrandModel * brandModel;
	InventoryModel * inventoryModel;
	AccountModel * accountModel;
	PartnerModel * partnersModel;
	QueryModel * queryModel;
	//ShoppingModel * shoppingModel;

public:
	Database(const QString & dbname) :
		dbname(dbname),
		sql(0),
		tagModel(0),
		wareModel(0),
		companyModel(0),
		brandModel(0),
		inventoryModel(0),
		accountModel(0),
		partnersModel(0),
		queryModel(0)
		//shoppingModel(0)
	{
	}

	explicit Database(const Database &) = delete;
	~Database()
	{
		//delete shoppingModel;
		delete queryModel;
		delete partnersModel;
		delete accountModel;
		delete companyModel;
		delete inventoryModel;
		delete brandModel;
		delete wareModel;
		delete tagModel;

		delete sql;
	}

	Database& operator=(const Database &) = delete;

	bool isEqual(const Database & dm) const { return dbname == dm.dbname; }

	bool isLess(const Database & dm) const { return dbname < dm.dbname; }
	
	bool isLess(const QString & s) const { return dbname < s; }
	bool isMore(const QString & s) const { return s < dbname; }

private:
	SqlConnection & sqlConn()
	{
		if(!sql)
			sql = new SqlConnection(descriptorSet.query(dbname));
		if(!sql->isOpen())
			sql->dbDesc = descriptorSet.query(dbname);
		return *sql;
	}

public:
	csjp::Object<ItemModel> items()
	{
		/* Each item view shall have its own special item model. */
		return csjp::Object<ItemModel>(new ItemModel(sqlConn(), wares()));
	}

	TagModel & tags()
	{
		if(!tagModel)
			tagModel = new TagModel(sqlConn());
		return *tagModel;
	}

	WareModel & wares()
	{
		if(!wareModel)
			wareModel = new WareModel(sqlConn());
		return *wareModel;
	}

	CompanyModel & company()
	{
		if(!companyModel)
			companyModel = new CompanyModel(sqlConn());
		return *companyModel;
	}

	BrandModel & brands()
	{
		if(!brandModel)
			brandModel = new BrandModel(sqlConn());
		return *brandModel;
	}

	InventoryModel & inventories()
	{
		if(!inventoryModel)
			inventoryModel = new InventoryModel(sqlConn());
		return *inventoryModel;
	}

	AccountModel & accounts()
	{
		if(!accountModel)
			accountModel = new AccountModel(sqlConn());
		return *accountModel;
	}

	PartnerModel & partners()
	{
		if(!partnersModel)
			partnersModel = new PartnerModel(sqlConn());
		return *partnersModel;
	}

	QueryModel & queries()
	{
		if(!queryModel)
			queryModel = new QueryModel(sqlConn());
		return *queryModel;
	}
/*
	ShoppingModel & shoppingItems()
	{
		if(!shoppingModel)
			shoppingModel = new ShoppingModel(*itemDb, wares());
		return *shoppingModel;
	}
*/
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
	csjp::Json & tree = config["database-connections"];
	unsigned s = tree.properties.size();
	for(unsigned i = 0; i < s; i++){
		csjp::Object<DatabaseDescriptor> desc(new DatabaseDescriptor);
		csjp::Json & dbt = tree.properties.queryAt(i);
		desc->name <<= dbt.key;
		desc->driver <<= dbt["driver"];
		desc->databaseName <<= dbt["databaseName"];
		desc->username <<= dbt["username"];
		desc->password <<= dbt["password"];
		if(!desc->password.isEmpty())
			desc->savePassword = true;
		desc->host <<= dbt["host"];
		desc->port <<= dbt["port"];
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
	csjp::Json tree("database-connections");
	unsigned s = descriptorSet.size();
	for(unsigned i = 0; i < s; i++){
		const DatabaseDescriptor & desc = descriptorSet.queryAt(i);
		csjp::String name(C_STR(desc.name));
		auto & db = tree[name];
		db["driver"] <<= desc.driver; // for example "QSQLITE"
		db["databaseName"] <<= desc.databaseName; //file name in case of sqlite
		db["username"] <<= desc.username;
		/* DO NOT SAVE THE VALUE OF desc.savePassword */
		if(desc.savePassword)
			db["password"] <<= desc.password;
		db["host"] <<= desc.host; // domain name or ip
		db["port"] <<= desc.port;
	}
	csjp::Json & origTree = config["database-connections"];
	origTree = move_cast(tree);
}

csjp::OwnerContainer<Database> databases;

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

TagModel & tagModel(const QString & dbname)
{
	return loadDatabase(dbname).tags();
}

WareModel & wareModel(const QString & dbname)
{
	return loadDatabase(dbname).wares();
}

CompanyModel & companyModel(const QString & dbname)
{
	return loadDatabase(dbname).company();
}

BrandModel & brandModel(const QString & dbname)
{
	return loadDatabase(dbname).brands();
}

InventoryModel & inventoryModel(const QString & dbname)
{
	return loadDatabase(dbname).inventories();
}

AccountModel & accountModel(const QString & dbname)
{
	return loadDatabase(dbname).accounts();
}

PartnerModel & partnersModel(const QString & dbname)
{
	return loadDatabase(dbname).partners();
}

QueryModel & queryModel(const QString & dbname)
{
	return loadDatabase(dbname).queries();
}
/*
ShoppingModel & shoppingModel(const QString & dbname)
{
	return loadDatabase(dbname).shoppingItems();
}
*/
csjp::Object<ItemModel> itemModel(const QString & dbname)
{
	return loadDatabase(dbname).items();
}