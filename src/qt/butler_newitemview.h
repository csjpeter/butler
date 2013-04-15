/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWITEMVIEW_H
#define BUTLER_NEWITEMVIEW_H

#include <butler_pannview.h>

/*forwards*/
class QLineEdit;
class QDoubleSpinBox;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QStandardItemModel;
class QListView;
class QDataWidgetMapper;
class QModelIndex;
class QComboBox;

#include <butler_item.h>

#include "butler_databases.h"

class NewItemView : public PannView
{
private:
	Q_OBJECT

public:
	NewItemView(const QString & dbname, QWidget *parent = 0);
	virtual ~NewItemView() {}

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

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

	QLineEdit *nameEditor;
	QComboBox *nameBox;
	QLineEdit *categoryEditor;
	QComboBox *categoryBox;
	QDoubleSpinBox *quantityEditor;
	QTextEdit *commentEditor;

	QPushButton *doneButton;
};

#endif
