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
	Pannable(Type * ptr = 0) : ptr(ptr), scroll(0) {}
	~Pannable() { }

	Type * operator->() const { return ptr; }
	Type *& operator&() { return ptr; }
	Pannable & operator=(Type * t)
	{
		if(ptr){
			scroll.disableKineticScroll();
			delete ptr; /* This already migth have been assigned to a parent. */
		}
		ptr = t;
		return *this;
	}
	void enablePanning() { scroll.enableKineticScrollFor(ptr); }

private:
	Type * ptr;

	QsKineticScroller scroll;
};

#endif
