/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWWAREVIEW_H
#define BUTLER_NEWWAREVIEW_H

#include <QDialog>

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
class QTableView;
class QScrollArea;
class QListView;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_ware.h>

#include "butler_waresmodel.h"

namespace Butler {

class TagWidget;

class NewWareView : public QDialog
{
private:
	Q_OBJECT

public:
	NewWareView(QWidget *parent, WaresModel &);

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
	WaresModel &model;

	QLineEdit *nameEditor;
	QLineEdit *unitEditor;
	QLineEdit *categoriesEditor;
	TagWidget *tagsSelector;

	QPushButton *doneButton;
};

}

#endif

