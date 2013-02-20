/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_H
#define BUTLER_ITEM_H

#include <QDate>
#include <QString>

#include <butler_tag_set.h>

class Tag;

class Item
{
public:
	Item();
	explicit Item(const QString &);
	explicit Item(const Item &);
	~Item();
	
	Item& operator=(const Item &);

	bool isEqual(const Item &) const;

	bool isLess(const Item &) const;

	bool isLess(const QDateTime &dt) const;
	bool isMore(const QDateTime &dt) const;

public:
	QDateTime uploaded; /* non editable */
	QString name;
	QString category;
	double quantity; /* amount to buy or bought */
	QString comment;

	bool bought; /* shows whether the below fields have sense */

	QDateTime purchased; /* item considered bought if valid,
				default value should suggested by
				gui but editable */
	QString shop;
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
		Comment,
		Bought,
		Purchased,
		Shop,
		Price,
		OnStock,
		UnitPrice,
		NumOfFields
	};
private:
	void copy(const Item &);
};

bool operator==(const Item &a, const Item &b);
bool operator!=(const Item &a, const Item &b);

bool operator<(const Item &a, const Item &b);
bool operator<(const Item &a, const QDateTime &b);
bool operator<(const QDateTime &a, const Item &b);

#endif

