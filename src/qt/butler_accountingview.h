/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ACCOUNTINGVIEW_H
#define BUTLER_ACCOUNTINGVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_item.h>

#include "butler_itemsmodel.h"
#include "butler_tagsmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QDoubleSpinBox;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QScrollArea;
class QStandardItemModel;
class QListView;
class QStandardItemModel;
class QComboBox;

class TagWidget;

class AccountingView : public QDialog
{
private:
	Q_OBJECT

public:
	AccountingView(QWidget *parent, ItemsModel &);

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
	ItemsModel &model;
	Item item;

	QDateTimeEdit *uploadDateTime;
	QLineEdit *nameEditor;
	QComboBox *nameBox;
	QLineEdit *categoryEditor;
	QComboBox *categoryBox;
	QDoubleSpinBox *quantityEditor;
	QLabel *unitLabel;
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
