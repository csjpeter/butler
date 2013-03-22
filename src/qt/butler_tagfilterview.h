/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAGFILTERVIEW_H
#define BUTLER_TAGFILTERVIEW_H

#include <QDialog>
#include <QsKineticScroller.h>

#include "butler_tagsmodel.h"

/*forwards*/
class QPushButton;
class QLabel;

class TagWidget;
class QScrollArea;

class TagFilterView : public QDialog
{
private:
	Q_OBJECT

public:
	TagFilterView(const QString & dbname, TagNameSet &tags, QWidget *parent = 0);
	virtual ~TagFilterView();

protected:

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);
	void resizeEvent(QResizeEvent *event);

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

	QScrollArea * tagScrollArea;
	TagWidget * tagsSelector;

	QPushButton * selectAllButton;
	QPushButton * selectNoneButton;
	QPushButton * okButton;

	QsKineticScroller scroll;
};

#endif
