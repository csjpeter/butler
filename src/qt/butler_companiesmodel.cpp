/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */
#if 0
#include <QtGui>

#include "butler_companiesmodel.h"

SCC TidCompanyFieldName		= QT_TRANSLATE_NOOP("CompaniesModel", "Name");
SCC TidCompanyFieldCountry	= QT_TRANSLATE_NOOP("CompaniesModel", "Country");
SCC TidCompanyFieldCity		= QT_TRANSLATE_NOOP("CompaniesModel", "City");
SCC TidCompanyFieldPostalCode	= QT_TRANSLATE_NOOP("CompaniesModel", "Postal code");
SCC TidCompanyFieldAddress	= QT_TRANSLATE_NOOP("CompaniesModel", "Address");
SCC TidCompanyFieldTaxId	= QT_TRANSLATE_NOOP("CompaniesModel", "Tax Id");

CompaniesModel::CompaniesModel(CompanyDb & db) :
	db(db)
{
	query();
}

CompaniesModel::~CompaniesModel()
{
}

QModelIndex CompaniesModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags CompaniesModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)companies.size() && index.column() < Company::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant CompaniesModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)companies.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Company::Name :
			return QVariant(companies.queryAt(index.row()).name);
			break;
		case Company::Country :
			return QVariant(companies.queryAt(index.row()).country);
			break;
		case Company::City :
			return QVariant(companies.queryAt(index.row()).city);
			break;
		case Company::PostalCode :
			return QVariant(companies.queryAt(index.row()).postalCode);
			break;
		case Company::Address :
			return QVariant(companies.queryAt(index.row()).address);
			break;
		case Company::TaxId :
			return QVariant(companies.queryAt(index.row()).taxId);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant CompaniesModel::headerData(int section, Qt::Orientation orientation, int role) const
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

bool CompaniesModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)companies.size() <= index.row())
		return false;

	int row = index.row();
	Company modified(companies.queryAt(row));

	switch(index.column()){
		case Company::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Company::Country :
			modified.country <<= value;
			update(row, modified);
			break;
		case Company::City :
			modified.city <<= value;
			update(row, modified);
			break;
		case Company::PostalCode :
			modified.postalCode <<= value;
			update(row, modified);
			break;
		case Company::Address :
			modified.address <<= value;
			update(row, modified);
			break;
		case Company::TaxId :
			modified.taxId <<= value;
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool CompaniesModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int CompaniesModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return companies.size();
}

int CompaniesModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Company::NumOfFields;
}

bool CompaniesModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
		ModelRemoveGuard g(this, parent, row, row + count - 1);
		return true;
}

bool CompaniesModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
		ModelInsertGuard g(this, parent, row, row + count - 1);
		return true;
}

void CompaniesModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int CompaniesModel::index(const Text & name) const
{
	if(companies.has(name))
		return companies.index(name);
	else
		return -1;
}

const Company& CompaniesModel::company(int row)
{
	return companies.queryAt(row);
}

void CompaniesModel::del(int row)
{
	Company & company = companies.queryAt(row);
	db.del(company);

	ModelRemoveGuard g(this, QModelIndex(), row, row);
	companies.removeAt(row);
}

void CompaniesModel::addNew(Company & company)
{
	db.insert(company);
	ModelInsertGuard g(this, QModelIndex(), companies.size(), companies.size());
	companies.add(new Company(company));
}

void CompaniesModel::update(int row, Company & modified)
{
	Company & orig = companies.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Company::NumOfFields-1));
}

void CompaniesModel::query()
{
		ModelResetGuard g(this);
		db.query(companies);
}

void CompaniesModel::sort(int column, bool ascending)
{
	if(companies.ascending == ascending && companies.ordering[0] == column)
		return;

		ModelResetGuard g(this);
		companies.ascending = ascending;
		companies.ordering.moveToFront(static_cast<Company::Fields>(column));
		companies.sort();
}
#endif
