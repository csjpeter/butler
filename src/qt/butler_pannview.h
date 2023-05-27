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
	PannView(QWidget * parent = 0);
	virtual ~PannView();
	void setupView();
	virtual QLayout * layout() const;
	virtual void setLayout(QLayout * layout);
	virtual QSize sizeHint();
	virtual int width();
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	bool eventFilter(QObject *obj, QEvent *event);
	virtual void loadState(const QString & prefix);
	virtual void saveState(const QString & prefix);
	virtual void setWindowTitle(const QString & str);

private slots:
	void verticalScrollBarRangeChanged();

public slots:
	void activate();
	void accept();
	void reject();
	void done(int result);

signals:
	void accepted();
	void finished(int);
	void rejected();

public:
	Label titleLabel;
	ToolBar toolBar;
	ControlBar footerBar;
private:
	QScrollArea scrollArea;
	QWidget main;

	KineticScroller scroller;
};

#endif

