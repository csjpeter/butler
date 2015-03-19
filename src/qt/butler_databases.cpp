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
static DatabaseDescriptorModel databases_model(descriptorSet);

class Database
{
private:
	csjp::String dbname;
	SqlConnection * sql;
	TagModel * tagModel;
	WareModel * wareModel;
	CompanyModel * companyModel;
	BrandModel * brandModel;
	InventoryModel * inventoryModel;
	PartnerModel * partnersModel;
	AccountModel * accountModel;
	QueryModel * queryModel;
	//ShoppingModel * shoppingModel;

public:
	Database(const csjp::String & dbname) :
		dbname(dbname),
		sql(0),
		tagModel(0),
		wareModel(0),
		companyModel(0),
		brandModel(0),
		inventoryModel(0),
		partnersModel(0),
		accountModel(0),
		queryModel(0)
		//shoppingModel(0)
	{
		Tag::tableInit(sqlConn());
		Ware::tableInit(sqlConn());
		WareType::tableInit(sqlConn());
		WareTag::tableInit(sqlConn());
		Company::tableInit(sqlConn());
		Brand::tableInit(sqlConn());
		Inventory::tableInit(sqlConn());
		Partner::tableInit(sqlConn());
		Account::tableInit(sqlConn());
		Payment::tableInit(sqlConn());
		Item::tableInit(sqlConn());
		Query::tableInit(sqlConn());
		QueryWithTag::tableInit(sqlConn());
		QueryWithoutTag::tableInit(sqlConn());
		QueryWare::tableInit(sqlConn());
		QueryPartner::tableInit(sqlConn());
	}

	explicit Database(const Database &) = delete;
	~Database()
	{
		//delete shoppingModel;
		delete queryModel;
		delete accountModel;
		delete partnersModel;
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
	
	bool isLess(const csjp::String & s) const { return dbname < s; }
	bool isMore(const csjp::String & s) const { return s < dbname; }

private:
	SqlConnection & sqlConn()
	{
		if(!sql)
			sql = new SqlConnection(descriptorSet.query(dbname));
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

	PartnerModel & partners()
	{
		if(!partnersModel)
			partnersModel = new PartnerModel(sqlConn());
		return *partnersModel;
	}

	AccountModel & accounts()
	{
		if(!accountModel)
			accountModel = new AccountModel(sqlConn());
		return *accountModel;
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

inline bool operator<(const csjp::String & a, const Database & b)
{
	return b.isMore(a);
}

inline bool operator<(const Database & a, const csjp::String & b)
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
		desc->name = dbt.key;
		desc->driver <<= dbt["driver"];
		desc->databaseName <<= dbt["databaseName"];
		desc->username <<= dbt["username"];
		desc->password <<= dbt["password"];
		if(desc->password.length)
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
		sqlitedb->databaseName <<= defaultSQLiteDbFileName();
		LOG("Sqlite db file path: %s", sqlitedb->databaseName.str);
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
		auto & db = tree[desc.name];
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

Database & loadDatabase(const csjp::String & name)
{
	if(!databases.has(name)){
		if(!descriptorSet.has(name))
			throw DbError("Database connection '%s' is not yet specified.", name.str);
		csjp::Object<Database> db(new Database(name));
		databases.add(db);
	}

	return databases.query(name);
}

DatabaseDescriptorModel & databasesModel()
{
	return databases_model;
}

TagModel & tagModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).tags();
}

WareModel & wareModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).wares();
}

CompanyModel & companyModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).company();
}

BrandModel & brandModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).brands();
}

InventoryModel & inventoryModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).inventories();
}

PartnerModel & partnersModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).partners();
}

AccountModel & accountModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).accounts();
}

QueryModel & queryModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).queries();
}
/*
ShoppingModel & shoppingModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).shoppingItems();
}
*/
csjp::Object<ItemModel> itemModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).items();
}
