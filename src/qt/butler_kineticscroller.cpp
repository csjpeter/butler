/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#define DEBUG

#include <math.h>

#include <butler_config.h>
#include "butler_kineticscroller.h"

csjp::OwnerContainer<ObjectScrollerPair> kineticScrollers;

static const int gTimerInterval = 40; // milisec periodic time to check move speed && set scroll pos
static const double gDecceleration = 0.05;

KineticScroller::KineticScroller(QAbstractScrollArea * scrollArea, QObject *parent) :
	QObject(parent),
	scrollArea(scrollArea),
	scrolled(0),
	manualStop(false),
	alreadyFocused(false)
{
	kineticScrollers.add(new ObjectScrollerPair(scrollArea, this));
	connect(&speedTimer, SIGNAL(timeout()), SLOT(onSpeedTimerElapsed()));
	connect(&kineticTimer, SIGNAL(timeout()), SLOT(onKineticTimerElapsed()));
}

KineticScroller::~KineticScroller()
{
	kineticScrollers.remove<QObject*>(scrollArea);
}

double KineticScroller::computeSpeed(double currPos, double lastPos, double lastSpeed)
{
	double newSpeed = currPos - lastPos;

	if(newSpeed * lastSpeed < 0) /* Change of direction restarts the motion. */
		lastSpeed = 0; 

	/* For longer motions we want balanced speed computation. */
	if(qAbs(gDecceleration) < qAbs(lastSpeed))
		newSpeed = (newSpeed + lastSpeed) / 2;

	return newSpeed;
}

void KineticScroller::onSpeedTimerElapsed()
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
	
/*	DBG("[scroll] speed %.2f, %.2f", speed.x(), speed.y());*/

	lastMousePos = cursorPos;

	if(speed.isNull())
		speedTimer.stop();
}

double KineticScroller::doScroll(
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

void KineticScroller::stopIfAtEnd()
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

bool KineticScroller::eventHandler(QObject * receiver, QEvent * event)
{
	ENSURE(scrollArea != 0, csjp::LogicError);

	QScrollBar * sbar = qobject_cast<QScrollBar*>(receiver);
	if(sbar)
		return false;

	const QEvent::Type eventType = event->type();
	if(		eventType != QEvent::MouseButtonPress &&
			eventType != QEvent::MouseButtonRelease &&
			eventType != QEvent::MouseMove)
		return false;

	QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
	QWidget * wgt = qobject_cast<QWidget*>(receiver);

	switch(eventType){
		case QEvent::MouseButtonPress:
			DBG("Press");
			if(wgt && wgt->hasFocus()){
				alreadyFocused = true;
				DBG("Already focused");
				return false;
			} else {
				DBG("Not yet focused");
				alreadyFocused = false;
			}
			manualStop = !speed.isNull();
			speed.setX(0);
			speed.setY(0);
			pressedScrollBarPosition.setX(scrollArea->horizontalScrollBar()->value());
			pressedScrollBarPosition.setY(scrollArea->verticalScrollBar()->value());
			lastMousePos = pressedMousePosition = mouseEvent->globalPos();
			scrolled = false;
			speedTimer.start(gTimerInterval);
//			DBG("return %s", manualStop ? "true" : "false" );
			return manualStop;
		case QEvent::MouseMove:
			DBG("Move");
			if(alreadyFocused){
				DBG("Already focused");
				return false;
			}

			if(!speedTimer.isActive()){
				if(mouseEvent->buttons() == Qt::NoButton){
//					DBG("speedTimer is not active nor button is pressed");
					return false; /* We have nothing to do without press. */
				}
//				DBG("restarting speedTimer");
				speedTimer.start(gTimerInterval);
			}

			/* If for some reason we missed the release event, on next pressless move
			 * lets not do any scrolling. */
			if(mouseEvent->buttons() == Qt::NoButton){
//				DBG("No button is pressed down");
				speedTimer.stop();
				return false;
			}

			if(sqrt(pow(pressedMousePosition.x() - mouseEvent->globalPos().x(), 2) +
				pow(pressedMousePosition.y() - mouseEvent->globalPos().y(), 2)) <
					Config::thresholdScrollDistance){
//				DBG("kinetic threshold not reached");
				return false;
			}

			if(!scrolled){
//				DBG("Not yet scrolled, so focus out");
				QApplication::postEvent(receiver,
						new QFocusEvent(QEvent::FocusOut));
				if(wgt){
					QApplication::postEvent(wgt,
						new QFocusEvent(QEvent::FocusOut));
					while(wgt->parentWidget())
						wgt = wgt->parentWidget();
					if(wgt->focusWidget())
						wgt->focusWidget()->clearFocus();
				}
				/* FIXME notice drag and drop and abort kinetic scrolling */
			}

//			DBG("scrolled");
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
			DBG("Release");
			if(alreadyFocused){
				DBG("Already focused");
				return false;
			}

			speedTimer.stop();

			if(!scrolled){
//				DBG("not scrolled, manualStop: %d", manualStop);
				speed.setX(0);
				speed.setY(0);
				return manualStop;
			}

//			DBG("start kinetic timer");
			kineticTimer.start(gTimerInterval);
			return true;
		default:
			DBG("DEFAULT EVENT");
			return false;
	}
}

void KineticScroller::onKineticTimerElapsed()
{
	if(!scrollArea)
		return;
	if(speed.isNull()){
		DBG("stop kinetic timer");
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

/*	DBG("[scroll] speed %.2f, %.2f", speed.x(), speed.y());*/
}
