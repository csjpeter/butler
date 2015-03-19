/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITWAREVIEW_H
#define BUTLER_EDITWAREVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>
#include <butler_tagwidget.h>

class EditWareView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditWareView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~EditWareView() {}

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

private:
	const csjp::String & dbname;
	WareModel & model;
	QModelIndex cursor;
	Ware ware;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	InputEditor unitEditor;
	InputEditor typesEditor;
	TagWidget tagsWidget;
};

#endif
