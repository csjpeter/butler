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

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);

private slots:
	void retranslate();
	void applyLayout();
	void relayout();
	void saveSlot();
	void quantityValueChangedSlot(double q);
	void unitPriceValueChangedSlot(double q);
	void grossPriceValueChangedSlot(double q);
	void mandatoryFieldChangedSlot(const QString &);
	void wareNameEditFinishedSlot();
	void wareNameEditFinishedSlot(int);
	void quantityEditFinishedSlot();

private:
	const QString & dbname;
	ItemsModel & model;
	Item item;

	Button backButton;
	Button doneButton;
	InfoLabel infoLabel;
	ComboSelector wareEditor;
	ComboSelector categoryEditor;
	QuantityEditor quantityEditor;
	PriceEditor unitPriceEditor;
	PriceEditor grossPriceEditor;
	FormCheckBox onStockCheck;
	FormCheckBox boughtCheck;
	ComboSelector shopEditor;
	DateTimeEditor purchaseDateTime;
	DateTimeEditor uploadDateTime;
	CommentEditor commentEditor;

	QWidget * lastNumEdited;
	QWidget * lastLastNumEdited;
	QString lastWareName;
};

#endif
