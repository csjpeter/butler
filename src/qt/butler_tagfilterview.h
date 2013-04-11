/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGFILTERVIEW_H
#define BUTLER_TAGFILTERVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include "butler_tagsmodel.h"

/*forwards*/
class QPushButton;
class QLabel;

class TagWidget;
class QScrollArea;

class TagFilterView : public PannView
{
private:
	Q_OBJECT

public:
	TagFilterView(const QString & dbname, TagNameSet &tags, QWidget * parent = 0);
	virtual ~TagFilterView();

protected:

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void resizeEvent(QResizeEvent *event);

	void loadState();
	void saveState();
	
	void mapToGui();
	void mapFromGui();

private slots:
	void okClickedSlot(bool);
	void selectAllClickedSlot(bool);
	void selectNoneClickedSlot(bool);

private:
	const QString & dbname;
	TagNameSet &tags;

	TagWidget * tagsSelector;

	QPushButton * selectAllButton;
	QPushButton * selectNoneButton;
	QPushButton * okButton;
};

#endif
