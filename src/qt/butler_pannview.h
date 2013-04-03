/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <QWidget>
#include <QsKineticScroller.h>

#include <config.h>

class PannView : public QWidget
{
private:
	Q_OBJECT

public:
	PannView(QWidget * parent = 0) :
		QWidget(parent),
		scrollArea(0),
		scroll(0)
	{
		scrollArea.setFrameStyle(QFrame::NoFrame);
		scrollArea.setWidget(&main);
		scrollArea.setWidgetResizable(true);

		QVBoxLayout * vLayout = new QVBoxLayout;
		vLayout->addWidget(&scrollArea);
		vLayout->setContentsMargins(0,0,0,0);
		QWidget::setLayout(vLayout);
	}
	virtual ~PannView() {}

	virtual QLayout * layout() const
	{
		return main.layout();
	}

	virtual void setLayout(QLayout * layout)
	{
		delete main.layout();
		scroll.disableKineticScroll();
		main.setLayout(layout);
		scroll.enableKineticScrollFor(&scrollArea);
	}

	virtual QSize sizeHint()
	{
		return main.sizeHint();
	}

	virtual void showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
	}

	virtual void closeEvent(QCloseEvent *event)
	{
		QWidget::closeEvent(event);
	}

public slots:
	void activate()
	{
		QWidget::show(); raise(); activateWindow();
//		setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	}
	void accept() { emit this->accepted(); emit this->finished(1); }
	void reject() { emit this->rejected(); emit this->finished(0); }
	void done(int result) { emit this->finished(result); }

signals:
	void accepted();
	void finished(int);
	void rejected();

private:
	QScrollArea scrollArea;
	QWidget main;

	QsKineticScroller scroll;
};

#endif
