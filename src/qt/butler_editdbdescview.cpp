/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editdbdescview.h"

SCC TidContext = "EditDatabaseDescriptorView";

SCC TidNewDatabaseDescriptorWindowTitle = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Add new database connection");
SCC TidEditDatabaseDescriptorWindowTitle = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Editing database connection");

SCC TidDriverOptions = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Driver:");
SCC TidPsqlDriverOption = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Postgre SQL");
SCC TidMysqlDriverOption = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "My SQL");
SCC TidSqliteDriverOption = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "SQLite");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Previous database connection");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Next database connection");

SCC TidDbConnName = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Name for database connection:");
SCC TidDbName = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Database name:");
SCC TidUsername = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Username:");
SCC TidPassword = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Password:");
SCC TidSavePassword = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Save plain password :");
SCC TidHost = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Host:");
SCC TidPort = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Port:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Please fill the name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Database connection is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditDatabaseDescriptorView", "Database connection is updated.");

EditDatabaseDescriptorView::EditDatabaseDescriptorView(DatabaseDescriptorModel & model, QWidget * parent) :
	PannView(parent),
	model(model),
	doneButton(TidDoneButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Return)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	prevButton(TidPrevButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Left)),
	nextButton(TidNextButton, TidContext, QKeySequence(Qt::CTRL + Qt::Key_Right))
{
	setWindowModality(Qt::ApplicationModal);

	ENSURE(!cursor.isValid(), csjp::LogicError);

	driverOptions.group.addButton(&sqliteDriverOption);
	(void)TidPsqlDriverOption;
	(void)TidMysqlDriverOption;
#ifdef PGSQL
	driverOptions.group.addButton(&psqlDriverOption);
#endif
#ifdef MYSQL
	driverOptions.group.addButton(&mysqlDriverOption);
#endif

	toolBar.addToolWidget(doneButton);
	toolBar.addToolWidget(resetButton);
	footerBar.addToolWidget(prevButton);
	footerBar.addToolWidget(nextButton);
	footerBar.expanding = true;
	footerBar.spacerEnd = false;

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextSlot()));

	connect(&passwordEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(passwordFieldModified()));

	connect(&nameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&databaseNameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&usernameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&passwordEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&savePasswordCheckBox.box, SIGNAL(stateChanged(int)),
			this, SLOT(updateToolButtonStates()));
	connect(&hostEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&portEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&sqliteDriverOption, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
#ifdef PGSQL
	connect(&psqlDriverOption, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
#endif
#ifdef MYSQL
	connect(&mysqlDriverOption, SIGNAL(toggled(bool)), this, SLOT(updateToolButtonStates()));
#endif

	passwordEditor.editor.setEchoMode(QLineEdit::PasswordEchoOnEdit);

	setupView();
	retranslate();
	loadState();
}

void EditDatabaseDescriptorView::mapToGui()
{
	if(cursor.isValid())
		databaseDescriptor = DatabaseDescriptor(model.data(cursor.row()));

	nameEditor.editor.setText(databaseDescriptor.name);
	databaseNameEditor.editor.setText(databaseDescriptor.databaseName);
	usernameEditor.editor.setText(databaseDescriptor.username);
	passwordEditor.editor.setText(databaseDescriptor.password);
	savePasswordCheckBox.box.setCheckState(
			databaseDescriptor.savePassword ? Qt::Checked : Qt::Unchecked);
	hostEditor.editor.setText(databaseDescriptor.host);
	portEditor.setValue(databaseDescriptor.port);

	sqliteDriverOption.setChecked(false);
#ifdef PGSQL
	psqlDriverOption.setChecked(false);
#endif
#ifdef MYSQL
	mysqlDriverOption.setChecked(false);
#endif

	if(databaseDescriptor.driver == "SQLite")
		sqliteDriverOption.setChecked(true);
#ifdef PGSQL
	else if(databaseDescriptor.driver == "PSql")
		psqlDriverOption.setChecked(true);
#endif
#ifdef MYSQL
	else if(databaseDescriptor.driver == "MySQL")
		mysqlDriverOption.setChecked(true);
#endif

	updateToolButtonStates();
}

void EditDatabaseDescriptorView::mapFromGui()
{
	databaseDescriptor.name <<= nameEditor.editor.text();
	databaseDescriptor.databaseName <<= databaseNameEditor.editor.text();
	databaseDescriptor.username <<= usernameEditor.editor.text();
	databaseDescriptor.password <<= passwordEditor.editor.text();
	databaseDescriptor.savePassword = savePasswordCheckBox.box.checkState() == Qt::Checked;
	databaseDescriptor.host <<= hostEditor.editor.text();
	databaseDescriptor.port = portEditor.value();

	if(driverOptions.group.checkedButton() == &sqliteDriverOption)
		databaseDescriptor.driver = SqlDriver::SQLite;
#ifdef PGSQL
	else if(driverOptions.group.checkedButton() == &psqlDriverOption)
		databaseDescriptor.driver = SqlDriver::PSql;
#endif
#ifdef MYSQL
	else if(driverOptions.group.checkedButton() == &mysqlDriverOption)
		databaseDescriptor.driver = SqlDriver::MySQL;
#endif
}

void EditDatabaseDescriptorView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditDatabaseDescriptorWindowTitle));
	else
		setWindowTitle(tr(TidNewDatabaseDescriptorWindowTitle));

	driverOptions.label.setText(tr(TidDriverOptions));

	sqliteDriverOption.setText(tr(TidSqliteDriverOption));
#ifdef PGSQL
	psqlDriverOption.setText(tr(TidPsqlDriverOption));
#endif
#ifdef MYSQL
	mysqlDriverOption.setText(tr(TidMysqlDriverOption));
