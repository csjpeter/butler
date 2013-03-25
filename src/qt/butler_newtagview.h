/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWTAGVIEW_H
#define BUTLER_NEWTAGVIEW_H

#include <butler_pannview.h>

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QScrollArea;
class QStandardItemModel;
class QListView;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_tag.h>

#include "butler_databases.h"

class TagWidget;

class NewTagView : public PannView
{
private:
	Q_OBJECT

public:
	NewTagView(const QString & dbname, QWidget *parent);

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
	Tag tag;
	const QString & dbname;
	TagsModel & model;

	QLineEdit *nameEditor;
	QLineEdit *descEditor;

	QPushButton *doneButton;
};

#endif
