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

#ifndef QSKINETICSCROLLER_H
#define QSKINETICSCROLLER_H

#include <QObject>
#include <QPoint>
#include <QTimer>
#include <QVector2D>
#include <QScrollBar>

class QAbstractScrollArea;
class QEvent;

//! Vertical kinetic scroller implementation without overshoot and bouncing.
//! A temporary solution to get kinetic-like scrolling on Symbian.
class QsKineticScroller: public QObject
{
Q_OBJECT
public:
	QsKineticScroller(QObject* parent = 0);
	~QsKineticScroller();

public slots:
	void disableKineticScroll();
	void enableKineticScrollFor(QAbstractScrollArea* scrollArea);

protected:
	bool eventFilter(QObject* object, QEvent* event);

private:
	double computeSpeed(double currPos, double lastPos, double lastSpeed);
	double doScroll( /* Returns the computed new scroll position. */
		double origPixPos, double currPixPos, double contentPixSize,
		double origScrollPos, QScrollBar * scroll);
	void stopIfAtEnd();

private slots:
	void onSpeedTimerElapsed(); /* Compute/Update the speed on speedTimer tick. */
	void onKineticTimerElapsed();

private:
	QAbstractScrollArea* scrollArea;
	bool scrolled;
	bool manualStop;

	QPoint pressedScrollBarPosition;
	QPoint pressedMousePosition;
	QPoint lastMousePos;
	QVector2D speed;
	QTimer speedTimer; /* should be active iff mouse button is pressed */
	QTimer kineticTimer;

	/* Copy of QScrollBar's value. This is redundancy but we need it to be real number. */
	QVector2D computedScrollBarPosition;

	QList<QEvent*> ignoreList;
};

#endif
