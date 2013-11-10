/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITBRANDVIEW_H
#define BUTLER_EDITBRANDVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>

class EditBrandView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditBrandView(const QString & dbname, QWidget * parent = 0);
	virtual ~EditBrandView() {}

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
	BrandsModel & model;
	QModelIndex cursor;
	Brand brand;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	ComboSelector companyEditor;
};

#endif
