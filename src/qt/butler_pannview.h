/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <QWidget>
#include <butler_macros.h>
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
		quitShortcut(QKeySequence(QKeySequence::Quit), this),
		scroller(&scrollArea)
	{
		setFocusPolicy(Qt::NoFocus);
		scrollArea.horizontalScrollBar()->setObjectName("pannviewscrollbar");
		scrollArea.verticalScrollBar()->setObjectName("pannviewscrollbar");
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
		setSizeIncrement(5, 1);

		connect(&quitShortcut, SIGNAL(activated()), qApp, SLOT(quit()));
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

	virtual void loadState(QString prefix)
	{
		QSettings settings;
		QPoint pos = settings.value(prefix + "/position", QPoint()).toPoint();
		QSize size = settings.value(prefix + "/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		else
			adjustSize();
		move(pos);
	}

	virtual void saveState(QString prefix)
	{
		QSettings settings;
		settings.setValue(prefix + "/position", pos());
		settings.setValue(prefix + "/size", size());
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
	QShortcut quitShortcut;

	KineticScroller scroller;
};

#endif
