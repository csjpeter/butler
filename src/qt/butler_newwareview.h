/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWWAREVIEW_H
#define BUTLER_NEWWAREVIEW_H

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

#include <butler_ware.h>

#include "butler_databases.h"

class TagWidget;

class NewWareView : public PannView
{
private:
	Q_OBJECT

public:
	NewWareView(const QString & dbname, QWidget * parent = 0);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void doneClickedSlot(bool);

private:
	Ware ware;
	const QString & dbname;
	WaresModel & model;

	QLineEdit *nameEditor;
	QLineEdit *unitEditor;
	QLineEdit *categoriesEditor;
	TagWidget *tagsSelector;

	QPushButton *doneButton;
};

#endif
