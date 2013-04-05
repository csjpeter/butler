// Copyright (c) 2011, Razvan Petru
// All rights reserved.

// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:

// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice, this
//   list of conditions and the following disclaimer in the documentation and/or other
//   materials provided with the distribution.
// * The name of the contributors may not be used to endorse or promote products
//   derived from this software without specific prior written permission.

// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
// OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

// The original code is improved much by Peter Csaszar <csjpeter@gmail.com>
// All rights are reserved for the applicable improvements.
// Copyright (C) 2013 Peter Csaszar
// The license shall remain the original given by Razvan Petru.

#include <math.h>

#include "QsKineticScroller.h"

// http://blog.codeimproved.net/2010/12/kinetic-scrolling-with-qt-the-what-and-the-how/

static const int gThresholdScrollDistance = 20; /* Distance from presspos, threshold for scroll. */

static const int gTimerInterval = 40; // milisec periodic time to check move speed && set scroll pos
static const double gDecceleration = 0.05;

QsKineticScroller::QsKineticScroller(QObject *parent) :
	QObject(parent),
	scrollArea(0),
	scrolled(0),
	manualStop(false)
{
	connect(&speedTimer, SIGNAL(timeout()), SLOT(onSpeedTimerElapsed()));
	connect(&kineticTimer, SIGNAL(timeout()), SLOT(onKineticTimerElapsed()));
}

QsKineticScroller::~QsKineticScroller()
{
	if(scrollArea)
		disableKineticScroll();
}

void QsKineticScroller::disableKineticScroll()
{
	if(!scrollArea)
		return;

	QList<QWidget*> widgets = scrollArea->findChildren<QWidget*>();
	Q_FOREACH(QWidget *widget, widgets)
		widget->removeEventFilter(this);

	scrollArea->removeEventFilter(this);
	disconnect(scrollArea, SIGNAL(destroyed()), this, SLOT(disableKineticScroll()));
	scrollArea = 0;
}

void QsKineticScroller::enableKineticScrollFor(QAbstractScrollArea* newScrollArea)
{
	ENSURE(newScrollArea != 0, csjp::LogicError);

	if(scrollArea)
		disableKineticScroll();
	scrollArea = newScrollArea;
	connect(scrollArea, SIGNAL(destroyed()), this, SLOT(disableKineticScroll()));

	scrollArea->installEventFilter(this);

	QList<QWidget*> widgets = scrollArea->findChildren<QWidget*>();
	Q_FOREACH(QWidget *widget, widgets)
		widget->installEventFilter(this);
}

double QsKineticScroller::computeSpeed(double currPos, double lastPos, double lastSpeed)
{
	double newSpeed = currPos - lastPos;

	if(newSpeed * lastSpeed < 0) /* Change of direction restarts the motion. */
		lastSpeed = 0; 

	/* For longer motions we want balanced speed computation. */
	if(qAbs(gDecceleration) < qAbs(lastSpeed))
		newSpeed = (newSpeed + lastSpeed) / 2;

	return newSpeed;
}

void QsKineticScroller::onSpeedTimerElapsed()
{
	const QPoint cursorPos = QCursor::pos();
/*
	DBG("Vert scroll min %d, max %d, page %d",
			scrollArea->verticalScrollBar()->minimum(),
			scrollArea->verticalScrollBar()->maximum(),
			scrollArea->verticalScrollBar()->pageStep());
	DBG("Horiz scroll min %d, max %d, page %d",
			scrollArea->horizontalScrollBar()->minimum(),
			scrollArea->horizontalScrollBar()->maximum(),
			scrollArea->horizontalScrollBar()->pageStep());
*/
	speed.setX(computeSpeed(cursorPos.x(), lastMousePos.x(), speed.x()));
	speed.setY(computeSpeed(cursorPos.y(), lastMousePos.y(), speed.y()));
	
//	DBG("[scroll] speed %.2f, %.2f", speed.x(), speed.y());

	lastMousePos = cursorPos;
}

double QsKineticScroller::doScroll(
		double origPixPos, double currPixPos, double contentSize,
		double origScrollPos, QScrollBar * scroll)
{
	double scrollUnitInPixels = contentSize / scroll->pageStep();
	double moveInPixels = currPixPos - origPixPos;
	double moveInScrollUnits = moveInPixels / scrollUnitInPixels;
	double newScrollPos = origScrollPos - moveInScrollUnits;
/*	DBG("%.2f - %.2f - %.2f - %.2f",
			scrollUnitInPixels, moveInPixels, moveInScrollUnits, newScrollPos);*/
	scroll->setValue(newScrollPos);
	return newScrollPos;
}

