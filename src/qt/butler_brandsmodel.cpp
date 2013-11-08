/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

//#define DEBUG
#include <csjp_logger.h>
#undef DEBUG

#include <QtGui>

#include "butler_brandsmodel.h"

SCC TidBrandFieldName = QT_TRANSLATE_NOOP("BrandsModel", "Name");
SCC TidBrandFieldCompany = QT_TRANSLATE_NOOP("BrandsModel", "Company");

BrandsModel::BrandsModel(BrandDb & db) :
	db(db)
{
	query();
}

BrandsModel::~BrandsModel()
{
}

QModelIndex BrandsModel::index(int row, int column, const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags BrandsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)brands.size() && index.column() < Brand::NumOfFields){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant BrandsModel::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)brands.size() <= index.row())
		return QVariant();
	
	QString result;

	switch(index.column()){
		case Brand::Name :
			return QVariant(brands.queryAt(index.row()).name);
			break;
		case Brand::Company :
			return QVariant(brands.queryAt(index.row()).company);
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant BrandsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
		case Brand::Name :
			return QVariant(tr(TidBrandFieldName));
			break;
		case Brand::Company :
			return QVariant(tr(TidBrandFieldCompany));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool BrandsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)brands.size() <= index.row())
		return false;

	int row = index.row();
	Brand modified(brands.queryAt(row));

	QString str;

	switch(index.column()){
		case Brand::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Brand::Company :
			modified.company <<= value;
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool BrandsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int BrandsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return brands.size();
}

int BrandsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Brand::NumOfFields;
}

bool BrandsModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool BrandsModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void BrandsModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

int BrandsModel::index(const Text & name) const
{
	if(brands.has(name))
		return brands.index(name);
	else
		return -1;
}

const Brand& BrandsModel::brand(int row) const
{
	return brands.queryAt(row);
}

void BrandsModel::del(int row)
{
	Brand & brand = brands.queryAt(row);
	db.del(brand);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	brands.removeAt(row);
}

void BrandsModel::addNew(Brand & brand)
{
	db.insert(brand);
	ModelInsertGuard g(this, QModelIndex(), brands.size(), brands.size());
	brands.add(new Brand(brand));
}

void BrandsModel::update(int row, Brand & modified)
{
	Brand & orig = brands.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Brand::NumOfFields-1));
}

void BrandsModel::query()
{
	ModelResetGuard g(this);
	db.query(brands);
}

void BrandsModel::sort(int column, bool ascending)
{
	if(brands.ascending == ascending && brands.ordering == column)
		return;

	ModelResetGuard g(this);
	brands.ascending = ascending;
	brands.ordering = static_cast<Brand::Fields>(column);
	brands.sort();
}
