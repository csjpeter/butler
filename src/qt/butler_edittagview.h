/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITTAGVIEW_H
#define BUTLER_EDITTAGVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_tag.h>

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

class EditTagView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditTagView(const QString & dbname, QWidget * parent = 0);
	virtual ~EditTagView() {}

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

private:
	Tag tag;
	const QString & dbname;
	TagsModel & model;
	QModelIndex cursor;

	QLineEdit *nameEditor;
	QLineEdit *descEditor;

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
};

#endif
