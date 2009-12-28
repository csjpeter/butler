/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item.h"

namespace Butler {

	Item::Item() :
		name(""),
		uploaded(0,0,0),
		expectedPrice(0),
		purchased(0,0,0),
		paidPrice(0),
		amortYears(0),
		amortMonths(0),
		amortDays(0),
		comment("")
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	Item::Item(const QString &_name) : QObject(),
		name(_name),
		uploaded(0,0,0),
		expectedPrice(0),
		purchased(0,0,0),
		paidPrice(0),
		amortYears(0),
		amortMonths(0),
		amortDays(0),
		comment("")
	{
		ENTER_CONSTRUCTOR();
		LEAVE_CONSTRUCTOR();
	}

	Item::Item(const Item &i) : QObject()
	{
		ENTER_CONSTRUCTOR();
		equal(i);
		LEAVE_CONSTRUCTOR();
	}

	Item::~Item()
	{
		ENTER_DESTRUCTOR();
		LEAVE_DESTRUCTOR();
	}

	Item& Item::operator=(const Item &i)
	{
		ENTER_FUNCTION();
		equal(i);
		LEAVE_FUNCTION();
		return *this;
	}

	bool Item::isEqual(const Item &i) const
	{
		ENTER_FUNCTION();
		if(name != i.name) return false;
		if(uploaded != i.uploaded) return false;
		if(expectedPrice != i.expectedPrice) return false;
		if(purchased != i.purchased) return false;
		if(paidPrice != i.paidPrice) return false;
		if(amortYears != i.amortYears) return false;
		if(amortMonths != i.amortMonths) return false;
		if(amortDays != i.amortDays) return false;
		if(comment != i.comment) return false;

		if(tags != i.tags) return false;

		LEAVE_FUNCTION();
		return true;
	}

	bool Item::isLess(const Item &i) const
	{
		ENTER_FUNCTION();
		bool ret = uploaded < i.uploaded;
		LEAVE_FUNCTION();
		return ret;
	}

	bool Item::isMore(const Item &i) const
	{
		ENTER_FUNCTION();
		bool ret = uploaded > i.uploaded;
		LEAVE_FUNCTION();
		return ret;
	}

	void Item::equal(const Item &i)
	{
		ENTER_FUNCTION();
		name = i.name;
		uploaded = i.uploaded;
		expectedPrice = i.expectedPrice;
		purchased = i.purchased;
		paidPrice = i.paidPrice;
		amortYears = i.amortYears;
		amortMonths = i.amortMonths;
		amortDays = i.amortDays;
		comment = i.comment;

		tags = i.tags;
		LEAVE_FUNCTION();
	}

	bool operator==(const Item &a, const Item &b)
	{
		ENTER_STATIC_FUNCTION();
		return a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
	}

	bool operator!=(const Item &a, const Item &b)
	{
		ENTER_STATIC_FUNCTION();
		return !a.isEqual(b);
		LEAVE_STATIC_FUNCTION();
	}

	bool operator<(const Item &a, const Item &b)
	{
		ENTER_STATIC_FUNCTION();
		return a.isLess(b);
		LEAVE_STATIC_FUNCTION();
	}

	bool operator>(const Item &a, const Item &b)
	{
		ENTER_STATIC_FUNCTION();
		return a.isMore(b);
		LEAVE_STATIC_FUNCTION();
	}
}

