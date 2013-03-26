/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <QWidget>
#include <QsKineticScroller.h>

#include <csjp_logger.h>

/*forwards*/
class QScrollArea;

class PannView : public QWidget
{
private:
	Q_OBJECT

public:
	PannView(QWidget *parent = 0);
	~PannView();

	void setLayout(QLayout * layout);

	virtual void showEvent(QShowEvent *event)
	{
		QWidget::showEvent(event);
	}

	virtual void closeEvent(QCloseEvent *event)
	{
		QWidget::closeEvent(event);
	}

	virtual void resizeEvent(QResizeEvent *event)
	{
		QWidget::resizeEvent(event);
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
	QScrollArea * scrollArea;

	QsKineticScroller scroll;
};

#endif
