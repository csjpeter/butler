/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITWAREVIEW_H
#define BUTLER_EDITWAREVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_ware.h>

#include "butler_databases.h"

/*forwards*/
class QLineEdit;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;

class TagWidget;

class EditWareView : public PannView
{
private:
	Q_OBJECT

public:
	EditWareView(const QString & dbname, QWidget * parent = 0);
	virtual ~EditWareView() {}

	void setCursor(const QModelIndex& index);

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void saveSlot();
	void prevClickedSlot();
	void nextClickedSlot();
	void closeSlot();

private:
	Ware ware;
	const QString & dbname;
	WaresModel & model;
	QModelIndex cursor;

	QLineEdit *nameEditor;
	QLineEdit *unitEditor;
	QLineEdit *categoriesEditor;
	TagWidget *tagsSelector;

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
	QPushButton *closeButton;
};

#endif
