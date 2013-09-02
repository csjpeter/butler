/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_H
#define BUTLER_ITEM_H

#include <math.h>
#include <float.h>

#include <csjp_sorter_owner_container.h>

#include <QDate>
#include <QString>

//#include <butler_tag.h>

class Item
{
public:
	QDateTime uploaded; /* non editable */
	QDateTime lastModified; /* non editable */
	QString name;
	QString category;
	double quantity; /* amount to buy or bought */
	QString comment;

	bool bought; /* shows whether the below fields have sense */

	QDateTime purchased; /* item considered bought if valid,
				default value should suggested by
				gui but editable */
	QString partner;
	double price; /* price of gross piece/amount quantity
		      expected price if not yet bought;
		      should set to 0 when bought to notify
		      to set the real paid price. */
	bool onStock; /* do we have it on store?
			when marked to no, advise to put on shopping list */

	enum Fields {
		Uploaded = 0,
		Name,
		Category,
		Quantity,
		Bought,
		Purchased,
		Partner,
		Price,
		OnStock,
		UnitPrice, /* virtual field (computed) */
		Comment,
		LastModified,
		NumOfFields
	};

public:
	Item() :
		uploaded(QDate(0,0,0), QTime(0,0,0)),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		name(""),
		category(),
		quantity(1),
		comment(""),
		bought(false),
		purchased(QDate(0,0,0), QTime(0,0,0)),
		partner(""),
		price(0),
		onStock(false)
	{
	}

	Item(const QString & _name) :
		uploaded(QDate(0,0,0), QTime(0,0,0)),
		lastModified(QDate(0,0,0), QTime(0,0,0)),
		name(_name),
		category(""),
		quantity(1),
		comment(""),
		bought(false),
		purchased(QDate(0,0,0), QTime(0,0,0)),
		partner(""),
		price(0),
		onStock(false)
	{
	}

	Item(const Item & i)
	{
		copy(i);
	}

	~Item()
	{
	}

	Item& operator=(const Item & i)
	{
		copy(i);
		return *this;
	}

	bool isEqual(const Item & i) const
	{
		if(		name != i.name ||
				category != i.category ||
				uploaded.toString() != i.uploaded.toString() ||
				lastModified.toString() != i.lastModified.toString() ||
				purchased.toString() != i.purchased.toString() ||
				partner != i.partner ||
				price != i.price ||
				quantity != i.quantity ||
				onStock != i.onStock ||
				comment != i.comment)
			return false;
		return true;
	}

	bool isLess(const Item & i) const
	{
		return uploaded < i.uploaded;
	}

	bool isLess(const QDateTime & dt) const
	{
		return uploaded < dt;
	}

	bool isMore(const QDateTime & dt) const
	{
		return dt < uploaded;
	}

	double unitPrice() const
	{
		//if(isnormal(quantity)) /*FIXME*/
		return price / quantity;
		//return 0;
	}

private:
	void copy(const Item & i)
	{
		uploaded = i.uploaded;
		lastModified = i.lastModified;
		name = i.name;
		category = i.category;
		quantity = i.quantity;
		comment = i.comment;

		bought = i.bought;
		purchased = i.purchased;
		partner = i.partner;
		price = i.price;
		onStock = i.onStock;
	}
};

inline bool operator==(const Item & a, const Item & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const Item & a, const Item & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const Item & a, const Item & b)
{
	return a.isLess(b);
}

inline bool operator<(const Item & a, const QDateTime & b)
{
	return a.isLess(b);
}

inline bool operator<(const QDateTime & a, const Item & b)
{
	return b.isMore(a);
}

class ItemSet : public csjp::SorterOwnerContainer<Item>
{
public: 
	Item::Fields ordering;
	bool ascending;
public:
	ItemSet() :
		csjp::SorterOwnerContainer<Item>(),
		ordering(Item::Name),
		ascending(true){}
	ItemSet(const ItemSet & is) :
		csjp::SorterOwnerContainer<Item>(is),
		ordering(Item::Name),
		ascending(true){}
	~ItemSet() {}

	Item& query(const QDateTime & dt) const {
		return csjp::SorterOwnerContainer<Item>::query<QDateTime>(dt);}

	bool has(const QDateTime & dt) const {
		return csjp::SorterOwnerContainer<Item>::has<QDateTime>(dt);}

	unsigned index(const QDateTime & dt) const {
		return csjp::SorterOwnerContainer<Item>::index<QDateTime>(dt);}

	virtual int compare(const Item & a, const Item & b) const
	{
		bool ret;

		switch(ordering) {
			case Item::Uploaded :
				ret = a.uploaded < b.uploaded;
				break;
			case Item::Name :
				ret = QString::localeAwareCompare(a.name, b.name) < 0;
				break;
			case Item::Category :
				ret = QString::localeAwareCompare(a.category, b.category) < 0;
				break;
			case Item::Quantity :
				ret = a.quantity < b.quantity;
				break;
			case Item::Comment :
				ret = QString::localeAwareCompare(a.comment, b.comment) < 0;
				break;
			case Item::Bought :
				ret = a.bought < b.bought;
				break;
			case Item::Purchased :
				ret = a.purchased < b.purchased;
				break;
			case Item::Partner :
				ret = QString::localeAwareCompare(a.partner, b.partner) < 0;
				break;
			case Item::Price :
				ret = a.price < b.price;
				break;
			case Item::OnStock :
				ret = a.onStock < b.onStock;
				break;
			case Item::UnitPrice :
				{
					double aVal = 0, bVal = 0;
					if(DBL_EPSILON <= a.quantity)
						aVal = a.price / a.quantity;
					if(DBL_EPSILON <= b.quantity)
						bVal = b.price / b.quantity;
					ret = aVal < bVal;
				}
				break;
			default:
				ret = a.uploaded < b.uploaded;
				break;
		}

		DBG("Compare: %s < %s = %d\n", C_STR(a.name), C_STR(b.name), ret);

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}
};

#endif
