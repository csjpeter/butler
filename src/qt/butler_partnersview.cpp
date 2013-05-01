/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include "butler_partnersview.h"
#include "butler_editpartnerview.h"
#include "butler_config.h"

SCC TidContext = "PartnersView";

SCC TidPartnersWindowTitle = QT_TRANSLATE_NOOP("PartnersView", "Business partners");
SCC TidAddButton = QT_TRANSLATE_NOOP("PartnersView", "Add new partner");
SCC TidEditButton = QT_TRANSLATE_NOOP("PartnersView", "Edit partner");
SCC TidDelButton = QT_TRANSLATE_NOOP("PartnersView", "Delete partner");
SCC TidRefreshButton = QT_TRANSLATE_NOOP("PartnersView", "Refresh partner list");

PartnersView::PartnersView(const QString & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(partnersModel(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			TidAddButton, TidContext, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			TidDelButton, TidContext, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			TidEditButton, TidContext, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			TidRefreshButton, TidContext, QKeySequence(QKeySequence::Refresh)),
	newPartnerView(NULL),
	editPartnerView(NULL)
{
	setWindowIcon(QIcon(Path::icon("partner.png")));

	tableView.setModel(&model);

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(newPartner()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(editPartner()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(delPartner()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

PartnersView::~PartnersView()
{
	delete newPartnerView;
	delete editPartnerView;
}

void PartnersView::retranslate()
{
	setWindowTitle(tr(TidPartnersWindowTitle));
	relayout();
}

void PartnersView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void PartnersView::relayout()
{
	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setResizeMode(
				Partner::StoreName, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setResizeMode(
				Partner::StoreName, QHeaderView::ResizeToContents);

	applyLayout();

	updateToolButtonStates();
}

void PartnersView::updateToolButtonStates()
{
	if(tableView.currentIndex().isValid()){
		editButton.show();
		delButton.show();
	} else {
		editButton.hide();
		delButton.hide();
	}
	toolBar.updateButtons();
}

void PartnersView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void PartnersView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void PartnersView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void PartnersView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void PartnersView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void PartnersView::loadState()
{
	QString prefix("PartnersView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	QString name = settings.value(prefix + "/currentitem", "").toString();
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.partnerSet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/editPartnerView", false).toBool())
		QTimer::singleShot(0, this, SLOT(editItem()));

	updateToolButtonStates();
}

void PartnersView::saveState()
{
	QString prefix("PartnersView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.partner(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(editPartnerView);
}

void PartnersView::newPartner()
{
	if(!newPartnerView)
		newPartnerView = new EditPartnerView(dbname);

	newPartnerView->activate();
}

void PartnersView::editPartner()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select partner first."));
		return;
	}

	if(!editPartnerView)
		editPartnerView = new EditPartnerView(dbname);

	editPartnerView->setCursor(tableView.currentIndex());
	editPartnerView->activate();
}

void PartnersView::delPartner()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select partner first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const Partner &partner = model.partner(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a partner"),
			tr("Shall we delete this partner: ") + partner.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void PartnersView::refresh()
{
	QString name;
	if(tableView.currentIndex().isValid())
		name = model.partner(tableView.currentIndex().row()).name;

	model.query();

	if(model.partnerSet().has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void PartnersView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order == Qt::AscendingOrder);
}

void PartnersView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
