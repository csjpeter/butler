/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_BUYITEMVIEW_H
#define BUTLER_BUYITEMVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include "butler_databases.h"

class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;
class QComboBox;
class QDoubleSpinBox;

class SwitchButton;
class FlowLayout;
class TagSet;
class NumPad;
class ItemsModel;
class ShopsModel;

class BuyItemView : public PannView
{
private:
	Q_OBJECT

public:
	BuyItemView(const QString & dbname, QWidget *parent = 0);
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
	const QString & dbname;
	ShoppingModel & model;
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

#endif
