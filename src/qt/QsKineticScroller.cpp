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

#include <csjp_exception.h>
#include <csjp_logger.h>

#include "QsKineticScroller.h"
#include <QApplication>
#include <QScrollBar>
#include <QAbstractButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QAbstractScrollArea>
#include <QMouseEvent>
#include <QEvent>
#include <QTimer>
#include <QList>
#include <QtDebug>
#include <cstddef> // for NULL

// http://blog.codeimproved.net/2010/12/kinetic-scrolling-with-qt-the-what-and-the-how/

// A number of mouse moves are ignored after a press to differentiate
// it from a press & drag. FIXME drag should be started with long tap
static const int gMaxIgnoredMouseMoves = 2;

static const int gTimerInterval = 40; // milisec periodic time to check move speed && set scroll pos
static const double gSpeedReducingFactor = 10;
static const double gMaxSpeed = 20; // speed = (newPos - lastPos ) / gSpeedReducingFactor
static const double gDecceleration = 0.05;

class QsKineticScrollerImpl
{
public:
	QsKineticScrollerImpl() :
		scrollArea(NULL),
		isPressed(false),
		isMoving(false),
		lastMouseYPos(0),
		lastScrollBarPosition(0),
		speed(0),
		ignoredMouseMoves(0)
	{}

	void stopMotion()
	{
		isMoving = false;
		speed = 0;
		kineticTimer.stop();
	}

	QAbstractScrollArea* scrollArea;
	bool isPressed;
	bool isMoving;
	QPoint lastPressPoint;
	int lastMouseYPos;
	int lastScrollBarPosition;
	double speed;
	int ignoredMouseMoves;
	QTimer kineticTimer;

	QList<QEvent*> ignoreList;
};

QsKineticScroller::QsKineticScroller(QObject *parent) :
	QObject(parent),
	d(new QsKineticScrollerImpl)
{
	connect(&d->kineticTimer, SIGNAL(timeout()), SLOT(onKineticTimerElapsed()));
}

// needed by smart pointer
QsKineticScroller::~QsKineticScroller()
{
	if(d->scrollArea)
		disableKineticScrollFor(d->scrollArea);
}

void QsKineticScroller::disableKineticScrollFor(QAbstractScrollArea* scrollArea)
{
	ENSURE(scrollArea != NULL, csjp::LogicError);

	// remove existing association
	QList<QAbstractButton*> buttons = d->scrollArea->findChildren<QAbstractButton*>();
	Q_FOREACH(QAbstractButton *button, buttons)
		button->removeEventFilter(this);

	QList<QLineEdit*> edits = d->scrollArea->findChildren<QLineEdit*>();
	Q_FOREACH(QLineEdit *edit, edits)
		edit->removeEventFilter(this);

	QList<QTextEdit*> textEdits = d->scrollArea->findChildren<QTextEdit*>();
	Q_FOREACH(QTextEdit *textEdit, textEdits)
		textEdit->viewport()->removeEventFilter(this);

	QList<QComboBox*> combos = d->scrollArea->findChildren<QComboBox*>();
	Q_FOREACH(QComboBox *combo, combos)
		combo->removeEventFilter(this);

	d->scrollArea->viewport()->removeEventFilter(this);
	d->scrollArea->removeEventFilter(this);
	d->scrollArea = NULL;
}

void QsKineticScroller::enableKineticScrollFor(QAbstractScrollArea* scrollArea)
{
	ENSURE(scrollArea != NULL, csjp::LogicError);

	if(d->scrollArea)
		disableKineticScrollFor(scrollArea);

	// associate
	scrollArea->installEventFilter(this);
	scrollArea->viewport()->installEventFilter(this);
	d->scrollArea = scrollArea;

	QList<QAbstractButton*> buttons = scrollArea->findChildren<QAbstractButton*>();
	Q_FOREACH(QAbstractButton *button, buttons)
		button->installEventFilter(this);

	QList<QLineEdit*> edits = scrollArea->findChildren<QLineEdit*>();
	Q_FOREACH(QLineEdit *edit, edits)
		edit->installEventFilter(this);

	QList<QTextEdit*> textEdits = scrollArea->findChildren<QTextEdit*>();
	Q_FOREACH(QTextEdit *textEdit, textEdits)
		textEdit->viewport()->installEventFilter(this);

	QList<QComboBox*> combos = scrollArea->viewport()->findChildren<QComboBox*>();
	Q_FOREACH(QComboBox *combo, combos)
		combo->installEventFilter(this);
}

