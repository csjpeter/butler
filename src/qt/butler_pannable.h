/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNABLE_H
#define BUTLER_PANNABLE_H

#include <QWidget>
#include <QsKineticScroller.h>

/*forwards*/
class QScrollArea;

/** Note: this class is not owner of the received ptr, we just referencing it. */
template <typename Type>
class Pannable
{
public:
	Pannable(bool vertical = true, bool horizontal = false) :
		ptr(0),
		scroll(0, vertical, horizontal)
	{}

	Pannable(Type * ptr, bool vertical = true, bool horizontal = false) :
		ptr(ptr),
		scroll(0, vertical, horizontal)
	{
		scroll.enableKineticScrollFor(ptr);
	}

	~Pannable()
	{
		if(ptr)
			scroll.disableKineticScrollFor(ptr);
	}

	Type * operator->() const { return ptr; }
	Type *& operator&() { return ptr; }
	Pannable & operator=(Type * t)
	{
		if(ptr){
			scroll.disableKineticScrollFor(ptr);
			delete ptr;
		}
		ptr = t;
		if(ptr)
			scroll.enableKineticScrollFor(ptr);
		return *this;
	}

private:
	Type * ptr;

	QsKineticScroller scroll;
};

#endif
