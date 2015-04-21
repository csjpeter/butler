/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITDATABASEDESCRIPTORVIEW_H
#define BUTLER_EDITDATABASEDESCRIPTORVIEW_H

#include <butler_pannview.h>

class EditDatabaseDescriptorView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditDatabaseDescriptorView(DatabaseDescriptorModel & model, QWidget * parent = 0);
	virtual ~EditDatabaseDescriptorView() {}

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
	void prevSlot();
	void nextSlot();
	void saveSlotSpec();
	void saveSlot();
	void resetSlot();

private:
	DatabaseDescriptorModel & model;
	QModelIndex cursor;
	DatabaseDescriptor databaseDescriptor;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;
	
	ButtonGroup driverOptions;
#ifdef PGSQL
	QRadioButton psqlDriverOption;
#endif
#ifdef MYSQL
	QRadioButton mysqlDriverOption;
#endif
	QRadioButton sqliteDriverOption;

	InputEditor nameEditor;
	InputEditor databaseNameEditor;
	InputEditor usernameEditor;
	InputEditor passwordEditor;
	FormCheckBox savePasswordCheckBox;
	InputEditor hostEditor;
	PortEditor portEditor;
};

#endif
