/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include "butler_item.h"

namespace Butler {

	Item::Item()
	{
	}

	Item::Item(const Item &item) : QObject()
	{
		name = item.name;
		uploaded = item.uploaded;
		expectedPrice = item.expectedPrice;
		purchased = item.purchased;
		paidPrice = item.paidPrice;
		amortYears = item.amortYears;
		amortMonths = item.amortMonths;
		amortDays = item.amortDays;
		comment = item.comment;
		tags = item.tags;
	}

	Item::~Item()
	{
	}

	bool Item::isEqual(const Item &a, const Item &b)
	{
		if(a.name != b.name) return false;
		if(a.uploaded != b.uploaded) return false;
		if(a.expectedPrice != b.expectedPrice) return false;
		if(a.purchased != b.purchased) return false;
		if(a.paidPrice != b.paidPrice) return false;
		if(a.amortYears != b.amortYears) return false;
		if(a.amortMonths != b.amortMonths) return false;
		if(a.amortDays != b.amortDays) return false;
		if(a.comment != b.comment) return false;
		if(a.tags != b.tags) return false;
		return true;
	}
	
	bool operator==(const Item &a, const Item &b)
	{
		return Item::isEqual(a,b);
	}
}

