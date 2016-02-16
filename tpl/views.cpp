
@Define{showEvent@
void Edit@Type@View::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}
@}Define@

@Define{closeEvent@
void Edit@Type@View::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}
@}Define@

@Define{loadState@
void Edit@Type@View::loadState()
{
	QString prefix(cursor.isValid() ? "Edit@Type@View" : "New@Type@View");
	PannView::loadState(prefix);
}
@}Define@

@Define{saveState@
void Edit@Type@View::saveState()
{
	QString prefix(cursor.isValid() ? "Edit@Type@View" : "New@Type@View");
	PannView::saveState(prefix);
}
@}Define@

@Define{changeEvent@
void Edit@Type@View::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}
@}Define@

@Define{resizeEvent@
void Edit@Type@View::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}
@}Define@

@Define{setCursor@
void Edit@Type@View::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEdit@Type@WindowTitle));
	mapToGui();
}
@}Define@

@Define{prevSlot@
void Edit@Type@View::prevSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}
@}Define@

@Define{nextSlot@
void Edit@Type@View::nextSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (cursor.row() < model.rowCount() - 1) ?
			(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}
@}Define@

@Define{saveSlot@
void Edit@Type@View::saveSlot()
{
	mapFromGui();

	saveSlotSpec();

	if(cursor.isValid()){
		if(model.data(cursor.row()) != @type@)
			model.update(cursor.row(), @type@);
		auto row = model.set.index(@For{KeyField@@type@.@.Name@, @-@@type@.@.Name@@}@);
		setCursor(model.index(row, cursor.column()));
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(@type@);

		@type@ = @Type@();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		//nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}
@}Define@

@Define{resetSlot@
void Edit@Type@View::resetSlot()
{
	mapToGui();
}
@}Define@

@Define{simpleListViewImplementation@

#include "butler_edit@type@view.h"

@Type@sView::@Type@sView(const csjp::String & dbname, QWidget * parent) :
	PannView(parent),
	dbname(dbname),
	model(@type@Model(dbname)),
	addButton(QIcon(Path::icon("add.png")),
			Tid@Type@AddButton, Tid@Type@Context, QKeySequence(Qt::Key_F1)),
	delButton(QIcon(Path::icon("delete.png")),
			Tid@Type@DelButton, Tid@Type@Context, QKeySequence(Qt::Key_F2)),
	editButton(QIcon(Path::icon("edit.png")),
			Tid@Type@EditButton, Tid@Type@Context, QKeySequence(Qt::Key_F3)),
	refreshButton(QIcon(Path::icon("refresh.png")),
			Tid@Type@RefreshButton, Tid@Type@Context, QKeySequence(QKeySequence::Refresh)),
	new@Type@View(NULL),
	edit@Type@View(NULL)
{
	setWindowIcon(QIcon(Path::icon("@type@.png")));

	tableView.setModel(&model);
	tableView.hideColumn(@Type@::LastModified);
	tableView.hideColumn(@Type@::Deleted);
	tableView.hideColumn(@Type@::NumOfFields);
	@For{Field@@IfIsSpec{@
	tableView.hideColumn(@Type@::@.EnumName@);
	@IfIsSpec}@@}@

	toolBar.addToolWidget(addButton);
	toolBar.addToolWidget(editButton);
	toolBar.addToolWidget(delButton);
	toolBar.addToolWidget(refreshButton);

	connect(&addButton, SIGNAL(clicked()), this, SLOT(new@Type@()));
	connect(&editButton, SIGNAL(clicked()), this, SLOT(edit@Type@()));
	connect(&delButton, SIGNAL(clicked()), this, SLOT(del@Type@()));
	connect(&refreshButton, SIGNAL(clicked()), this, SLOT(refresh()));

	connect(&tableView, SIGNAL(currentIndexChanged(const QModelIndex &, const QModelIndex &)),
			this, SLOT(currentIndexChanged(const QModelIndex &, const QModelIndex &)));

	setupView();
	retranslate();
	loadState();
}

@Type@sView::~@Type@sView()
{
	delete new@Type@View;
	delete edit@Type@View;
}

void @Type@sView::retranslate()
{
	QString titlePrefix;
	if(dbname != "localdb"){
		titlePrefix <<= dbname;
		titlePrefix += " :: ";
	}
	setWindowTitle(titlePrefix + tr(Tid@Type@sWindowTitle));
	relayout();
}

void @Type@sView::applyLayout()
{
	delete layout();

	VLayout * mainLayout = new VLayout;
	mainLayout->addWidget(&tableView);

	setLayout(mainLayout);
}

void @Type@sView::relayout()
{
/*	if(tableView.horizontalHeader()->width() < width())
		tableView.horizontalHeader()->setSectionResizeMode(
				@Type@::Types, QHeaderView::Stretch);
	else
		tableView.horizontalHeader()->setSectionResizeMode(
				@Type@::Types, QHeaderView::ResizeToContents);
*/
	applyLayout();

	updateToolButtonStates();
}

void @Type@sView::updateToolButtonStates()
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

void @Type@sView::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}

void @Type@sView::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	relayout();
}

void @Type@sView::keyPressEvent(QKeyEvent * event)
{
	qApp->postEvent(&tableView, new QKeyEvent(*event));
}

void @Type@sView::showEvent(QShowEvent *event)
{
	PannView::showEvent(event);
	relayout();
}

void @Type@sView::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}

