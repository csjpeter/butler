/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2014 Csaszar, Peter
 */

#include <QtGui>

#include "butler_accountsmodel.h"

SCC TidAccountFieldName			= QT_TRANSLATE_NOOP("AccountsModel", "Name");
SCC TidAccountFieldCurrency		= QT_TRANSLATE_NOOP("AccountsModel", "Currency");
SCC TidAccountFieldBankOffice	= QT_TRANSLATE_NOOP("AccountsModel", "Bank Office");
SCC TidAccountFieldIban			= QT_TRANSLATE_NOOP("AccountsModel", "IBAN");
SCC TidAccountFieldSwiftCode	= QT_TRANSLATE_NOOP("AccountsModel", "Swift code");

AccountsModel::AccountsModel(AccountDb & db) :
	db(db)
{
	query();
}

AccountsModel::~AccountsModel()
{
}

QModelIndex AccountsModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags AccountsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)accounts.size() && index.column() < Account::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant AccountsModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)accounts.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Account::Name :
			return QVariant(accounts.queryAt(index.row()).name);
			break;
		case Account::Currency :
			return QVariant(accounts.queryAt(index.row()).currency);
			break;
		case Account::BankOffice :
			return QVariant(accounts.queryAt(index.row()).bankOffice);
			break;
		case Account::Iban :
			return QVariant(accounts.queryAt(index.row()).iban);
			break;
		case Account::SwiftCode :
			return QVariant(accounts.queryAt(index.row()).swiftCode);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant AccountsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("");

	switch(section){
		case Account::Name :
			return QVariant(tr(TidAccountFieldName));
			break;
		case Account::Currency :
			return QVariant(tr(TidAccountFieldCurrency));
			break;
		case Account::BankOffice :
			return QVariant(tr(TidAccountFieldBankOffice));
			break;
		case Account::Iban :
			return QVariant(tr(TidAccountFieldIban));
			break;
		case Account::SwiftCode :
			return QVariant(tr(TidAccountFieldSwiftCode));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool AccountsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)accounts.size() <= index.row())
		return false;

	int row = index.row();
	Account modified(accounts.queryAt(row));

	switch(index.column()){
		case Account::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Account::Currency :
			modified.currency <<= value;
			update(row, modified);
			break;
		case Account::BankOffice :
			modified.bankOffice <<= value;
			update(row, modified);
			break;
		case Account::Iban :
			modified.iban <<= value;
			update(row, modified);
			break;
		case Account::SwiftCode :
			modified.swiftCode <<= value;
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool AccountsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int AccountsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return accounts.size();
}

int AccountsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Account::NumOfFields;
}

bool AccountsModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool AccountsModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void AccountsModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int AccountsModel::index(const Text & name) const
{
	if(accounts.has(name))
		return accounts.index(name);
	else
		return -1;
}

const Account& AccountsModel::account(int row)
{
	return accounts.queryAt(row);
}

void AccountsModel::del(int row)
{
	Account & account = accounts.queryAt(row);
	db.del(account);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	accounts.removeAt(row);
}

void AccountsModel::addNew(Account & account)
{
	db.insert(account);
	ModelInsertGuard g(this, QModelIndex(), accounts.size(), accounts.size());
	accounts.add(new Account(account));
}

void AccountsModel::update(int row, Account & modified)
{
	Account & orig = accounts.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Account::NumOfFields-1));
}

void AccountsModel::query()
{
	ModelResetGuard g(this);
	db.query(accounts);
}

void AccountsModel::sort(int column, bool ascending)
{
	if(accounts.ascending == ascending && accounts.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	accounts.ascending = ascending;
	accounts.ordering = static_cast<Account::Fields>(column);
	accounts.sort();
}
