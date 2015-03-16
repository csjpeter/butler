/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITINVENTORYVIEW_H
#define BUTLER_EDITINVENTORYVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>

class EditInventoryView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditInventoryView(const QString & dbname, QWidget * parent = 0);
	virtual ~EditInventoryView() {}

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
	const QString & dbname;
	InventoryModel & model;
	QModelIndex cursor;
	Inventory inventory;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	CommentEditor commentEditor;
};

#endif
