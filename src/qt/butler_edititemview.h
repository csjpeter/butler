/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITITEMVIEW_H
#define BUTLER_EDITITEMVIEW_H

#include <butler_tagwidget.h>
#include <butler_pannview.h>

class EditItemView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditItemView(const QString & dbname, ItemsModel & model, QWidget * parent = 0);
	virtual ~EditItemView();

	static EditItemView * newItemViewFactory(const QString & dbname);

	void setCursor(const QModelIndex& index);

	void loadState();
	void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void mapToGui();
	void mapFromGui();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);

private slots:
	void retranslate();
	void applyLayout(bool test = false);
	void relayout();
	void updateToolButtonStates();
	void prevClickedSlot();
	void nextClickedSlot();
	void saveSlot();
	void resetSlot();
	void quantityValueChangedSlot(double q);
	void unitPriceValueChangedSlot(double q);
	void grossPriceValueChangedSlot(double q);
	void wareNameEditFinishedSlot();
	void wareNameEditFinishedSlot(int);

public:
	const QString & dbname;
private:
	ItemsModel & model;
	ItemsModel * ownModel;
	QModelIndex cursor;
	Item item;
	Ware ware;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	ComboSelector wareEditor;
	ComboSelector categoryEditor;
	QuantityEditor quantityEditor;
	PriceEditor unitPriceEditor;
	PriceEditor grossPriceEditor;
	FormCheckBox onStockCheck;
	FormCheckBox boughtCheck;
	ComboSelector partnerEditor;
	DateTimeEditor purchaseDateTime;
	DateTimeEditor uploadDateTime;
	CommentEditor commentEditor;
	TagWidget tagsWidget;

	QWidget * lastNumEdited;
	QWidget * lastLastNumEdited;
	Text lastWareName;
};

#endif
