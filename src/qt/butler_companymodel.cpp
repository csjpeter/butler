/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_companymodel.h"

SCC TidCompanyFieldName		= QT_TRANSLATE_NOOP("CompanyModel", "Name");
SCC TidCompanyFieldCountry	= QT_TRANSLATE_NOOP("CompanyModel", "Country");
SCC TidCompanyFieldCity		= QT_TRANSLATE_NOOP("CompanyModel", "City");
SCC TidCompanyFieldPostalCode	= QT_TRANSLATE_NOOP("CompanyModel", "Postal code");
SCC TidCompanyFieldAddress	= QT_TRANSLATE_NOOP("CompanyModel", "Address");
SCC TidCompanyFieldTaxId	= QT_TRANSLATE_NOOP("CompanyModel", "Tax Id");

CompanyModel::CompanyModel(Db & db) :
	db(db)
{
	query();
}

CompanyModel::~CompanyModel()
{
}

QModelIndex CompanyModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags CompanyModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)companys.size() && index.column() < Company::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant CompanyModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)companys.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Company::Name :
			return QVariant(companys.queryAt(index.row()).name);
			break;
		case Company::Country :
			return QVariant(companys.queryAt(index.row()).country);
			break;
		case Company::City :
			return QVariant(companys.queryAt(index.row()).city);
			break;
		case Company::PostalCode :
			return QVariant(companys.queryAt(index.row()).postalCode);
			break;
		case Company::Address :
			return QVariant(companys.queryAt(index.row()).address);
			break;
		case Company::TaxId :
			return QVariant(companys.queryAt(index.row()).taxId);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant CompanyModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("");

	switch(section){
		case Company::Name :
			return QVariant(tr(TidCompanyFieldName));
			break;
		case Company::Country :
			return QVariant(tr(TidCompanyFieldCountry));
			break;
		case Company::City :
			return QVariant(tr(TidCompanyFieldCity));
			break;
		case Company::PostalCode :
			return QVariant(tr(TidCompanyFieldPostalCode));
			break;
		case Company::Address :
			return QVariant(tr(TidCompanyFieldAddress));
			break;
		case Company::TaxId :
			return QVariant(tr(TidCompanyFieldTaxId));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool CompanyModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)companys.size() <= index.row())
		return false;

	int row = index.row();
	Company modified(companys.queryAt(row));

	switch(index.column()){
		case Company::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case Company::Country :
			modified.country = value.toString();
			update(row, modified);
			break;
		case Company::City :
			modified.city = value.toString();
			update(row, modified);
			break;
		case Company::PostalCode :
			modified.postalCode = value.toString();
			update(row, modified);
			break;
		case Company::Address :
			modified.address = value.toString();
			update(row, modified);
			break;
		case Company::TaxId :
			modified.taxId = value.toString();
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool CompanyModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int CompanyModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return companys.size();
}

int CompanyModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Company::NumOfFields;
}

bool CompanyModel::removeRows(
		int row, int count, const QModelIndex &parent)
{
	try {
		beginRemoveRows(parent, row, row + count - 1);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
	return true;
}

bool CompanyModel::insertRows(
		int row, int count, const QModelIndex &parent)
{
	try {
		beginInsertRows(parent, row, row + count - 1);
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
	return true;
}

void CompanyModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int CompanyModel::index(const QString &name) const
{
	if(companys.has(name))
		return companys.index(name);
	else
		return -1;
}

const Company& CompanyModel::company(int row)
{
	return companys.queryAt(row);
}

void CompanyModel::del(int row)
{
	Company &company = companys.queryAt(row);
	db.company.del(company);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		companys.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void CompanyModel::addNew(Company &company)
{
	db.company.insert(company);
	try {
		beginInsertRows(QModelIndex(), companys.size(), companys.size());
		companys.add(new Company(company));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
}

void CompanyModel::update(int row, Company &modified)
{
	Company &orig = companys.queryAt(row);
	db.company.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Company::NumOfFields-1));
}

void CompanyModel::query()
{
	try {
		beginResetModel();
		db.company.query(companys);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

void CompanyModel::sort(int column, bool ascending)
{
	if(companys.ascending == ascending && companys.ordering == column)
		return;

	try {
		beginResetModel();
		companys.ascending = ascending;
		companys.ordering = static_cast<Company::Fields>(column);
		companys.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}
