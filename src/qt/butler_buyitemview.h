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
class PartnersModel;

class BuyItemView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	BuyItemView(const QString & dbname, QWidget *parent = 0);
	virtual ~BuyItemView();

	void setItem(unsigned itemRow, unsigned partnerRow);

	void loadState();
	void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	
	void mapToGui();
	void mapFromGui();

private slots:
	void buy();
	void quantityValueChangedSlot(double q);
	void unitPriceEditingFinishedSlot();
	void grossPriceValueChangedSlot(double g);

public:
	const QString dbname;
private:
	ShoppingModel & model;
	unsigned itemCursor;
	unsigned partnerCursor;
	Item item;

	NumPad *numpad;

	QLabel *nameLabel;
	QDoubleSpinBox *quantityEditor;
	QDoubleSpinBox *unitPriceEditor;
	QDoubleSpinBox *grossPriceEditor;
	QDateTimeEdit *invChangeDateTime;
	QComboBox *partnerBox;

	QPushButton *buyButton;

};

#endif