#endif

	nameEditor.label.setText(tr(TidDbConnName));
	databaseNameEditor.label.setText(tr(TidDbName));
	usernameEditor.label.setText(tr(TidUsername));
	passwordEditor.label.setText(tr(TidPassword));
	savePasswordCheckBox.label.setText(tr(TidSavePassword));
	hostEditor.label.setText(tr(TidHost));
	portEditor.label.setText(tr(TidPort));

	relayout();
}

void EditDatabaseDescriptorView::applyLayout(LayoutState state)
{
	QBoxLayout * driverOptionsLayout = 0;

	if(state == LayoutState::Narrow)
		driverOptionsLayout = new VLayout;
	else
		driverOptionsLayout = new HLayout;

	driverOptionsLayout->addWidget(&driverOptions);
	driverOptionsLayout->addStretch(0);
	driverOptionsLayout->addWidget(&sqliteDriverOption);
	driverOptionsLayout->addStretch(0);
#ifdef PGSQL
	driverOptionsLayout->addWidget(&psqlDriverOption);
	driverOptionsLayout->addStretch(0);
#endif
#ifdef MYSQL
	driverOptionsLayout->addWidget(&mysqlDriverOption);
	driverOptionsLayout->addStretch(0);
#endif

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	mainLayout->addStretch(0);
	mainLayout->addLayout(driverOptionsLayout);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&databaseNameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&usernameEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&passwordEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&savePasswordCheckBox);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&hostEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&portEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditDatabaseDescriptorView::relayout()
{
	LayoutState newState = LayoutState::Wide;
	{
		nameEditor.wideLayout();
		databaseNameEditor.wideLayout();
		usernameEditor.wideLayout();
		passwordEditor.wideLayout();
		hostEditor.wideLayout();
		portEditor.wideLayout();
		applyLayout(newState);
	}
	if(width() < sizeHint().width()){
		newState = LayoutState::Narrow;
		nameEditor.narrowLayout();
		databaseNameEditor.narrowLayout();
		usernameEditor.narrowLayout();
		passwordEditor.narrowLayout();
		hostEditor.narrowLayout();
		portEditor.narrowLayout();
		applyLayout(newState);
	}

	updateToolButtonStates();
}

void EditDatabaseDescriptorView::passwordFieldModified()
{
	/* Use case to avoid:
	 * - user dont want to save his password
	 * - cracker writes bad password into config file, thus turns on desc.savePassword value
	 * - user can not login so needs to set his real password
	 * - now, if desc.savePassword remains true, password gets saved and cracker can read it
	 */
//	if(databaseDescriptor.password != passwordEditor.editor.text())
		savePasswordCheckBox.box.setCheckState(Qt::Unchecked);
}

void EditDatabaseDescriptorView::updateToolButtonStates()
{
	bool modified = !(
			Text(databaseDescriptor.name) == nameEditor.editor.text() &&
			Text(databaseDescriptor.databaseName) == databaseNameEditor.editor.text() &&
			Text(databaseDescriptor.username) == usernameEditor.editor.text() &&
			Text(databaseDescriptor.password) == passwordEditor.editor.text() &&
			databaseDescriptor.savePassword == (
					savePasswordCheckBox.box.checkState() == Qt::Checked) &&
			Text(databaseDescriptor.host) == hostEditor.editor.text() &&
			databaseDescriptor.port == portEditor.value()
			);

	if(!modified){
		if(databaseDescriptor.driver == "SQLite" &&
				driverOptions.group.checkedButton() != &sqliteDriverOption)
			modified = true;
#ifdef PGSQL
		else if(databaseDescriptor.driver == "PSql" &&
				driverOptions.group.checkedButton() != &psqlDriverOption)
			modified = true;
#endif
#ifdef MYSQL
		else if(databaseDescriptor.driver == "MySQL" &&
				driverOptions.group.checkedButton() != &mysqlDriverOption)
			modified = true;
#endif
	}

	bool mandatoriesGiven = nameEditor.editor.text().size();

	footerBar.show(); /* We cant set visible status for a widget having hidden parent. */
	prevButton.setVisible(!modified && cursor.isValid() && 0 < cursor.row());
	nextButton.setVisible(!modified && cursor.isValid() && cursor.row() < model.rowCount()-1);
	doneButton.setVisible(mandatoriesGiven && modified);
	resetButton.setVisible(modified);

	if(modified){
		if(!mandatoriesGiven)
			toolBar.setInfo(tr(TidInfoMandatoryFields));
		else
			toolBar.clearInfo();
	}

	if(driverOptions.group.checkedButton() == &sqliteDriverOption){
		usernameEditor.setVisible(false);
		passwordEditor.setVisible(false);
		savePasswordCheckBox.setVisible(false);
		hostEditor.setVisible(false);
		portEditor.setVisible(false);
	}
#ifdef PGSQL
	else if(driverOptions.group.checkedButton() == &psqlDriverOption){
		usernameEditor.setVisible(true);
		passwordEditor.setVisible(true);
		savePasswordCheckBox.setVisible(true);
		hostEditor.setVisible(true);
		portEditor.setVisible(true);
	}
#endif
#ifdef MYSQL
	else if(driverOptions.group.checkedButton() == &mysqlDriverOption){
		usernameEditor.setVisible(true);
		passwordEditor.setVisible(true);
		savePasswordCheckBox.setVisible(true);
		hostEditor.setVisible(true);
		portEditor.setVisible(true);
	}
#endif

	toolBar.updateButtons();
	footerBar.updateButtons();
}

void EditDatabaseDescriptorView::saveSlotSpec()
{
}

@include@ views.cpp
@declare@ DatabaseDescriptor

@include@ setCursor prevSlot nextSlot saveSlot resetSlot
@include@ showEvent closeEvent loadState saveState changeEvent resizeEvent