void QsKineticScroller::stopIfAtEnd()
{
	int pos = scrollArea->verticalScrollBar()->value();
	if(		scrollArea->verticalScrollBar()->minimum() == pos ||
			scrollArea->verticalScrollBar()->maximum() == pos)
		speed.setY(0);
	pos = scrollArea->horizontalScrollBar()->value();
	if(		scrollArea->horizontalScrollBar()->minimum() == pos ||
			scrollArea->horizontalScrollBar()->maximum() == pos)
		speed.setX(0);
}

//! intercepts mouse events to make the scrolling work
bool QsKineticScroller::eventFilter(QObject* obj, QEvent* event)
{
	if(!scrollArea)
		return false;

	const QEvent::Type eventType = event->type();
	if(		eventType != QEvent::MouseButtonPress &&
			eventType != QEvent::MouseButtonRelease &&
			eventType != QEvent::MouseMove)
		return false;

	QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);

#ifdef DEBUG
	//const QPoint pos = QCursor::pos();
	QPoint pos = mouseEvent->globalPos();
	DBG("QsKineticScroller::eventFilter mouse event at %d, %d", pos.x(), pos.y());
#endif

	switch(eventType){
		case QEvent::MouseButtonPress:
			manualStop = !speed.isNull();
			speed.setX(0);
			speed.setY(0);
			pressedScrollBarPosition.setX(scrollArea->horizontalScrollBar()->value());
			pressedScrollBarPosition.setY(scrollArea->verticalScrollBar()->value());
			lastMousePos = pressedMousePosition = mouseEvent->globalPos();
			scrolled = 0;
			speedTimer.start(gTimerInterval);
			return false;
		case QEvent::MouseMove:
			if(!speedTimer.isActive())
				return false; /* We have nothing to do with move without press. */

			/* If for some reason we missed the release event, on next pressless move
			 * lets not do any scrolling. */
			if(mouseEvent->buttons() == Qt::NoButton){
				speedTimer.stop();
				return false;
			}

			if(sqrt(pow(pressedMousePosition.x() - mouseEvent->globalPos().x(), 2) +
				pow(pressedMousePosition.y() - mouseEvent->globalPos().y(), 2)) <
					gThresholdScrollDistance){
				QApplication::postEvent(obj, new QFocusEvent(QEvent::FocusOut),
						Qt::HighEventPriority);
				return true;
			}

			scrolled = true;

			computedScrollBarPosition.setY(doScroll(
					pressedMousePosition.y(),
					mouseEvent->globalPos().y(),
					/* FIXME need scrollarea's inner view height */
					scrollArea->height(),
					pressedScrollBarPosition.y(),
					scrollArea->verticalScrollBar()));

			computedScrollBarPosition.setX(doScroll(
					pressedMousePosition.x(),
					mouseEvent->globalPos().x(),
					/* FIXME need scrollarea's inner view width */
					scrollArea->width(),
					pressedScrollBarPosition.x(),
					scrollArea->horizontalScrollBar()));

			stopIfAtEnd();

			return true;
		case QEvent::MouseButtonRelease:
			speedTimer.stop();

			if(!speed.isNull()){
				kineticTimer.start(gTimerInterval);
				return true;
			}

			if(scrolled || manualStop)
				return true;

			return false;
		default:
			DBG("DEFAULT EVENT");
			return false;
	}
}

void QsKineticScroller::onKineticTimerElapsed()
{
	if(!scrollArea)
		return;
	if(speed.isNull()){
		kineticTimer.stop();
		return;
	}

	if(qAbs(gDecceleration) < qAbs(speed.y())){
		computedScrollBarPosition.setY(doScroll(
			/* The differe, which shall be speed.y() is the essence. */
			0, speed.y(),
			/* FIXME need scrollarea's inner view height */
			scrollArea->height(),
			computedScrollBarPosition.y(),
			scrollArea->verticalScrollBar()));

		/* now reduce the speed */
		if(0 < speed.y())
			speed.setY(speed.y() - gDecceleration);
		else
			speed.setY(speed.y() + gDecceleration);
	} else
		speed.setY(0);

	if(qAbs(gDecceleration) < qAbs(speed.x())){
		computedScrollBarPosition.setX(doScroll(
			/* The differe, which shall be speed.x() is the essence. */
			0, speed.x(),
			/* FIXME need scrollarea's inner view width */
			scrollArea->width(),
			computedScrollBarPosition.x(),
			scrollArea->horizontalScrollBar()));

		/* now reduce the speed */
		if(0 < speed.x())
			speed.setX(speed.x() - gDecceleration);
		else
			speed.setX(speed.x() + gDecceleration);
	} else
		speed.setX(0);

	stopIfAtEnd();

//	DBG("[scroll] speed %.2f, %.2f", speed.x(), speed.y());
}
