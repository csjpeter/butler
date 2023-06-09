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
	EditItemView(const csjp::String & dbname, ItemModel & model, QWidget * parent = 0);
	virtual ~EditItemView();

	static EditItemView * newItemViewFactory(const csjp::String & dbname);

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
	void prevSlot();
	void nextSlot();
	void saveSlotSpec();
	void saveSlot();
	void resetSlot();
	void quantityValueChangedSlot(double q);
	void unitPriceValueChangedSlot(double q);
	void grossPriceValueChangedSlot(double q);
	void wareNameEditFinishedSlot();
	void wareNameEditFinishedSlot(int);
	void accountNameEditFinishedSlot();
	void accountNameEditFinishedSlot(int);

public:
	const csjp::String dbname;
private:
	ItemModel & model;
	ItemModel * ownModel;
	QModelIndex cursor;
	Item item;
	Ware ware;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	ComboSelector wareEditor;
	ComboSelector typeEditor;
	ComboSelector brandEditor;
	QuantityEditor quantityEditor;
	PriceEditor unitPriceEditor;
	PriceEditor grossPriceEditor;
	ComboSelector accountEditor;
	ComboSelector partnerEditor;
	ComboSelector inventoryEditor;
	DateTimeEditor invChangeDateTime;
	DateTimeEditor uploadDateTime;
	CommentEditor commentEditor;
	TagWidget tagsWidget;

	QWidget * lastNumEdited;
	QWidget * lastLastNumEdited;
	Text lastWareName;
	Text lastAccountName;
};

#endif

