/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWTAGVIEW_H
#define BUTLER_NEWTAGVIEW_H

#include <butler_pannview.h>

/*forwards*/
class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_tag.h>

#include "butler_databases.h"

class NewTagView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	NewTagView(const QString & dbname, QWidget * parent = 0);
	virtual ~NewTagView() {}

	void loadState();
	void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void mapToGui();
	void mapFromGui();

private slots:
	void doneClickedSlot(bool);

private:
	Tag tag;
	const QString & dbname;
	TagsModel & model;

	QLineEdit *nameEditor;
	QLineEdit *descEditor;

	QPushButton *doneButton;
};

#endif
