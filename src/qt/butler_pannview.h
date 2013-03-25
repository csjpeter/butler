/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_PANNVIEW_H
#define BUTLER_PANNVIEW_H

#include <QWidget>
#include <QsKineticScroller.h>

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

private:
	QScrollArea * scrollArea;

	QsKineticScroller scroll;
};

#endif
