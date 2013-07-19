/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_editdbdescview.h"
#include "butler_databasesmodel.h"

SCC TidContext = "EditDbDescView";

SCC TidNewCompanyWindowTitle = QT_TRANSLATE_NOOP("EditDbDescView", "Add new database connection");
SCC TidEditCompanyWindowTitle = QT_TRANSLATE_NOOP("EditDbDescView", "Editing database connection");

SCC TidDriverOptions = QT_TRANSLATE_NOOP("EditDbDescView", "Driver:");
SCC TidSqliteDriverOption = QT_TRANSLATE_NOOP("EditDbDescView", "SQLite");
SCC TidPsqlDriverOption = QT_TRANSLATE_NOOP("EditDbDescView", "Postgre SQL");

SCC TidDoneButton = QT_TRANSLATE_NOOP("EditDbDescView", "Done");
SCC TidResetButton = QT_TRANSLATE_NOOP("EditDbDescView", "Reset");
SCC TidPrevButton = QT_TRANSLATE_NOOP("EditDbDescView", "Previous database connection");
SCC TidNextButton = QT_TRANSLATE_NOOP("EditDbDescView", "Next database connection");

SCC TidCompanyName = QT_TRANSLATE_NOOP("EditDbDescView", "Name for database connection:");
SCC TidCompanyCountry = QT_TRANSLATE_NOOP("EditDbDescView", "Database name:");
SCC TidCompanyCity = QT_TRANSLATE_NOOP("EditDbDescView", "Username:");
SCC TidCompanyPostalCode = QT_TRANSLATE_NOOP("EditDbDescView", "Password:");
SCC TidCompanyAddress = QT_TRANSLATE_NOOP("EditDbDescView", "Host:");
SCC TidCompanyTaxId = QT_TRANSLATE_NOOP("EditDbDescView", "Port:");

SCC TidInfoMandatoryFields = QT_TRANSLATE_NOOP("EditDbDescView", "Please fill the name field.");
SCC TidInfoNewSaved = QT_TRANSLATE_NOOP("EditDbDescView", "Database connection is saved, you may add another.");
SCC TidInfoEditSaved = QT_TRANSLATE_NOOP("EditDbDescView", "Database connection is updated.");

EditDbDescView::EditDbDescView(DatabasesModel & model, QWidget * parent) :
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
	driverOptions.group.addButton(&psqlDriverOption);

	toolBar.addToolWidget(doneButton);
	toolBar.addToolWidget(resetButton);
	footerBar.addToolWidget(prevButton);
	footerBar.addToolWidget(nextButton);
	footerBar.expanding = true;
	footerBar.spacerEnd = false;

	connect(&doneButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));
	connect(&prevButton, SIGNAL(clicked()), this, SLOT(prevClickedSlot()));
	connect(&nextButton, SIGNAL(clicked()), this, SLOT(nextClickedSlot()));

	connect(&nameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&databaseNameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&usernameEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&passwordEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&hostEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&portEditor.editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	loadState();
	retranslate();
}

void EditDbDescView::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}

void EditDbDescView::closeEvent(QCloseEvent *event)
{
	saveState();

	PannView::closeEvent(event);
}

void EditDbDescView::loadState()
{
	QString prefix(cursor.isValid() ? "EditDbDescView" : "NewCompanyView");
	PannView::loadState(prefix);
}

void EditDbDescView::saveState()
{
	QString prefix(cursor.isValid() ? "EditDbDescView" : "NewCompanyView");
	PannView::saveState(prefix);
}

void EditDbDescView::mapToGui()
{
	if(cursor.isValid())
		dbdesc = DatabaseDescriptor(model.query(cursor.row()));

	nameEditor.editor.setText(dbdesc.name);
	databaseNameEditor.editor.setText(dbdesc.databaseName);
	usernameEditor.editor.setText(dbdesc.username);
	passwordEditor.editor.setText(dbdesc.password);
	hostEditor.editor.setText(dbdesc.host);
	portEditor.setValue(dbdesc.port);

	updateToolButtonStates();
}

void EditDbDescView::mapFromGui()
{
	dbdesc.name = nameEditor.editor.text();
	dbdesc.databaseName = databaseNameEditor.editor.text();
	dbdesc.username = usernameEditor.editor.text();
	dbdesc.password = passwordEditor.editor.text();
	dbdesc.host = hostEditor.editor.text();
	dbdesc.port = portEditor.value();
}

void EditDbDescView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void EditDbDescView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}

void EditDbDescView::retranslate()
{
	if(cursor.isValid())
		setWindowTitle(tr(TidEditCompanyWindowTitle));
	else
		setWindowTitle(tr(TidNewCompanyWindowTitle));

	driverOptions.label.setText(tr(TidDriverOptions));
	sqliteDriverOption.setText(tr(TidSqliteDriverOption));
	psqlDriverOption.setText(tr(TidPsqlDriverOption));

	nameEditor.label.setText(tr(TidCompanyName));
	databaseNameEditor.label.setText(tr(TidCompanyCountry));
	usernameEditor.label.setText(tr(TidCompanyCity));
	passwordEditor.label.setText(tr(TidCompanyPostalCode));
	hostEditor.label.setText(tr(TidCompanyAddress));
	portEditor.label.setText(tr(TidCompanyTaxId));

	relayout();
}

void EditDbDescView::applyLayout(LayoutState state)
{
	QBoxLayout * driverOptionsLayout = 0;

	if(state == LayoutState::Narrow)
		driverOptionsLayout = new VLayout;
	else
		driverOptionsLayout = new HLayout;

	driverOptionsLayout->addWidget(&sqliteDriverOption);
	driverOptionsLayout->addStretch(0);
	driverOptionsLayout->addWidget(&psqlDriverOption);
	driverOptionsLayout->addStretch(0);

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
	mainLayout->addWidget(&hostEditor);
	mainLayout->addStretch(0);
	mainLayout->addWidget(&portEditor);
	mainLayout->addStretch(0);

	delete layout();
	setLayout(mainLayout);
	updateGeometry();
}

void EditDbDescView::relayout()
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

void EditDbDescView::updateToolButtonStates()
{
	bool modified = !(
			dbdesc.name == nameEditor.editor.text() &&
			dbdesc.databaseName == databaseNameEditor.editor.text() &&
			dbdesc.username == usernameEditor.editor.text() &&
			dbdesc.password == passwordEditor.editor.text() &&
			dbdesc.host == hostEditor.editor.text() &&
			dbdesc.port == portEditor.value()
			);

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

	toolBar.updateButtons();
	footerBar.updateButtons();
}

void EditDbDescView::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEditCompanyWindowTitle));
	mapToGui();
}

void EditDbDescView::prevClickedSlot()
{
	int col = cursor.column();
	int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void EditDbDescView::nextClickedSlot()
{
	int col = cursor.column();
	int row = (cursor.row() < model.rowCount() - 1) ?
		(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}

void EditDbDescView::saveSlot()
{
	mapFromGui();

	if(cursor.isValid()){
		if(model.query(cursor.row()) != dbdesc)
			model.update(cursor.row(), dbdesc);
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(dbdesc);

		dbdesc = DatabaseDescriptor();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}

void EditDbDescView::resetSlot()
{
	mapToGui();
}
