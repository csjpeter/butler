/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWITEMVIEW_H
#define BUTLER_NEWITEMVIEW_H

#include <butler_pannview.h>

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QDoubleSpinBox;
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
class QComboBox;

#include <butler_item.h>

#include "butler_databases.h"

class TagWidget;

class NewItemView : public PannView
{
private:
	Q_OBJECT

public:
	NewItemView(const QString & dbname, QWidget *parent = NULL);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void doneClickedSlot(bool);
	void nameEditFinishedSlot();
	void nameCurrentIndexChangedSlot(int index);

private:
	Item item;
	const QString & dbname;
	ShoppingModel & model;

	TagWidget *tagwidget;

	QLineEdit *nameEditor;
	QComboBox *nameBox;
	QLineEdit *categoryEditor;
	QComboBox *categoryBox;
	QDoubleSpinBox *quantityEditor;
	QLabel *unitLabel;
	QTextEdit *commentEditor;

	QPushButton *doneButton;
};

#endif
