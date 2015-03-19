/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2014 Csaszar, Peter
 */

#ifndef BUTLER_EDITACCOUNTVIEW_H
#define BUTLER_EDITACCOUNTVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>

class EditAccountView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditAccountView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~EditAccountView() {}

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
	void applyLayout();
	void relayout();
	void updateToolButtonStates();
	void prevClickedSlot();
	void nextClickedSlot();
	void saveSlot();
	void resetSlot();

private:
	const csjp::String & dbname;
	AccountModel & model;
	QModelIndex cursor;
	Account account;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	InputEditor currencyEditor;
	InputEditor ibanEditor;
	InputEditor swiftCodeEditor;
	ComboSelector partnerEditor;
};

#endif
