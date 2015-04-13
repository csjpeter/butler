/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYOPTIONSVIEW_H
#define BUTLER_QUERYOPTIONSVIEW_H

#include <butler_dataclasses.h>
#include <butler_tagwidget.h>
#include <butler_pannview.h>

class QueryOptionsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	QueryOptionsView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~QueryOptionsView();

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
	void applyLayout(LayoutState state, bool test = false);
	void relayout();
	void querySlot();
	void saveSlotSpec();
	void saveSlot();
	void delSlot();
	void resetSlot();
	void backSlot();
	void querySelectedSlot();
	void layoutContentChangeSlot();
	void updateToolButtonStates();

private:
	const csjp::String & dbname;

public:
	Query query;

private:
	Button queryButton;
	Button saveButton;
	Button delButton;
	Button resetButton;

	ComboSelector nameEditor;
	DateTimeEditor startDate;
	DateTimeEditor endDate;
	Selector wareSelector;
	FormCheckBox wareFilter;
	Selector partnerSelector;
	FormCheckBox partnerFilter;

	ButtonGroup stockOptions;
	QRadioButton stockOptAll;
	QRadioButton stockOptOnStock;
	QRadioButton stockOptUsedUp;
	
	ButtonGroup tagOptions;
	QRadioButton tagOptAllMatch;
	QRadioButton tagOptAnyMatch;

	FormCheckBox withTagFilter;
	TagWidget tagsWidget;
	FormCheckBox withoutTagFilter;
	TagWidget withoutTagsWidget;
};

#endif
