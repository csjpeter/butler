/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ITEM_DB_H
#define BUTLER_ITEM_DB_H

#include "ButlerQuery"
#include "ButlerItemSet"

namespace Butler {

	class ItemDb
	{
		public:
			virtual bool insertItem(const Item &) = 0;
			virtual bool updateItem(
					const Item &orig,
					const Item &modified) = 0;
			virtual bool deleteItem(const Item &) = 0;

			/* a set of available tags with checked state of those
			 * tags with which the item is marked */
			virtual TagSet* queryItemTags(const Item &item) = 0;
			virtual ItemSet* queryItems(const Query &qo) = 0;
			/*	virtual bool buyItem(int id, int price) = 0;*/
	};

}

#endif

