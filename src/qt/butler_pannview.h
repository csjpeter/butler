/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <QWidget>
#include <butler_kineticscroller.h>

#include <config.h>

class PannView : public QWidget
{
private:
	Q_OBJECT

public:
	PannView(QWidget * parent = 0) :
		QWidget(parent),
		scrollArea(0),
		scroller(&scrollArea)
	{
		setFocusPolicy(Qt::NoFocus);
		scrollArea.setObjectName("pannviewscrollarea");
		scrollArea.setFrameStyle(QFrame::NoFrame);
		scrollArea.setWidget(&main);
		scrollArea.setWidgetResizable(true);
		scrollArea.setFocusPolicy(Qt::NoFocus);
		main.setFocusPolicy(Qt::NoFocus);

		QVBoxLayout * vLayout = new QVBoxLayout;
		vLayout->addWidget(&scrollArea);
		vLayout->setContentsMargins(0,0,0,0);
		QWidget::setLayout(vLayout);

		installEventFilter(this);
	}
	virtual ~PannView() {}

	virtual QLayout * layout() const
	{
		return main.layout();
	}

	virtual void setLayout(QLayout * layout)
	{
		delete main.layout();
		main.setLayout(layout);
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

	bool eventFilter(QObject *obj, QEvent *event)
	{
		if (event->type() != QEvent::KeyPress)
			return QObject::eventFilter(obj, event);

		QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
		DBG("Received key press 0x%x", keyEvent->key());

		switch (keyEvent->key()){
/*			case Qt::Key_Enter:
			case Qt::Key_Return:
				break;*/
			case Qt::Key_Escape:
				{
					QWidget * focused = focusWidget();
					if(focused)
						focusWidget()->clearFocus();
					else
						reject();
				}
				return true;
			default:
				break;
		}
		return QObject::eventFilter(obj, event);
	}

public slots:
	void activate()
	{
		QWidget::show(); raise(); activateWindow();
//		setWindowState( (windowState() & ~Qt::WindowMinimized) | Qt::WindowActive);
	}
	void accept() { emit this->accepted(); emit this->finished(1); close(); }
	void reject() { emit this->rejected(); emit this->finished(0); close(); }
	void done(int result) { emit this->finished(result); close(); }

signals:
	void accepted();
	void finished(int);
	void rejected();

private:
	QScrollArea scrollArea;
	QWidget main;

	KineticScroller scroller;
};

#endif
