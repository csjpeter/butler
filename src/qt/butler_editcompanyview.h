/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITCOMPANYVIEW_H
#define BUTLER_EDITCOMPANYVIEW_H

#include <butler_dataclasses.h>
#include <butler_pannview.h>

class EditCompanyView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditCompanyView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~EditCompanyView() {}

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
	void prevSlot();
	void nextSlot();
	void saveSlotSpec();
	void saveSlot();
	void resetSlot();

private:
	const csjp::String & dbname;
	CompanyModel & model;
	QModelIndex cursor;
	Company company;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	InputEditor countryEditor;
	InputEditor cityEditor;
	InputEditor postalCodeEditor;
	InputEditor addressEditor;
	InputEditor taxIdEditor;
};

#endif

