/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYOPTIONSVIEW_H
#define BUTLER_QUERYOPTIONSVIEW_H

#include <butler_query.h>
#include <butler_tagwidget.h>

class QueryOptionsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	QueryOptionsView(const QString & dbname, QWidget * parent = 0);
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
	void queryClickedSlot();
	void saveClickedSlot();
	void delClickedSlot();
	void resetClickedSlot();
	void backClickedSlot();
	void querySelectedSlot();
	void layoutContentChangeSlot();
	void updateToolButtonStates();

private:
	const QString & dbname;

public:
	Query query;

private:
	ToolBar toolBar;
	Button queryButton;
	Button saveButton;
	Button delButton;
	Button resetButton;
	InfoLabel infoLabel;

	ComboSelector nameEditor;
	DateTimeEditor startDate;
	DateTimeEditor endDate;
	Selector wareSelector;
	FormCheckBox wareFilter;
	Selector shopSelector;
	FormCheckBox shopFilter;

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
