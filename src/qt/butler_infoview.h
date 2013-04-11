/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_INFOVIEW_H
#define BUTLER_INFOVIEW_H

#include "butler_pannview.h"

/*forwards*/
class QLabel;

class InfoView : public PannView
{
private:
	Q_OBJECT

public:
	InfoView(QWidget *parent = 0);
	~InfoView();

private:
	QLabel * label;
};

#endif
