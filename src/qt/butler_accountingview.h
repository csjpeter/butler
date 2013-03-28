/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ACCOUNTINGVIEW_H
#define BUTLER_ACCOUNTINGVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QLineEdit>

#include <butler_item.h>

#include "butler_databases.h"

/*forwards*/
class QLineEdit;
class QDoubleSpinBox;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;
class QComboBox;

class MySpinBox : public QDoubleSpinBox
{
private:
	Q_OBJECT
public:
	MySpinBox(QWidget * parent = 0) :
		QDoubleSpinBox(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
	}
};

class MyDateTimeEdit : public QDateTimeEdit
{
private:
	Q_OBJECT
public:
	MyDateTimeEdit(QWidget * parent = 0) :
		QDateTimeEdit(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
	}
};

class AccountingView : public PannView
{
private:
	Q_OBJECT

public:
	AccountingView(const QString & dbname, ItemsModel & model, QWidget * parent = 0);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void saveSlot();
	void nameEditFinishedSlot();
	void quantityValueChangedSlot(double q);
	void unitPriceEditingFinishedSlot();
	void grossPriceValueChangedSlot(double g);

private:
	const QString & dbname;
	ItemsModel & model;
	Item item;

	QDateTimeEdit *uploadDateTime;
	QLineEdit *nameEditor;
	QComboBox *nameBox;
	QLineEdit *categoryEditor;
	QComboBox *categoryBox;
	QDoubleSpinBox *quantityEditor;
	QTextEdit *commentEditor;

	QCheckBox *boughtCheck;
	QDoubleSpinBox *unitPriceEditor;
	QDoubleSpinBox *grossPriceEditor;
	QDateTimeEdit *purchaseDateTime;
	QComboBox *shopBox;
	QCheckBox *onStockCheck;

	QPushButton *saveButton;
};

#endif
