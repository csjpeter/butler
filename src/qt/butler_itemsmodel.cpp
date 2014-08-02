/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <float.h>

#include <QtGui>

#include "butler_itemsmodel.h"

csjp::ReferenceContainer<ItemsModel> ItemsModel::itemOperationListeners;

const unsigned UnitPriceOffset = 0;

SCC TidItemFieldUploadDate	= QT_TRANSLATE_NOOP("ItemsModel", "Upload date");
SCC TidItemFieldCommonName	= QT_TRANSLATE_NOOP("ItemsModel", "Common name");
SCC TidItemFieldUnit		= QT_TRANSLATE_NOOP("ItemsModel", "Unit");
SCC TidItemFieldType		= QT_TRANSLATE_NOOP("ItemsModel", "Type");
SCC TidItemFieldBrand		= QT_TRANSLATE_NOOP("ItemsModel", "Brand");
SCC TidItemFieldQuantity	= QT_TRANSLATE_NOOP("ItemsModel", "Quantity");
SCC TidItemFieldPrice		= QT_TRANSLATE_NOOP("ItemsModel", "Price");
SCC TidItemFieldCurrency	= QT_TRANSLATE_NOOP("ItemsModel", "Currency");
SCC TidItemFieldAccount		= QT_TRANSLATE_NOOP("ItemsModel", "Account");
SCC TidItemFieldPartner		= QT_TRANSLATE_NOOP("ItemsModel", "Partner");
SCC TidItemFieldInventory	= QT_TRANSLATE_NOOP("ItemsModel", "Inventory");
SCC TidItemFieldComment		= QT_TRANSLATE_NOOP("ItemsModel", "Comment");
SCC TidItemFieldInvChangeDate	= QT_TRANSLATE_NOOP("ItemsModel", "Change date");
SCC TidItemFieldUnitPrice	= QT_TRANSLATE_NOOP("ItemsModel", "Unit price");

ItemsModel::ItemsModel(ItemDb & db, const WaresModel & wmodel) :
	db(db),
	wmodel(wmodel)
{
	itemOperationListeners.add(*this);
	opts.endDate = DateTime::now();
	opts.tagOption = Query::TagOptions::MatchAny;
}

ItemsModel::~ItemsModel()
{
	/* Since ItemsModel instances are held in a static storage class container somewhere,
	 * it might happen on destruction time, that itemOperationListener (which is also a
	 * static storage class object) got destroyed before some Itemsmodel instances. */
	if(itemOperationListeners.has(*this))
		itemOperationListeners.remove(*this);
}

QModelIndex ItemsModel::index(	int row,
		int column,
		const QModelIndex & parent) const
{
	return QAbstractTableModel::index(row, column, parent);
}

Qt::ItemFlags ItemsModel::flags(const QModelIndex & index) const
{
	if(index.row() < (int)items.size() && index.column() < Item::NumOfFields){
		if(index.column() != Item::UploadDate)
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
		else
			return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else if(index.row() < (int)items.size() &&
			index.column() < Item::NumOfFields + 0){
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	} else
		return Qt::NoItemFlags;
}

QVariant ItemsModel::data(const QModelIndex & index, int role) const 
{
	QVariant value;

	if(!index.isValid())
		return value;

	if(index.parent().isValid())
		return value;

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return value;

	if((int)items.size() <= index.row())
		return value;

	switch(index.column()){
		case Item::UploadDate :
			return QVariant(items.queryAt(index.row()).uploadDate);
			break;
		case Item::Name :
			return QVariant(items.queryAt(index.row()).name);
			break;
		case Item::Unit :
			return QVariant(items.queryAt(index.row()).unit);
			break;
		case Item::Type :
			return QVariant(items.queryAt(index.row()).type);
			break;
		case Item::Brand :
			return QVariant(items.queryAt(index.row()).brand);
			break;
		case Item::Quantity :
			{
				Item & item = items.queryAt(index.row());
				int i = wmodel.index(item.name);
				QString val;
				val.setNum(item.quantity, 'g', 3);
				if(i != -1) {
					const Ware & w = wmodel.ware(i);
					val += " ";
					val += w.unit;
				}
				return QVariant(val);
			}
			break;
		case Item::Price :
			return QVariant(items.queryAt(index.row()).price);
			break;
		case Item::Currency :
			return QVariant(items.queryAt(index.row()).currency);
			break;
		case Item::Account :
			return QVariant(items.queryAt(index.row()).account);
			break;
		case Item::Partner :
			return QVariant(items.queryAt(index.row()).partner);
			break;
		case Item::Inventory :
			return QVariant(items.queryAt(index.row()).inventory);
			break;
		case Item::Comment :
			return QVariant(items.queryAt(index.row()).comment);
			break;
		case Item::InvChangeDate :
			return QVariant(items.queryAt(index.row()).invChangeDate);
			break;
		case Item::NumOfFields + UnitPriceOffset :
			{
				Item & item = items.queryAt(index.row());
				double unitPrice = 0;
				//if(isnormal(item.quantity)) /*FIXME*/
				unitPrice = item.price / item.quantity;
				return QVariant(unitPrice);
			}
			break;
		default :
			return value;
	}

	return value;
}

QVariant ItemsModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("");

	switch(section){
		case Item::UploadDate :
			return QVariant(tr(TidItemFieldUploadDate));
			break;
		case Item::Name :
			return QVariant(tr(TidItemFieldCommonName));
			break;
		case Item::Unit :
			return QVariant(tr(TidItemFieldUnit));
			break;
		case Item::Type :
			return QVariant(tr(TidItemFieldType));
			break;
		case Item::Brand :
			return QVariant(tr(TidItemFieldBrand));
			break;
		case Item::Quantity :
			return QVariant(tr(TidItemFieldQuantity));
			break;
		case Item::Price :
			return QVariant(tr(TidItemFieldPrice));
			break;
		case Item::Currency :
			return QVariant(tr(TidItemFieldCurrency));
			break;
		case Item::Account :
			return QVariant(tr(TidItemFieldAccount));
			break;
		case Item::Partner :
			return QVariant(tr(TidItemFieldPartner));
			break;
		case Item::Inventory :
			return QVariant(tr(TidItemFieldInventory));
			break;
		case Item::Comment :
			return QVariant(tr(TidItemFieldComment));
			break;
		case Item::InvChangeDate :
			return QVariant(tr(TidItemFieldInvChangeDate));
			break;
		case Item::NumOfFields + UnitPriceOffset :
			return QVariant(tr(TidItemFieldUnitPrice));
			break;
		default :
			return QVariant();
	}

	return QVariant();
}

