/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <csjp_map.h>

#include "butler_databases.h"

#include <butler_sql_connection.h>

#include <butler_tag_db.h>
#include <butler_ware_db.h>
#include <butler_brand_db.h>
#include <butler_inventory_db.h>
#include <butler_company_db.h>
#include <butler_partner_db.h>
#include <butler_item_db.h>
#include <butler_queries_db.h>

#include <QDir>

DatabaseDescriptorSet descriptorSet;

class Database
{
private:
	QString dbname;
	SqlConnection * sql;
	TagDb * tagDb;
	WareDb * wareDb;
	CompanyDb * companyDb;
	BrandDb * brandDb;
	InventoryDb * inventoryDb;
	AccountDb * accountDb;
	PartnerDb * partnerDb;
	QueryDb * queryDb;
	ItemDb * itemDb;
	TagsModel * tagsModel;
	WaresModel * waresModel;
	CompaniesModel * companyModel;
	BrandsModel * brandsModel;
	InventoriesModel * inventoriesModel;
	AccountsModel * accountsModel;
	PartnersModel * partnersModel;
	QueriesModel * queriesModel;
	ShoppingModel * shoppingModel;

public:
	Database(const QString & dbname) :
		dbname(dbname),
		sql(0),
		tagDb(0),
		wareDb(0),
		companyDb(0),
		brandDb(0),
		inventoryDb(0),
		accountDb(0),
		partnerDb(0),
		queryDb(0),
		itemDb(0),
		tagsModel(0),
		waresModel(0),
		companyModel(0),
		brandsModel(0),
		inventoriesModel(0),
		accountsModel(0),
		partnersModel(0),
		queriesModel(0),
		shoppingModel(0)
	{
	}

	explicit Database(const Database &) = delete;
	~Database()
	{
		delete shoppingModel;
		delete queriesModel;
		delete partnersModel;
		delete accountsModel;
		delete companyModel;
		delete inventoriesModel;
		delete brandsModel;
		delete waresModel;
		delete tagsModel;

		delete itemDb;
		delete queryDb;
		delete partnerDb;
		delete accountDb;
		delete companyDb;
		delete inventoryDb;
		delete brandDb;
		delete wareDb;
		delete tagDb;

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
	csjp::Object<ItemsModel> items()
	{
		if(!itemDb)
			itemDb = new ItemDb(sqlConn());
		/* Each item view shall have its own special item model. */
		return csjp::Object<ItemsModel>(new ItemsModel(*itemDb, wares()));
	}

	TagsModel & tags()
	{
		if(!tagDb)
			tagDb = new TagDb(sqlConn());
		if(!tagsModel)
			tagsModel = new TagsModel(*tagDb);
		return *tagsModel;
	}

	WaresModel & wares()
	{
		if(!wareDb)
			wareDb = new WareDb(sqlConn());
		if(!waresModel)
			waresModel = new WaresModel(*wareDb);
		return *waresModel;
	}

	CompaniesModel & company()
	{
		if(!companyDb)
			companyDb = new CompanyDb(sqlConn());
		if(!companyModel)
			companyModel = new CompaniesModel(*companyDb);
		return *companyModel;
	}

	BrandsModel & brands()
	{
		if(!brandDb)
			brandDb = new BrandDb(sqlConn());
		if(!brandsModel)
			brandsModel = new BrandsModel(*brandDb);
		return *brandsModel;
	}

	InventoriesModel & inventories()
	{
		if(!inventoryDb)
			inventoryDb = new InventoryDb(sqlConn());
		if(!inventoriesModel)
			inventoriesModel = new InventoriesModel(*inventoryDb);
		return *inventoriesModel;
	}

	AccountsModel & accounts()
	{
		if(!accountDb)
			accountDb = new AccountDb(sqlConn());
		if(!accountsModel)
			accountsModel = new AccountsModel(*accountDb);
		return *accountsModel;
	}

	PartnersModel & partners()
	{
		if(!partnerDb)
			partnerDb = new PartnerDb(sqlConn());
		if(!partnersModel)
			partnersModel = new PartnersModel(*partnerDb);
		return *partnersModel;
	}

	QueriesModel & queries()
	{
		if(!queryDb)
			queryDb = new QueryDb(sqlConn());
		if(!queriesModel)
			queriesModel = new QueriesModel(*queryDb);
		return *queriesModel;
	}
/*
	ShoppingModel & shoppingItems()
	{
		if(!itemDb)
			itemDb = new ItemDb(sqlConn());
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

TagsModel & tagsModel(const QString & dbname)
{
	return loadDatabase(dbname).tags();
}

WaresModel & waresModel(const QString & dbname)
{
	return loadDatabase(dbname).wares();
}

CompaniesModel & companiesModel(const QString & dbname)
{
	return loadDatabase(dbname).company();
}

BrandsModel & brandsModel(const QString & dbname)
{
	return loadDatabase(dbname).brands();
}

InventoriesModel & inventoriesModel(const QString & dbname)
{
	return loadDatabase(dbname).inventories();
}

AccountsModel & accountsModel(const QString & dbname)
{
	return loadDatabase(dbname).accounts();
}

PartnersModel & partnersModel(const QString & dbname)
{
	return loadDatabase(dbname).partners();
}

QueriesModel & queriesModel(const QString & dbname)
{
	return loadDatabase(dbname).queries();
}
/*
ShoppingModel & shoppingModel(const QString & dbname)
{
	return loadDatabase(dbname).shoppingItems();
}
*/
csjp::Object<ItemsModel> itemModel(const QString & dbname)
{
	return loadDatabase(dbname).items();
}
