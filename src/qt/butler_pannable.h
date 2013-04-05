/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNABLE_H
#define BUTLER_PANNABLE_H

#include <QtGui>
#include <butler_kineticscroller.h>

/* FIXME delete this class asap */
/** Note: this class is not owner of the received ptr, we just referencing it. */
template <typename Type>
class Pannable
{
public:
	Pannable(Type * ptr = 0) : ptr(ptr), scroller(ptr) {}
	~Pannable() { }

	Type * operator->() const { return ptr; }
	Type *& operator&() { return ptr; }
	Pannable & operator=(Type * t)
	{
		if(ptr)
			delete ptr; /* This already migth have been assigned to a parent. */
		ptr = t;
		return *this;
	}

private:
	Type * ptr;

	KineticScroller scroller;
};

#endif
