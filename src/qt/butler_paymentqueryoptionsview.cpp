/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_paymentqueryoptionsview.h"

SCC TidContext = "PaymentQueryOptionsView";

SCC TidEditQueryWindowTitle = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Select and edit query");

SCC TidQueryButton = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Query");
SCC TidSaveButton = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Save");
SCC TidResetButton = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Reset");
SCC TidDelButton = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Delete");

SCC TidQuerySelector = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Query name:");
SCC TidPartnerFilter = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Filter by partner:");
SCC TidPartnerSelector = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Business partner:");

SCC TidFromDateTimeEditor = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "From:");
SCC TidTillDateTimeEditor = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Till:");

SCC TidQuerySavedInfo = QT_TRANSLATE_NOOP("PaymentQueryOptionsView", "Query is saved.");

PaymentQueryOptionsView::PaymentQueryOptionsView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	queryButton(TidQueryButton, TidContext, QKeySequence(Qt::ALT + Qt::Key_Enter)),
	saveButton(TidSaveButton, TidContext, QKeySequence(QKeySequence::Save)),
	delButton(TidDelButton, TidContext, QKeySequence(QKeySequence::Delete)),
	resetButton(TidResetButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	nameEditor(&paymentQueryModel(dbname), PaymentQuery::Name),
	partnerSelector(&partnerModel(dbname), Partner::Name)
{
	setWindowModality(Qt::ApplicationModal);

	query.endDate = DateTime::now();

	toolBar.addToolWidget(queryButton);
	toolBar.addToolWidget(saveButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(resetButton);

	connect(&queryButton, SIGNAL(clicked()), this, SLOT(querySlot()));
	connect(&saveButton, SIGNAL(clicked()), this, SLOT(saveSlot()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delSlot()));
	connect(&resetButton, SIGNAL(clicked()), this, SLOT(resetSlot()));

	connect(&partnerFilter.box, SIGNAL(stateChanged(int)), this, SLOT(layoutContentChangeSlot()));

	connect(&nameEditor.box, SIGNAL(activated(const QString &)),
			this, SLOT(querySelectedSlot()));
	connect(&nameEditor.editor, SIGNAL(editingFinished()), this, SLOT(querySelectedSlot()));

	connect(&nameEditor.box, SIGNAL(editTextChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));
	connect(&startDate.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&endDate.edit, SIGNAL(dateTimeChanged(const QDateTime &)),
			this, SLOT(updateToolButtonStates()));
	connect(&partnerSelector.box, SIGNAL(currentIndexChanged(const QString &)),
			this, SLOT(updateToolButtonStates()));

	setupView();
	retranslate();
	loadState();
}

PaymentQueryOptionsView::~PaymentQueryOptionsView()
{
}

void PaymentQueryOptionsView::showEvent(QShowEvent *event)
{
	mapToGui();
	PannView::showEvent(event);
	relayout();
}

void PaymentQueryOptionsView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PaymentQueryOptionsView::loadState()
{
	QString prefix("PaymentQueryOptionsView");
	PannView::loadState(prefix);
}

void PaymentQueryOptionsView::saveState()
{
	QString prefix("PaymentQueryOptionsView");
	PannView::saveState(prefix);
}

void PaymentQueryOptionsView::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PaymentQueryOptionsView::resizeEvent(QResizeEvent *event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	updateGeometry();
	relayout();
}

void PaymentQueryOptionsView::mapToGui()
{
	nameEditor.setText(query.name);
	startDate.edit.setDateTime(query.startDate);
	endDate.edit.setDateTime(query.endDate);

	if(query.partners.size()){
		PartnerModel & sm = partnerModel(dbname);
		int row = sm.index(query.partners.queryAt(0).partner);
		if(0 <= row){
			partnerFilter.box.blockSignals(true);
			partnerFilter.box.setChecked(true);
			partnerFilter.box.blockSignals(false);
			partnerSelector.box.setCurrentIndex(row);
		}
	} else {
		partnerFilter.box.blockSignals(true);
		partnerFilter.box.setChecked(false);
		partnerFilter.box.blockSignals(false);
	}

	updateToolButtonStates();
}

void PaymentQueryOptionsView::mapFromGui()
{
	query.name = nameEditor.text();
	query.startDate = startDate.edit.dateTime();
	query.endDate = endDate.edit.dateTime();
	
	query.partners.clear();
	if(partnerFilter.box.isChecked()){
		int i = partnerSelector.box.currentIndex();
		PartnerModel & sm = partnerModel(dbname);
		if(0 <= i && i < sm.rowCount())
			query.partners.add(new PaymentQueryPartner(query.name, sm.data(i).name.trimmed()));
	}
}

void PaymentQueryOptionsView::retranslate()
{
	setWindowTitle(tr(TidEditQueryWindowTitle));

	nameEditor.label.setText(tr(TidQuerySelector));

	startDate.label.setText(tr(TidFromDateTimeEditor));
	endDate.label.setText(tr(TidTillDateTimeEditor));

	partnerFilter.label.setText(tr(TidPartnerFilter));
	partnerSelector.label.setText(tr(TidPartnerSelector));

	relayout();
}

void PaymentQueryOptionsView::applyLayout(LayoutState state, bool test)
{
	(void)test;
	delete layout();

	HLayout * filterLayout = 0;
	if(state == LayoutState::Wide){
		filterLayout = new HLayout;
		filterLayout->addWidget(&partnerFilter);
		filterLayout->addStretch(0);
	}

	HLayout * filter1Layout = 0;
	if(state == LayoutState::Medium){
		filter1Layout = new HLayout;
		filter1Layout->addStretch(0);
		filter1Layout->addWidget(&partnerFilter);
		filter1Layout->addStretch(0);
	}

	HLayout * dateLayout = 0;
	if(state == LayoutState::Wide){
		dateLayout = new HLayout;
		dateLayout->addWidget(&startDate, 4);
		dateLayout->addStretch(1);
		dateLayout->addWidget(&endDate, 4);
	}

	VLayout * mainLayout = new VLayout;
	mainLayout->addStretch(0);
	mainLayout->addWidget(&nameEditor);
	if(state == LayoutState::Wide){
		mainLayout->addStretch(0);
		mainLayout->addLayout(dateLayout);
	} else {
		mainLayout->addStretch(0);
		mainLayout->addWidget(&startDate);
		mainLayout->addStretch(0);
		mainLayout->addWidget(&endDate);
	}
	if(state == LayoutState::Wide){
		mainLayout->addStretch(0);
		mainLayout->addLayout(filterLayout);
	}
	if(state == LayoutState::Medium){
		mainLayout->addStretch(0);
		mainLayout->addLayout(filter1Layout);
	}
	if(state == LayoutState::Narrow){
		mainLayout->addStretch(0);
		mainLayout->addWidget(&partnerFilter);
	}
	mainLayout->addStretch(0);
	mainLayout->addWidget(&partnerSelector);

	setLayout(mainLayout);
	updateGeometry();
}

void PaymentQueryOptionsView::relayout()
{
	LayoutState newState = LayoutState::Wide;
	nameEditor.wideLayout();
	startDate.wideLayout();
	endDate.wideLayout();
	partnerSelector.wideLayout();
	applyLayout(newState, true);

	if(width() < sizeHint().width()){
		newState = LayoutState::Medium;
		nameEditor.wideLayout();
		startDate.wideLayout();
		endDate.wideLayout();
		partnerSelector.wideLayout();
		applyLayout(newState, true);
	}
	if(width() < sizeHint().width()){
		newState = LayoutState::Narrow;
		nameEditor.narrowLayout();
		startDate.narrowLayout();
		endDate.narrowLayout();
		partnerSelector.narrowLayout();
		applyLayout(newState, true);
	}

	applyLayout(newState);
	updateToolButtonStates();
}

void PaymentQueryOptionsView::saveSlotSpec()
{
}

void PaymentQueryOptionsView::saveSlot()
{
	mapFromGui();

	PaymentQueryModel & qm = paymentQueryModel(dbname);
	if(qm.set.has(query.name)) {
		if(qm.set.query(query.name) != query)
			qm.update(qm.index(query.name), query);
	} else
		qm.addNew(query);
	toolBar.setInfo(tr(TidQuerySavedInfo));
	updateToolButtonStates();
}

void PaymentQueryOptionsView::delSlot()
{
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a query"),
			tr("Shall we delete this query: ") + query.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes){
		PaymentQueryModel & qm = paymentQueryModel(dbname);
		if(qm.set.has(query.name))
			qm.del(qm.index(query.name));
		query = PaymentQuery();
		query.endDate = DateTime::now();
		mapToGui();
	}
}

void PaymentQueryOptionsView::querySlot()
{
	mapFromGui();

	PaymentQueryModel & qm = paymentQueryModel(dbname);
	if(!qm.set.has(query.name))
		query.name = "";
	else if(qm.set.query(query.name) != query)
		query.name = "";

	accept();
}

void PaymentQueryOptionsView::resetSlot()
{
	mapToGui();
}

void PaymentQueryOptionsView::backSlot()
{
	reject();
}

void PaymentQueryOptionsView::querySelectedSlot()
{
	PaymentQueryModel & qm = paymentQueryModel(dbname);
	Text name(nameEditor.text());
	if(!qm.set.has(name))
		return;
	query = qm.set.query(name);
	mapToGui();
}

void PaymentQueryOptionsView::layoutContentChangeSlot()
{
	updateToolButtonStates();
	relayout();
}

void PaymentQueryOptionsView::updateToolButtonStates()
{
	bool modified = !(
			query.name == nameEditor.text() &&
			query.startDate == startDate.edit.dateTime() &&
//			query.endDate == endDate.edit.dateTime() &&

			((0<query.partners.size())==(partnerFilter.box.checkState()==Qt::Checked))
			);
	if(!modified)
		if(query.partners.size() && (partnerFilter.box.checkState()==Qt::Checked))
			if(query.partners.queryAt(0).partner != partnerSelector.box.currentText())
				modified = true;

	bool hasName(nameEditor.text().size());

	/* Lets adjust states */

	saveButton.setVisible(modified && hasName);
	delButton.setVisible(!modified && hasName);
	resetButton.setVisible(modified);

	partnerSelector.setVisible(partnerFilter.box.checkState()==Qt::Checked);

	if(modified)
		toolBar.clearInfo();

	toolBar.updateButtons();
}
