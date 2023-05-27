/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_EDITPAYMENTVIEW_H
#define BUTLER_EDITPAYMENTVIEW_H

#include <butler_tagwidget.h>
#include <butler_pannview.h>

class EditPaymentView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditPaymentView(const csjp::String & dbname, PaymentModel & model, QWidget * parent = 0);
	virtual ~EditPaymentView();

	static EditPaymentView * newPaymentViewFactory(const csjp::String & dbname);

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
	void accountNameEditFinishedSlot();
	void accountNameEditFinishedSlot(int);

public:
	const csjp::String dbname;
private:
	PaymentModel & model;
	PaymentModel * ownModel;
	QModelIndex cursor;
	Payment payment;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	PriceEditor amountEditor;
	ComboSelector accountEditor;
	ComboSelector partnerEditor;
	DateTimeEditor payDateTime;
	DateTimeEditor uploadDateTime;
	CommentEditor commentEditor;

	Text lastAccountName;
};

#endif

