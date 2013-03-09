/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITWAREVIEW_H
#define BUTLER_EDITWAREVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_ware.h>

#include <butler_databases.h>

#include "butler_tagsmodel.h"
#include "butler_waresmodel.h"

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
class QTableView;
class QScrollArea;
class QListView;

class TagWidget;

class EditWareView : public QDialog
{
private:
	Q_OBJECT

public:
	EditWareView(QWidget *parent, WaresModel &);

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
	void closeSlot();

private:
	Ware ware;
	WaresModel &model;
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
