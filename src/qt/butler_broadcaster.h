/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_BROADCASTER_H
#define BUTLER_BROADCASTER_H

#include "butler_signal.h"

/* Forwards: */
class QSessionManager;

namespace Butler {

class CloseApplicationListener
{
	public:
	virtual void saveStatelicationSlot() = 0;
};

class ItemChangeListener
{
	public:
	virtual void itemChangeSlot() = 0;
};

class Broadcaster :
	public Signal<ItemChangeListener>,
	public Signal<CloseApplicationListener>
{
public:
	static Broadcaster& instance();
	virtual ~Broadcaster();

	void notify(const ItemChangeListener *exception = NULL)
	{
		int i,s;
		s = Signal<ItemChangeListener>::set.size();
		for(i=0; i<s; i++){
			ItemChangeListener &listener = Signal<ItemChangeListener>::set.query(i);
			if(&listener != exception)
				listener.itemChangeSlot();
		}
	}

	void notify(const CloseApplicationListener *exception = NULL)
	{
		int i,s;
		s = Signal<CloseApplicationListener>::set.size();
		for(i=0; i<s; i++){
			CloseApplicationListener &listener = Signal<CloseApplicationListener>::set.query(i);
			if(&listener != exception)
				listener.saveStatelicationSlot();
		}
	}

protected:
	Broadcaster();
};

}

#endif

