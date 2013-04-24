/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_KINETICSCROLLER_H
#define BUTLER_KINETICSCROLLER_H

#include <QtGui>

#include <csjp_logger.h>

#include <csjp_owner_container.h>

#include <butler_config.h>

class KineticScroller: public QObject
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	/* scrollArea must outlive a QSKineticScroller */
	KineticScroller(QAbstractScrollArea * scrollArea, QObject* parent = 0);
	~KineticScroller();

public:
	bool eventHandler(QObject* object, QEvent* event);

private:
	double computeSpeed(double currPos, double lastPos, double lastSpeed);
	double doScroll( /* Returns the computed new scroll position. */
		double origPixPos, double currPixPos, double contentPixSize,
		double origScrollPos, QScrollBar * scroller);
	void stopIfAtEnd();

private slots:
	void onSpeedTimerElapsed(); /* Compute/Update the speed on speedTimer tick. */
	void onKineticTimerElapsed();

private:
	QAbstractScrollArea* scrollArea;
	bool scrolled;
	bool manualStop;
	bool alreadyFocused; /* For widget had focus before press disable kinetic scroll. */

	QPoint pressedScrollBarPosition;
	QPoint pressedMousePosition;
	QPoint lastMousePos;
	QVector2D speed;
	QTimer speedTimer; /* should be active iff mouse button is pressed */
	QTimer kineticTimer;

	/* Copy of QScrollBar's value. This is redundancy but we need it to be real number. */
	QVector2D computedScrollBarPosition;
public:
	bool stealEventFromFocusedWidgets;
};

class ObjectScrollerPair
{
public:
	ObjectScrollerPair(QAbstractScrollArea * scrollArea, KineticScroller * scroller) :
		scrollArea(scrollArea), scroller(scroller) {}
	QAbstractScrollArea * scrollArea;
	KineticScroller * scroller;
};

inline bool operator<(const QObject * a, const ObjectScrollerPair & b)
	{ return a < b.scrollArea; }
inline bool operator<(const ObjectScrollerPair & a, const QObject * b)
	{ return a.scrollArea < b; }
inline bool operator<(const ObjectScrollerPair & a, const ObjectScrollerPair & b)
	{ return a.scrollArea < b.scrollArea; }

extern csjp::OwnerContainer<ObjectScrollerPair> kineticScrollers;

#endif
