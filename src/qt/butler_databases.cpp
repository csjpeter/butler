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
	@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
	@Type@Model @type@Model;
	@}ForTypes@
private:
	csjp::String dbname;

public:
	Database(const csjp::String & dbname) :
		sql(descriptorSet.query(dbname)),
		@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
		@type@Model(sql),
		@}ForTypes@
		dbname(dbname)
	{
		SqlColumns cols;
		const SqlTableNames & tables = sql.tables();
		//for(auto & t : tables)
		//	LOG("table ", t);
		@ForTypes{Tag,Ware,WareType,WareTag,Company,Brand,Inventory,Partner,Account
				Item,ItemQuery,ItemQueryWithTag,ItemQueryWithoutTag,ItemQueryWare
				ItemQueryPartner,Payment,PaymentQuery,PaymentQueryPartner@
		if(tables.has("@TableName@")){
			cols = sql.columns("@TableName@");
			//LOG("Table @TableName@");
			//for(auto & c : cols)
			//	LOG("column ", c);
			if(@For{TableField@!cols.has("@.colName@") ||@-@!cols.has("@.colName@"))@}@
				throw DbIncompatibleTableError(
					"Incompatible table @TableName@ in the openend database.");
		} else {
			sql.exec("CREATE TABLE @TableName@ ("
				@For{TableField@"@.colName@ @.SqlDecl@,"
				@-@"@.colName@ @.SqlDecl@"@}@
				@For{Constraint@", @Constraint@"@}@
				")");
		}
		@}ForTypes@

		@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
		@type@Model.query();
		@}ForTypes@
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
	csjp::Json tree("database-connections");
	for(const auto & desc : descriptorSet){
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

@ForTypes{Tag,Ware,Company,Brand,Inventory,Partner,Account,ItemQuery,PaymentQuery@
@Type@Model & @type@Model(const csjp::String & dbname)
{
	return loadDatabase(dbname).@type@Model;
}
@}ForTypes@