//! intercepts mouse events to make the scrolling work
bool QsKineticScroller::eventFilter(QObject* obj, QEvent* event)
{
	if(0 < d->ignoreList.removeAll(event))
		return false;

	if(!d->scrollArea)
		return false;

	const QEvent::Type eventType = event->type();
	if(		eventType != QEvent::MouseButtonPress &&
			eventType != QEvent::MouseButtonRelease &&
			eventType != QEvent::MouseMove)
		return false;

	QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
	switch(eventType){
		case QEvent::MouseButtonPress:
			DBG("[scroll] press pos %d, %d",
					mouseEvent->globalPos().x(),
					mouseEvent->globalPos().y());
			d->isPressed = true;
			d->lastPressPoint = mouseEvent->globalPos();
			d->lastScrollBarPosition = d->scrollArea->verticalScrollBar()->value();
			if(d->isMoving) // press while kinetic scrolling, so stop
				d->stopMotion();
			break;
		case QEvent::MouseMove:
			if(!d->isPressed)
				return false; /* We have nothing to do with move without press. */

			DBG("[scroll] move pos %d, %d",
					mouseEvent->globalPos().x(),
					mouseEvent->globalPos().y());
			if(!d->isMoving){
				// A few move events are ignored as "click jitter", but after
				// that we assume that the user is doing a click & drag
				if( d->ignoredMouseMoves < gMaxIgnoredMouseMoves )
					++d->ignoredMouseMoves;
				else {
					d->ignoredMouseMoves = 0;
					d->isMoving = true;
					d->lastMouseYPos = mouseEvent->globalPos().y();
					if( !d->kineticTimer.isActive() )
						d->kineticTimer.start(gTimerInterval);
				}
			} else {
				// manual scroll
				const int dragDistance =
					mouseEvent->globalPos().y() - d->lastPressPoint.y();
				const int newScrollPos = d->lastScrollBarPosition - dragDistance;
				d->scrollArea->verticalScrollBar()->setValue(newScrollPos);
				DBG("[scroll] manual scroll to %u", newScrollPos);
			}
			break;
		case QEvent::MouseButtonRelease:
			DBG("[scroll] release pos %d, %d",
					mouseEvent->pos().x(), mouseEvent->pos().y());
			d->isPressed = false;
			d->ignoredMouseMoves = 0;
			if(d->isMoving)
				break;

			// Looks like the user wanted a single click. Simulate the click,
			// as the events were already consumed
			if(		obj != d->scrollArea->viewport() &&
					!qobject_cast<QComboBox*>(obj) &&
					!qobject_cast<QLineEdit*>(obj) &&
					!qobject_cast<QAbstractButton*>(obj) &&
					!qobject_cast<QTextEdit*>(obj->parent()) )
				break;

			{
				QMouseEvent* mousePress = new QMouseEvent(
						QEvent::MouseButtonPress, mouseEvent->pos(),
						Qt::LeftButton,	Qt::LeftButton, Qt::NoModifier);
				QMouseEvent* mouseRelease = new QMouseEvent(
						QEvent::MouseButtonRelease, mouseEvent->pos(),
						Qt::LeftButton, Qt::LeftButton, Qt::NoModifier);

				// Ignore these two
				d->ignoreList << mousePress;
				d->ignoreList << mouseRelease;

				QApplication::postEvent(obj, mousePress);
				QApplication::postEvent(obj, mouseRelease);
			}

			if(		!qobject_cast<QLineEdit*>(obj) &&
					!qobject_cast<QTextEdit*>(obj->parent()))
				break;

			DBG("[scroll] trying to open keyboard");
			QApplication::postEvent(obj, new QEvent(QEvent::RequestSoftwareInputPanel));

			break;
		default:
			break;
	}

	return true;
}

void QsKineticScroller::onKineticTimerElapsed()
{
	if(!d->scrollArea)
		return;

	if(d->isPressed && d->isMoving){
		// the speed is measured between two timer ticks
		const int cursorYPos = QCursor::pos().y();

		DBG("[scroll] cursor Y pos %u; last is %u", cursorYPos, d->lastMouseYPos);

		d->speed = (cursorYPos - d->lastMouseYPos) / gSpeedReducingFactor;
		d->speed = qBound(-gMaxSpeed, d->speed, gMaxSpeed);
		d->lastMouseYPos = cursorYPos;
	} else if(!d->isPressed && d->isMoving){
		// use the previously recorded speed and gradually decelerate
		if(qAbs(d->speed) < qAbs(gDecceleration)) {
			d->speed = 0;
			d->stopMotion();
		} else {
			if(0 < d->speed)
				d->speed -= gDecceleration;
			else
				d->speed += gDecceleration;
		}

		const int newScrollPos = d->scrollArea->verticalScrollBar()->value() - d->speed;
		d->scrollArea->verticalScrollBar()->setValue(newScrollPos);

		DBG("[scroll] kinetic scroll to %u with speed %2.2f", newScrollPos, d->speed);
	} else
		d->stopMotion();
}
