/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_partnersmodel.h"

SCC TidPartnerFieldName		= QT_TRANSLATE_NOOP("PartnersModel", "Name");
SCC TidPartnerFieldCountry	= QT_TRANSLATE_NOOP("PartnersModel", "Country");
SCC TidPartnerFieldCity		= QT_TRANSLATE_NOOP("PartnersModel", "City");
SCC TidPartnerFieldPostalCode	= QT_TRANSLATE_NOOP("PartnersModel", "Postal code");
SCC TidPartnerFieldAddress	= QT_TRANSLATE_NOOP("PartnersModel", "Address");
SCC TidPartnerFieldCompany	= QT_TRANSLATE_NOOP("PartnersModel", "Company");
SCC TidPartnerFieldStoreName	= QT_TRANSLATE_NOOP("PartnersModel", "Store/Agent");

PartnersModel::PartnersModel(Db & db) :
	db(db)
{
	query();
}

PartnersModel::~PartnersModel()
{
}

QModelIndex PartnersModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags PartnersModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)partners.size() && index.column() < Partner::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant PartnersModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)partners.size() <= index.row())
		return QVariant();

	switch(index.column()){
		case Partner::Name :
			return QVariant(partners.queryAt(index.row()).name);
			break;
		case Partner::Country :
			return QVariant(partners.queryAt(index.row()).country);
			break;
		case Partner::City :
			return QVariant(partners.queryAt(index.row()).city);
			break;
		case Partner::PostalCode :
			return QVariant(partners.queryAt(index.row()).postalCode);
			break;
		case Partner::Address :
			return QVariant(partners.queryAt(index.row()).address);
			break;
		case Partner::Company :
			return QVariant(partners.queryAt(index.row()).company);
			break;
		case Partner::StoreName :
			return QVariant(partners.queryAt(index.row()).storeName);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant PartnersModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("");

	switch(section){
		case Partner::Name :
			return QVariant(tr(TidPartnerFieldName));
			break;
		case Partner::Country :
			return QVariant(tr(TidPartnerFieldCountry));
			break;
		case Partner::City :
			return QVariant(tr(TidPartnerFieldCity));
			break;
		case Partner::PostalCode :
			return QVariant(tr(TidPartnerFieldPostalCode));
			break;
		case Partner::Address :
			return QVariant(tr(TidPartnerFieldAddress));
			break;
		case Partner::Company :
			return QVariant(tr(TidPartnerFieldCompany));
			break;
		case Partner::StoreName :
			return QVariant(tr(TidPartnerFieldStoreName));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool PartnersModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)partners.size() <= index.row())
		return false;

	int row = index.row();
	Partner modified(partners.queryAt(row));

	switch(index.column()){
		case Partner::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case Partner::Country :
			modified.country = value.toString();
			update(row, modified);
			break;
		case Partner::City :
			modified.city = value.toString();
			update(row, modified);
			break;
		case Partner::PostalCode :
			modified.postalCode = value.toString();
			update(row, modified);
			break;
		case Partner::Address :
			modified.address = value.toString();
			update(row, modified);
			break;
		case Partner::Company :
			modified.company = value.toString();
			update(row, modified);
			break;
		case Partner::StoreName :
			modified.storeName = value.toString();
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool PartnersModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int PartnersModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return partners.size();
}

int PartnersModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Partner::NumOfFields;
}

bool PartnersModel::removeRows(
		int row, int count, const QModelIndex &parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool PartnersModel::insertRows(
		int row, int count, const QModelIndex &parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void PartnersModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int PartnersModel::index(const QString &name) const
{
	if(partners.has(name))
		return partners.index(name);
	else
		return -1;
}

const Partner& PartnersModel::partner(int row)
{
	return partners.queryAt(row);
}

void PartnersModel::del(int row)
{
	Partner &partner = partners.queryAt(row);
	db.partner.del(partner);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	partners.removeAt(row);
}

void PartnersModel::addNew(Partner &partner)
{
	db.partner.insert(partner);
	ModelInsertGuard g(this, QModelIndex(), partners.size(), partners.size());
	partners.add(new Partner(partner));
}

void PartnersModel::update(int row, Partner &modified)
{
	Partner &orig = partners.queryAt(row);
	db.partner.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Partner::NumOfFields-1));
}

void PartnersModel::query()
{
	ModelResetGuard g(this);
	db.partner.query(partners);
}

void PartnersModel::sort(int column, bool ascending)
{
	if(partners.ascending == ascending && partners.ordering == column)
		return;

	ModelResetGuard g(this);
	partners.ascending = ascending;
	partners.ordering = static_cast<Partner::Fields>(column);
	partners.sort();
}