void @Type@sView::loadState()
{
	QString prefix("@Type@sView");
	PannView::loadState(prefix);
	QSettings settings;

	tableView.loadState(prefix);

	Text name(settings.value(prefix + "/currentitem", ""));
	int col = settings.value(prefix + "/currentitemCol", "").toInt();
	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), col));

	if(settings.value(prefix + "/edit@Type@View", false).toBool())
		QTimer::singleShot(0, this, SLOT(edit@Type@()));
}

void @Type@sView::saveState()
{
	QString prefix("@Type@sView");
	PannView::saveState(prefix);
	QSettings settings;

	QString name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;
	settings.setValue(prefix + "/currentitem", name);
	settings.setValue(prefix + "/currentitemCol", tableView.currentIndex().column());

	tableView.saveState(prefix);

	SAVE_VIEW_STATE(edit@Type@View);
}

void @Type@sView::new@Type@()
{
	if(!new@Type@View)
		new@Type@View = new Edit@Type@View(dbname);

	new@Type@View->activate();
}

void @Type@sView::edit@Type@()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select @type@ first."));
		return;
	}

	if(!edit@Type@View)
		edit@Type@View = new Edit@Type@View(dbname);

	edit@Type@View->setCursor(tableView.currentIndex());
	edit@Type@View->activate();
}

void @Type@sView::del@Type@()
{
	if(!tableView.currentIndex().isValid()){
		QMessageBox::information(this, tr("Information"),
				tr("Please select @type@ first."));
		return;
	}

	int row = tableView.currentIndex().row();
	const @Type@ & @type@ = model.data(row);
	csjp::Object<QMessageBox> msg(new QMessageBox(
			QMessageBox::Question,
			tr("Deleting a @type@"),
			tr("Shall we delete this @type@: ") + @type@.name,
			QMessageBox::Yes | QMessageBox::No,
			0, Qt::Dialog));
	if(msg->exec() == QMessageBox::Yes)
		model.del(row);
}

void @Type@sView::refresh()
{
	Text name;
	if(tableView.currentIndex().isValid())
		name = model.data(tableView.currentIndex().row()).name;

	model.query();

	if(model.set.has(name))
		tableView.setCurrentIndex(model.index(model.index(name), 0));

	tableView.horizontalScrollBar()->setValue(tableView.horizontalScrollBar()->minimum());
}

void @Type@sView::sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order)
{
	model.sort(logicalIndex, order);
}

void @Type@sView::currentIndexChanged(const QModelIndex & current, const QModelIndex & previous)
{
	if(current.isValid() == previous.isValid())
		return;
	updateToolButtonStates();
}
@}Define@

