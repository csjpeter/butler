/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include "butler_databases.h"

#include <QDir>

DatabaseDescriptorSet descriptorSet;
static DatabaseDescriptorModel databases_model(descriptorSet);

class Database
{
private:
	SqlConnection sql;
public:
	
	TagModel tagModel;
	
	WareModel wareModel;
	
	CompanyModel companyModel;
	
	BrandModel brandModel;
	
	InventoryModel inventoryModel;
	
	PartnerModel partnerModel;
	
	AccountModel accountModel;
	
	ItemQueryModel itemQueryModel;
	
	PaymentQueryModel paymentQueryModel;
	
private:
	csjp::String dbname;

public:
	Database(const csjp::String & dbname) :
		sql(descriptorSet.query(dbname)),
		
		tagModel(sql),
		
		wareModel(sql),
		
		companyModel(sql),
		
		brandModel(sql),
		
		inventoryModel(sql),
		
		partnerModel(sql),
		
		accountModel(sql),
		
		itemQueryModel(sql),
		
		paymentQueryModel(sql),
		
		dbname(dbname)
	{
		SqlColumns cols;
		const SqlTableNames & tables = sql.tables();
		//for(auto & t : tables)
		//	LOG("table ", t);
		
		if(tables.has("tag")){
			cols = sql.columns("tag");
			//LOG("Table tag");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("description") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table tag in the openend database.");
		} else {
			sql.exec("CREATE TABLE tag ("
"name TEXT,"
				"description TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("ware")){
			cols = sql.columns("ware");
			//LOG("Table ware");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("unit") ||!cols.has("icon") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table ware in the openend database.");
		} else {
			sql.exec("CREATE TABLE ware ("
"name TEXT,"
				"unit TEXT,"
				"icon TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("ware_type")){
			cols = sql.columns("ware_type");
			//LOG("Table ware_type");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("ware") ||!cols.has("type") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table ware_type in the openend database.");
		} else {
			sql.exec("CREATE TABLE ware_type ("
"ware TEXT,"
				"type TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (ware, type)"", FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("ware_tag")){
			cols = sql.columns("ware_tag");
			//LOG("Table ware_tag");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("ware") ||!cols.has("tag") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table ware_tag in the openend database.");
		} else {
			sql.exec("CREATE TABLE ware_tag ("
"ware TEXT,"
				"tag TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (ware, tag)"", FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("company")){
			cols = sql.columns("company");
			//LOG("Table company");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("country") ||!cols.has("city") ||!cols.has("postal_code") ||!cols.has("address") ||!cols.has("tax_id") ||!cols.has("icon") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table company in the openend database.");
		} else {
			sql.exec("CREATE TABLE company ("
"name TEXT,"
				"country TEXT,"
				"city TEXT,"
				"postal_code TEXT,"
				"address TEXT,"
				"tax_id TEXT,"
				"icon TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("brand")){
			cols = sql.columns("brand");
			//LOG("Table brand");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("company") ||!cols.has("icon") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table brand in the openend database.");
		} else {
			sql.exec("CREATE TABLE brand ("
"name TEXT,"
				"company TEXT,"
				"icon TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"", FOREIGN KEY (company) REFERENCES company(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("inventory")){
			cols = sql.columns("inventory");
			//LOG("Table inventory");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("comment") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table inventory in the openend database.");
		} else {
			sql.exec("CREATE TABLE inventory ("
"name TEXT,"
				"comment TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("partner")){
			cols = sql.columns("partner");
			//LOG("Table partner");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("country") ||!cols.has("city") ||!cols.has("postal_code") ||!cols.has("address") ||!cols.has("company") ||!cols.has("store_name") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table partner in the openend database.");
		} else {
			sql.exec("CREATE TABLE partner ("
"name TEXT,"
				"country TEXT,"
				"city TEXT,"
				"postal_code TEXT,"
				"address TEXT,"
				"company TEXT,"
				"store_name TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"", FOREIGN KEY (company) REFERENCES company(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("account")){
			cols = sql.columns("account");
			//LOG("Table account");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("currency") ||!cols.has("bank_office") ||!cols.has("iban") ||!cols.has("swift_code") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table account in the openend database.");
		} else {
			sql.exec("CREATE TABLE account ("
"name TEXT,"
				"currency TEXT,"
				"bank_office TEXT,"
				"iban TEXT,"
				"swift_code TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"", FOREIGN KEY (bank_office) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("item")){
			cols = sql.columns("item");
			//LOG("Table item");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("upload_date") ||!cols.has("name") ||!cols.has("unit") ||!cols.has("type") ||!cols.has("brand") ||!cols.has("quantity") ||!cols.has("price") ||!cols.has("currency") ||!cols.has("account") ||!cols.has("partner") ||!cols.has("inventory") ||!cols.has("comment") ||!cols.has("inv_change_date") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item in the openend database.");
		} else {
			sql.exec("CREATE TABLE item ("
"upload_date TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date),"
				"name TEXT,"
				"unit TEXT,"
				"type TEXT,"
				"brand TEXT,"
				"quantity DECIMAL(15,3) NOT NULL,"
				"price DECIMAL(15,2) NOT NULL,"
				"currency TEXT,"
				"account TEXT,"
				"partner TEXT,"
				"inventory TEXT,"
				"comment TEXT,"
				"inv_change_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (upload_date)"", FOREIGN KEY (name) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (brand) REFERENCES brand(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (account) REFERENCES account(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (inventory) REFERENCES inventory(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("item_query")){
			cols = sql.columns("item_query");
			//LOG("Table item_query");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("start_date") ||!cols.has("end_date") ||!cols.has("stock_option") ||!cols.has("tag_option") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item_query in the openend database.");
		} else {
			sql.exec("CREATE TABLE item_query ("
"name TEXT,"
				"start_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"end_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"stock_option INTEGER NOT NULL,"
				"tag_option INTEGER NOT NULL,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("item_query_with_tag")){
			cols = sql.columns("item_query_with_tag");
			//LOG("Table item_query_with_tag");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("query") ||!cols.has("tag") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item_query_with_tag in the openend database.");
		} else {
			sql.exec("CREATE TABLE item_query_with_tag ("
"query TEXT,"
				"tag TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (query, tag)"", FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("item_query_without_tag")){
			cols = sql.columns("item_query_without_tag");
			//LOG("Table item_query_without_tag");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("query") ||!cols.has("tag") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item_query_without_tag in the openend database.");
		} else {
			sql.exec("CREATE TABLE item_query_without_tag ("
"query TEXT,"
				"tag TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (query, tag)"", FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (tag) REFERENCES tag(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("item_query_ware")){
			cols = sql.columns("item_query_ware");
			//LOG("Table item_query_ware");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("query") ||!cols.has("ware") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item_query_ware in the openend database.");
		} else {
			sql.exec("CREATE TABLE item_query_ware ("
"query TEXT,"
				"ware TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (query, ware)"", FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (ware) REFERENCES ware(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("item_query_partner")){
			cols = sql.columns("item_query_partner");
			//LOG("Table item_query_partner");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("query") ||!cols.has("partner") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table item_query_partner in the openend database.");
		} else {
			sql.exec("CREATE TABLE item_query_partner ("
"query TEXT,"
				"partner TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (query, partner)"", FOREIGN KEY (query) REFERENCES item_query(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("payment")){
			cols = sql.columns("payment");
			//LOG("Table payment");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("upload_date") ||!cols.has("account") ||!cols.has("partner") ||!cols.has("amount") ||!cols.has("subject") ||!cols.has("pay_date") ||!cols.has("comment") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table payment in the openend database.");
		} else {
			sql.exec("CREATE TABLE payment ("
"upload_date TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date),"
				"account TEXT,"
				"partner TEXT,"
				"amount DECIMAL(15,3) NOT NULL,"
				"subject TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date),"
				"pay_date TIMESTAMP CHECK('1970-01-01T00:00:00' < upload_date),"
				"comment TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (upload_date)"", FOREIGN KEY (account) REFERENCES account(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (subject) REFERENCES item(upload_date) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		
		if(tables.has("payment_query")){
			cols = sql.columns("payment_query");
			//LOG("Table payment_query");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("name") ||!cols.has("start_date") ||!cols.has("end_date") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table payment_query in the openend database.");
		} else {
			sql.exec("CREATE TABLE payment_query ("
"name TEXT,"
				"start_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"end_date TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (name)"				")");
		}
		
		if(tables.has("payment_query_partner")){
			cols = sql.columns("payment_query_partner");
			//LOG("Table payment_query_partner");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(!cols.has("query") ||!cols.has("partner") ||!cols.has("last_modified") ||!cols.has("deleted"))				throw DbIncompatibleTableError(
					"Incompatible table payment_query_partner in the openend database.");
		} else {
			sql.exec("CREATE TABLE payment_query_partner ("
"query TEXT,"
				"partner TEXT,"
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,"
				"deleted CHAR(1) NOT NULL DEFAULT 'N'"", PRIMARY KEY (query, partner)"", FOREIGN KEY (query) REFERENCES payment_query(name) ON DELETE CASCADE ON UPDATE CASCADE"", FOREIGN KEY (partner) REFERENCES partner(name) ON DELETE CASCADE ON UPDATE CASCADE"				")");
		}
		

		
		tagModel.query();
		
		wareModel.query();
		
		companyModel.query();
		
		brandModel.query();
		
		inventoryModel.query();
		
		partnerModel.query();
		
		accountModel.query();
		
		itemQueryModel.query();
		
		paymentQueryModel.query();
		
	}

	explicit Database(const Database &) = delete;
	~Database(){}

	Database& operator=(const Database &) = delete;

	bool isEqual(const Database & dm) const { return dbname == dm.dbname; }

	bool isLess(const Database & dm) const { return dbname < dm.dbname; }
	
	bool isLess(const csjp::String & s) const { return dbname < s; }
	bool isMore(const csjp::String & s) const { return s < dbname; }

public:
	csjp::Object<ItemModel> items()
	{
		/* Each item view shall have its own special item model. */
		return csjp::Object<ItemModel>(new ItemModel(sql, wareModel));
	}
	csjp::Object<PaymentModel> payments()
	{
		/* Each payment view shall have its own special payment model. */
		return csjp::Object<PaymentModel>(new PaymentModel(sql));
	}

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
	for(auto & j : tree.properties){
		csjp::Json & db = const_cast<csjp::Json &>(j);
		csjp::Object<DatabaseDescriptor> desc(new DatabaseDescriptor);
		desc->name = db.key;
#ifdef PGSQL
		if(db["driver"] == "PSql")
			desc->driver = SqlDriver::Enum::PSql;
		else
#endif
#ifdef MYSQL
			if(db["driver"] == "MySQL")
			desc->driver = SqlDriver::Enum::MySQL;
		else
#endif
			if(db["driver"] == "SQLite")
			desc->driver = SqlDriver::Enum::SQLite;
		desc->databaseName <<= db["databaseName"];
		desc->username <<= db["username"];
		desc->password <<= db["password"];
		if(desc->password.length)
			desc->savePassword = true;
		desc->host <<= db["host"];
		desc->port <<= db["port"];
		descriptorSet.add(desc);
	}

	/* Init the local database if it is not defined already */
	if(!descriptorSet.has("localdb")){
		csjp::Object<DatabaseDescriptor> sqlitedb(new DatabaseDescriptor);
		sqlitedb->name = "localdb";
		sqlitedb->driver = SqlDriver::Enum::SQLite;
		sqlitedb->databaseName <<= defaultSQLiteDbFileName();
		LOG("Sqlite db file path: ", sqlitedb->databaseName);
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
	csjp::Json tree;
	for(const auto & desc : descriptorSet)
	{
        auto & db = tree[desc.name];
		switch(desc.driver){
#ifdef PGSQL
			case SqlDriver::Enum::PSql: db["driver"] = "PSql"; break;
#endif
#ifdef MYSQL
			case SqlDriver::Enum::MySQL: db["driver"] = "MySQL"; break;
#endif
			case SqlDriver::Enum::SQLite: db["driver"] = "SQLite"; break;
		}
		db["databaseName"] = desc.databaseName; //file name in case of sqlite
		db["username"] = desc.username;
		/* DO NOT SAVE THE VALUE OF desc.savePassword */
		if(desc.savePassword)
			db["password"] = desc.password;
		db["host"] = desc.host; // domain name or ip
		db["port"] <<= desc.port;
	}
	config["database-connections"] = move_cast(tree);
}

csjp::OwnerContainer<Database> databases;

static Database & loadDatabase(const csjp::String & name)
{
	if(!databases.has(name)){
		if(!descriptorSet.has(name))
			throw DbError("Database connection '%' is not yet specified.", name);
		csjp::Object<Database> db(new Database(name));
		databases.add(db);
	}

	return const_cast<Database &>(databases.query(name));
}

DatabaseDescriptorModel & databasesModel()
{
	return databases_model;
}

csjp::Object<ItemModel> itemModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).items();
}

csjp::Object<PaymentModel> paymentModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).payments();
}


TagModel & tagModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).tagModel;
}

WareModel & wareModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).wareModel;
}

CompanyModel & companyModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).companyModel;
}

BrandModel & brandModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).brandModel;
}

InventoryModel & inventoryModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).inventoryModel;
}

PartnerModel & partnerModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).partnerModel;
}

AccountModel & accountModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).accountModel;
}

ItemQueryModel & itemQueryModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).itemQueryModel;
}

PaymentQueryModel & paymentQueryModel(const csjp::String & dbname)
{
	return loadDatabase(dbname).paymentQueryModel;
}


