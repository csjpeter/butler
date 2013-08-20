/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITCOMPANYVIEW_H
#define BUTLER_EDITCOMPANYVIEW_H

#include <butler_pannview.h>
#include <butler_databasesmodel.h>

class EditDbDescView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditDbDescView(DatabasesModel & model, QWidget * parent = 0);
	virtual ~EditDbDescView() {}

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
	void applyLayout(LayoutState state);
	void relayout();
	void passwordFieldModified();
	void updateToolButtonStates();
	void prevClickedSlot();
	void nextClickedSlot();
	void saveSlot();
	void resetSlot();

private:
	DatabasesModel & model;
	QModelIndex cursor;
	DatabaseDescriptor dbdesc;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;
	
	ButtonGroup driverOptions;
	QRadioButton sqliteDriverOption;
	QRadioButton psqlDriverOption;

	InputEditor nameEditor;
	InputEditor databaseNameEditor;
	InputEditor usernameEditor;
	InputEditor passwordEditor;
	FormCheckBox savePasswordCheckBox;
	InputEditor hostEditor;
	PortEditor portEditor;
};

#endif
