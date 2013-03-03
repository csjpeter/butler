/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_BUYITEMVIEW_H
#define BUTLER_BUYITEMVIEW_H

#include <QtGui>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include "butler_shoppingmodel.h"

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
class QStandardItemModel;
class QListView;
class QComboBox;
class QDoubleSpinBox;

namespace Butler {

class SwitchButton;
class FlowLayout;
class TagSet;
class NumPad;
class ItemsModel;
class ShopsModel;

class BuyItemView : public QDialog
{
private:
	Q_OBJECT

public:
	BuyItemView(QWidget *parent, ShoppingModel &);
	virtual ~BuyItemView();

	void setItem(unsigned itemRow, unsigned shopRow);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();
	
	void mapToGui();
	void mapFromGui();

private slots:
	void buy();
	void quantityValueChangedSlot(double q);
	void unitPriceEditingFinishedSlot();
	void grossPriceValueChangedSlot(double g);

private:
	ShoppingModel &shoppingModel;
	unsigned itemCursor;
	unsigned shopCursor;
	Item item;

	NumPad *numpad;

	QLabel *nameLabel;
	QDoubleSpinBox *quantityEditor;
	QLabel *unitLabel;
	QDoubleSpinBox *unitPriceEditor;
	QDoubleSpinBox *grossPriceEditor;
	QDateTimeEdit *purchaseDateTime;
	QComboBox *shopBox;

	QPushButton *buyButton;

};

}

#endif

