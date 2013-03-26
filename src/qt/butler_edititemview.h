/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITITEMVIEW_H
#define BUTLER_EDITITEMVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_item.h>

#include "butler_databases.h"

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
class QScrollArea;
class QStandardItemModel;
class QListView;
class QStandardItemModel;
class QComboBox;

class TagWidget;

class EditItemView : public PannView
{
private:
	Q_OBJECT

public:
	EditItemView(const QString & dbname, ItemsModel & model, QWidget * parent = 0);

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
	void nameEditFinishedSlot();
	void quantityValueChangedSlot(double q);
	void unitPriceEditingFinishedSlot();
	void grossPriceValueChangedSlot(double g);

private:
	const QString & dbname;
	ItemsModel & model;
	QModelIndex cursor;
	Item updatedItem;

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

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
};

#endif
