/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_shopsmodel.h"

SCC TidPartnerFieldName		= QT_TRANSLATE_NOOP("PartnersModel", "Name");
SCC TidPartnerFieldCity		= QT_TRANSLATE_NOOP("PartnersModel", "City");
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
	if(index.row() < (int)partners.size() && index.column() < Shop::NumOfFields){
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
		case Shop::Name :
			return QVariant(partners.queryAt(index.row()).name);
			break;
		case Shop::StoreName :
			return QVariant(partners.queryAt(index.row()).storeName);
			break;
		case Shop::City :
			return QVariant(partners.queryAt(index.row()).city);
			break;
		case Shop::Address :
			return QVariant(partners.queryAt(index.row()).address);
			break;
		case Shop::Company :
			return QVariant(partners.queryAt(index.row()).company);
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
		case Shop::Name :
			return QVariant(tr(TidPartnerFieldName));
			break;
		case Shop::StoreName :
			return QVariant(tr(TidPartnerFieldStoreName));
			break;
		case Shop::City :
			return QVariant(tr(TidPartnerFieldCity));
			break;
		case Shop::Address :
			return QVariant(tr(TidPartnerFieldAddress));
			break;
		case Shop::Company :
			return QVariant(tr(TidPartnerFieldCompany));
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
	Shop modified(partners.queryAt(row));

	switch(index.column()){
		case Shop::Name :
			modified.name = value.toString();
			update(row, modified);
			break;
		case Shop::StoreName :
			modified.storeName = value.toString();
			update(row, modified);
			break;
		case Shop::City :
			modified.city = value.toString();
			update(row, modified);
			break;
		case Shop::Address :
			modified.address = value.toString();
			update(row, modified);
			break;
		case Shop::Company :
			modified.company = value.toString();
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

	return Shop::NumOfFields;
}

bool PartnersModel::removeRows(
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

bool PartnersModel::insertRows(
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

const Shop& PartnersModel::partner(int row)
{
	return partners.queryAt(row);
}

void PartnersModel::del(int row)
{
	Shop &partner = partners.queryAt(row);
	db.partner.del(partner);
	try {
		beginRemoveRows(QModelIndex(), row, row);
		partners.removeAt(row);
		endRemoveRows();
	} catch (...) {
		endRemoveRows();
		throw;
	}
}

void PartnersModel::addNew(Shop &partner)
{
	db.partner.insert(partner);
	try {
		beginInsertRows(QModelIndex(), partners.size(), partners.size());
		partners.add(new Shop(partner));
		endInsertRows();
	} catch (...) {
		endInsertRows();
		throw;
	}
}

void PartnersModel::update(int row, Shop &modified)
{
	Shop &orig = partners.queryAt(row);
	db.partner.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Shop::NumOfFields-1));
}

void PartnersModel::query()
{
	try {
		beginResetModel();
		db.partner.query(partners);
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}

void PartnersModel::sort(int column, bool ascending)
{
	if(partners.ascending == ascending && partners.ordering == column)
		return;

	try {
		beginResetModel();
		partners.ascending = ascending;
		partners.ordering = static_cast<Shop::Fields>(column);
		partners.sort();
		endResetModel();
	} catch (...) {
		endResetModel();
		throw;
	}
}