bool ItemsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)items.size() <= index.row())
		return false;

	int row = index.row();
	Item modified(items.queryAt(row));

	switch(index.column()){
		case Item::UploadDate :
			modified.uploadDate <<= value;
			update(row, modified);
			break;
		case Item::Name :
			modified.name <<= value;
			update(row, modified);
			break;
		case Item::Unit :
			modified.unit <<= value;
			update(row, modified);
			break;
		case Item::Type :
			modified.type <<= value;
			update(row, modified);
			break;
		case Item::Brand :
			modified.brand <<= value;
			update(row, modified);
			break;
		case Item::Quantity :
			modified.quantity <<= value;
			update(row, modified);
			break;
		case Item::Price :
			modified.price <<= value;
			update(row, modified);
			break;
		case Item::Currency :
			modified.currency <<= value;
			update(row, modified);
			break;
		case Item::Account :
			modified.account <<= value;
			update(row, modified);
			break;
		case Item::Partner :
			modified.partner <<= value;
			update(row, modified);
			break;
		case Item::Inventory :
			modified.inventory <<= value;
			update(row, modified);
			break;
		case Item::Comment :
			modified.comment <<= value;
			update(row, modified);
			break;
		case Item::InvChangeDate :
			modified.invChangeDate <<= value;
			update(row, modified);
			break;
		default :
			return false;
	}

	//dataChanged(index, index);
	return true;
}

bool ItemsModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int ItemsModel::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return items.size();
}

int ItemsModel::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return Item::NumOfFields;
}

bool ItemsModel::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool ItemsModel::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void ItemsModel::sort(int column, Qt::SortOrder order)
{
	sort(column, order == Qt::AscendingOrder);
}

void ItemsModel::query()
{
	ModelResetGuard g(this);
	db.query(opts, stat, items);
}

int ItemsModel::index(const QDateTime & uploadDate) const
{
	DateTime dt(uploadDate);
	if(items.has(dt))
		return items.index(dt);
	else
		return -1;
}

const Item& ItemsModel::item(int row) const
{
	return items.queryAt(row);
}

void ItemsModel::del(int row)
{
	Item & item = items.queryAt(row);
	db.del(item);
	itemRemoved(db, item);
}

void ItemsModel::itemRemoved(const ItemDb & db, const Item & removed)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++)
		itemOperationListeners.queryAt(i).itemRemovedListener(db, removed);
}

void ItemsModel::itemRemovedListener(const ItemDb & db, const Item & removed)
{
	if(& db != &(this->db))
		return;

	if(!items.has(removed.uploadDate))
		return;

	int row = items.index(removed.uploadDate);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	items.removeAt(row);
}

void ItemsModel::addNew(Item & item)
{
	db.insert(item);
	itemChange(db, item);
}

void ItemsModel::update(int row, Item & modified)
{
	Item & orig = items.queryAt(row);

	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, Item::NumOfFields-1));
	itemChange(db, modified);
}

void ItemsModel::itemChange(const ItemDb & db, const Item & modified)
{
	int s = itemOperationListeners.size();
	for(int i=0; i<s; i++)
		itemOperationListeners.queryAt(i).itemChangeListener(db, modified);
}

bool ItemsModel::queryFilter(const Item & modified)
{
	(void)(modified);
	return true;
}

void ItemsModel::itemChangeListener(const ItemDb & db, const Item & modified)
{
	if(& db != &(this->db))
		return;

	bool want = queryFilter(modified);
	if(items.has(modified.uploadDate)){
		int row = items.index(modified.uploadDate);
		if(want){
			items.queryAt(row) = modified;
			dataChanged(index(row, 0), index(row, Item::NumOfFields-1));
		} else {
			ModelRemoveGuard g(this, QModelIndex(), row, row);
			items.removeAt(row);
		}
	} else {
		if(want){
			ModelInsertGuard g(this, QModelIndex(), items.size(), items.size());
			items.add(new Item(modified));
		}
	}
}

void ItemsModel::sort(int column, bool ascending)
{
	if(items.ascending == ascending && items.ordering == column)
		return;

	ModelResetGuard g(this);
	items.ascending = ascending;
	items.ordering = static_cast<Item::Fields>(column);
	items.sort();
}

bool operator<(const ItemsModel & a, const ItemsModel & b)
{
	return & a < & b;
}
