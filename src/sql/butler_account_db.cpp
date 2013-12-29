/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include "butler_account_db.h"

AccountDb::AccountDb(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("accounts"))
		sql.exec("CREATE TABLE accounts ("
				"name TEXT NOT NULL PRIMARY KEY, "
				"currency TEXT NOT NULL, "
				"bankOffice TEXT NOT NULL, "
				"postalCode TEXT NOT NULL, "
				"swiftCode TEXT NOT NULL, "
				"lastModified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP "
				")"
			       );
	cols = sql.columns("accounts");
	if(		!cols.has("name") ||
			!cols.has("currency") ||
			( !cols.has("bankOffice") && !cols.has("bankoffice") ) ||
			!cols.has("postalCode") ||
			( !cols.has("swiftCode") && !cols.has("swiftcode") ) ||
			( !cols.has("lastModified") && !cols.has("lastmodified") )
	  )
		throw DbIncompatibleTableError(
			"Incompatible table accounts in the openend database.");
}

AccountDb::~AccountDb()
{
}

void AccountDb::insert(const Account & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO accounts "
			"(name, currency, bankOffice, postalCode, swiftCode) "
			"VALUES(?, ?, ?, ?, ?, ?, ?)");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.bindValue(1, s.currency);
	sqlQuery.bindValue(2, s.bankOffice);
	sqlQuery.bindValue(3, s.iban);
	sqlQuery.bindValue(4, s.swiftCode);
	sqlQuery.exec();

	tr.commit();
}

void AccountDb::update(const Account & orig, const Account & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE accounts SET "
			"name = ?, "
			"currency = ?, "
			"bankOffice = ?, "
			"postalCode = ?, "
			"swiftCode = ?, "
			"WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.currency);
	sqlQuery.bindValue(2, modified.bankOffice);
	sqlQuery.bindValue(3, modified.iban);
	sqlQuery.bindValue(4, modified.swiftCode);
	sqlQuery.bindValue(5, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void AccountDb::del(const Account & s)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM accounts WHERE name = ?");
	sqlQuery.bindValue(0, s.name);
	sqlQuery.exec();

	tr.commit();
}

void AccountDb::query(AccountSet & ss)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT "
			"name, currency, bankOffice, postalCode, swiftCode "
			"FROM accounts");
	sqlQuery.exec();
	ss.clear();
	int nameNo = sqlQuery.colIndex("name");
	int currencyNo = sqlQuery.colIndex("currency");
	int bankOfficeNo = sqlQuery.colIndex("bankOffice");
	int ibanNo = sqlQuery.colIndex("postalCode");
	int swiftCodeNo = sqlQuery.colIndex("swiftCode");
	DBG("----- Account query result:");
	while(sqlQuery.next()) {
		Account *s = new Account();
		s->name = sqlQuery.text(nameNo);
		s->currency = sqlQuery.text(currencyNo);
		s->bankOffice = sqlQuery.text(bankOfficeNo);
		s->iban = sqlQuery.text(ibanNo);
		s->swiftCode = sqlQuery.text(swiftCodeNo);
		ss.add(s);
	}
	DBG("-----");

	tr.commit();
}
