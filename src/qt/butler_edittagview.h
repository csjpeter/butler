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
class QAction;
class QToolButton;
class QLineEdit;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QScrollArea;
class QStandardItemModel;
class QListView;

class EditTagView : public PannView
{
private:
	Q_OBJECT

public:
	EditTagView(const QString & dbname, QWidget * parent = 0);

	void setCursor(const QModelIndex& index);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

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
