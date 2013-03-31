/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_ACCOUNTINGVIEW_H
#define BUTLER_ACCOUNTINGVIEW_H

#include <butler_item.h>
#include <butler_widgets.h>

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

	virtual void resizeEvent(QResizeEvent * event);

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

	DoneButton doneButton;
	ShopSelector shopSelector;
	PurchaseDateTimeEditor purchaseDateTime;
	WareEditor wareEditor;
	CategoryEditor categoryEditor;
	QuantityEditor quantityEditor;
	UnitPriceEditor unitPriceEditor;
	GrossPriceEditor grossPriceEditor;
	OnStockCheckBox onStockCheck;
	UploadDateTimeEditor uploadDateTime;
	BoughtCheckBox boughtCheck;
	CommentEditor commentEditor;
};

#endif
