/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_BROADCASTER_H
#define BUTLER_BROADCASTER_H

#include "butler_signal.h"

/* Forwards: */
class QSessionManager;

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

#endif
