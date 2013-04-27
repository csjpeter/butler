/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <config.h>

#include <butler_widgets.h>

class PannView : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	PannView(QWidget * parent = 0) :
		QWidget(parent),
		scrollArea(0),
		scroller(&scrollArea)
	{
		setAttribute(Qt::WA_QuitOnClose, false);
		setFocusPolicy(Qt::NoFocus);
		scrollArea.horizontalScrollBar()->setObjectName("pannviewscrollbar");
		scrollArea.verticalScrollBar()->setObjectName("pannviewscrollbar");
		scrollArea.setFrameStyle(QFrame::NoFrame);
		scrollArea.setWidget(&main);
		scrollArea.setWidgetResizable(true);
		scrollArea.setFocusPolicy(Qt::NoFocus);
		main.setFocusPolicy(Qt::NoFocus);

		QVBoxLayout * vLayout = new QVBoxLayout;
		vLayout->addWidget(&toolBar);
		vLayout->addWidget(&scrollArea);
		vLayout->addWidget(&footerBar);
		vLayout->setContentsMargins(0,0,0,0);
		QWidget::setLayout(vLayout);

		installEventFilter(this);
		setSizeIncrement(0, 0);
	}
	virtual ~PannView() {}

	void setupView()
	{
		connect(&toolBar.backButton, SIGNAL(clicked()), this, SLOT(reject()));
		toolBar.relayout();
		footerBar.relayout();
	}

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

	virtual int width()
	{
		return scrollArea.viewport()->width();
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

	virtual void loadState(const QString & prefix)
	{
		QSettings settings;
		QPoint pos = settings.value(prefix + "/position", QPoint(-1, -1)).toPoint();
		QSize size = settings.value(prefix + "/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		else
			adjustSize();
		if(pos.x() != -1)
			move(pos);
	}

	virtual void saveState(const QString & prefix)
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

public:
	ToolBar toolBar;
	ControlBar footerBar;
private:
	QScrollArea scrollArea;
	QWidget main;

	KineticScroller scroller;
};

#endif
