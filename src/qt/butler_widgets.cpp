/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <butler_widgets.h>

TableView::TableView(QWidget * parent) :
	QTableView(parent),
	scroller(this)
{
	setTabKeyNavigation(false);
	setAlternatingRowColors(true);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
	setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
	setAutoScroll(true);
	setSelectionBehavior(QAbstractItemView::SelectRows);
	setSelectionMode(QAbstractItemView::SingleSelection);
	verticalHeader()->hide();
	setSortingEnabled(true);
	horizontalHeader()->setSectionsMovable(true);
	horizontalHeader()->setSortIndicatorShown(true);
	horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	scroller.stealEventFromFocusedWidgets = true;
}

void TableView::loadState(const QString & pref)
{
	if(!model())
		throw csjp::LogicError("Missing model in %.", __PRETTY_FUNCTION__);

	QString prefix(pref + "/TableView");
	QSettings settings;

	int c = model()->columnCount();
	for(int i = 0; i < c; i++){
		int pos = settings.value(prefix + "/column" + i, i).toInt();
		DBG("Lets move logical % (%, visual %) to visual % (%)",
				i,
				model()->headerData(
						i, Qt::Horizontal,
						Qt::DisplayRole).toString(),
				horizontalHeader()->visualIndex(i),
				pos,
				model()->headerData(
						horizontalHeader()->logicalIndex(pos),
						Qt::Horizontal, Qt::DisplayRole).toString()
				);
		horizontalHeader()->moveSection(horizontalHeader()->visualIndex(i), pos);
	}

	int sortPos = settings.value(prefix + "/sortColumn", 0).toInt();
	int sortOrder = settings.value(prefix + "/sortOrder", 0).toInt();
	horizontalHeader()->setSortIndicator(sortPos,
			sortOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
}

void TableView::saveState(const QString & pref)
{
	if(!model())
		return;

	QString prefix(pref + "/TableView");
	QSettings settings;

	unsigned c = model()->columnCount();
	for(unsigned i = 0; i < c; i++)
		settings.setValue(prefix + "/column" + i,
				horizontalHeader()->visualIndex(i));

	settings.setValue(prefix + "/sortColumn",
			horizontalHeader()->sortIndicatorSection());

	settings.setValue(prefix + "/sortOrder",
			horizontalHeader()->sortIndicatorOrder());
}

void TableView::currentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	QTableView::currentChanged(current, previous);
	currentIndexChanged(current, previous);
}

HLayout::HLayout() : QHBoxLayout()
{
	setContentsMargins(0,0,0,0);
	setSpacing(5);
}

VLayout::VLayout() : QVBoxLayout()
{
	setContentsMargins(0,0,0,0);
	setSpacing(5);
}

Label::Label() : QLabel()
{
	setFocusPolicy(Qt::NoFocus);
	setWordWrap(true);
}

InfoLabel::InfoLabel() : Label()
{
	setWordWrap(true);
	setProperty("infoField", true);
	setAlignment(Qt::AlignCenter);
}

Button::Button(	const char * textId,
		const char * textIdContext,
		const QKeySequence & seq,
		QWidget * parent) :
	QPushButton(parent),
	textId(textId),
	textIdContext(textIdContext),
	shortcut(seq, this)
{
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
	setAutoDefault(false);
	connect(&shortcut, SIGNAL(activated()), this, SLOT(click()));
}

void Button::focusInEvent(QFocusEvent * event)
{
	if(event->reason() == Qt::ActiveWindowFocusReason){
		if(focusWidget())
			focusWidget()->clearFocus();
		return;
	}
	QPushButton::focusInEvent(event);
}

void Button::narrow()
{
	setText(trShort(textId, textIdContext));
}

void Button::medium()
{
	setText(trMed(textId, textIdContext));
}

void Button::wide()
{
	setText(trLong(textId, textIdContext));
}

void Button::expanding()
{
	setText(trLong(textId, textIdContext));
}

QWidget * Button::qwidget() { return this; }


ToolButton::ToolButton(	const QIcon & icon,
		const char * textId,
		const char * textIdContext,
		const QKeySequence & seq,
		QWidget * parent) :
	QToolButton(parent),
	textId(textId),
	textIdContext(textIdContext),
	shortcut(seq, this)
{
	setIcon(icon);
	connect(&shortcut, SIGNAL(activated()), this, SLOT(click()));
}

void ToolButton::focusInEvent(QFocusEvent * event)
{
	if(event->reason() == Qt::ActiveWindowFocusReason){
		if(focusWidget())
			focusWidget()->clearFocus();
		return;
	}
	QToolButton::focusInEvent(event);
}

void ToolButton::narrow()
{
	setText(trShort(textId, textIdContext));
	setToolButtonStyle(Qt::ToolButtonIconOnly);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
}

void ToolButton::medium()
{
	setText(trMed(textId, textIdContext));
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
}

void ToolButton::wide()
{
	setText(trLong(textId, textIdContext));
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed));
}

void ToolButton::expanding()
{
	setText(trLong(textId, textIdContext));
	setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
}

QWidget * ToolButton::qwidget() { return this; }

LineEditor::LineEditor() : QLineEdit(),
	justFocusedIn(true)
{
	//setAttribute(Qt::WA_InputMethodEnabled, false);
	/*setInputMethodHints(Qt::ImhNoPredictiveText | Qt::ImhHiddenText);*/
}

void LineEditor::focusInEvent(QFocusEvent * e)
{
	QLineEdit::focusInEvent(e);
	selectAll();
	justFocusedIn = true;
}

void LineEditor::focusOutEvent(QFocusEvent * e)
{
	QLineEdit::focusOutEvent(e);
	justFocusedIn = false;
}

void LineEditor::keyPressEvent(QKeyEvent * e)
{
	QLineEdit::keyPressEvent(e);
	justFocusedIn = false;
}

void LineEditor::mousePressEvent(QMouseEvent * event)
{
	QLineEdit::mousePressEvent(event);
	if(justFocusedIn && !hasSelectedText())
		selectAll();
	justFocusedIn = false;
}

void LineEditor::inputMethodEvent(QInputMethodEvent * event)
{
	if(!event)
		return;

//	QScopedPointer<QInputMethodEvent> e(new QInputMethodEvent("", event->attributes()));
//	if(!event->commitString().isEmpty())
//		e->setCommitString(event->commitString());

	LOG("InputMethod event type: %", event->type());

	LOG("InputMethodAttribute list:");
	QList<QInputMethodEvent::Attribute> al = event->attributes();
	for(int i = 0; i < al.size(); i++){
		QInputMethodEvent::Attribute a = al.at(i);
		LOG("InputMethodAttribute: type % start % length % variant string: %",
				a.type, a.start, a.length, a.value.toString());
	}

	LOG("pre-edit string: ", event->preeditString());
	LOG("commit string: ", event->commitString());
	//QScopedPointer<QInputMethodEvent> e(new QInputMethodEvent("", event->attributes()));
	//if(event->commitString().isEmpty())
	//	e->setCommitString(event->preeditString());
	//else
	//	e->setCommitString(event->commitString());
	//QInputMethodEvent * e = event;
	
	LOG(		"Replacement start: %\n"
			"Replacement length: %\n"
			"Preedit string: %\n"
			"Commit string: %",
			event->replacementStart(),
			event->replacementLength(),
			event->preeditString(),
			event->commitString());

	//QLineEdit::inputMethodEvent(e.data());
	QLineEdit::inputMethodEvent(event);
	LOG("Entry text after QLineEdit::inputMethodEvent(): %\n", text());
}

InputEditor::InputEditor(QWidget * parent) :
	QWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	label.setFocusPolicy(Qt::NoFocus);

	setContentsMargins(0,0,0,0);
	wideLayout();
}

InputEditor::~InputEditor() {}

void InputEditor::narrowLayout()
{
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, -1, Qt::AlignBottom);
	newLayout->addWidget(&editor);
	newLayout->setSpacing(1);
	delete layout();
	setLayout(newLayout);
}

void InputEditor::wideLayout()
{
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, 1, Qt::AlignTop);
	newLayout->addWidget(&editor, 4);
	delete layout();
	setLayout(newLayout);
}

PortEditor::PortEditor(QWidget * parent) :
	QWidget(parent)
{
	setFocusPolicy(Qt::NoFocus);
	label.setFocusPolicy(Qt::NoFocus);
	QString regx("[0-9]{0,5}");
	validator.setRegExp(QRegExp(regx));
	editor.setValidator(&validator);
	editor.setInputMethodHints(Qt::ImhFormattedNumbersOnly);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	connect(&editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(textChangedSlot(const QString &)));

	setContentsMargins(0,0,0,0);
	wideLayout();
}

void PortEditor::textChangedSlot(const QString & newStr)
{
	int pos = editor.cursorPosition();
	QString str(newStr);
	if(validator.validate(str, pos) != QValidator::Acceptable){
		DBG("Invalid text: ", str);
		return;
	}
	DBG("Value: ", value());

	valueChanged(value());
	valueChanged(editor.text());
	return;
}

void PortEditor::setValue(unsigned v)
{
	QString vStr(QString::number(v));
	editor.blockSignals(true);
	editor.setText(vStr);
	editor.blockSignals(false);
	editor.setCursorPosition(vStr.length());
}

unsigned PortEditor::value()
{
	return editor.text().QString::toUInt();
}

void PortEditor::narrowLayout()
{
	delete layout();
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, -1, Qt::AlignBottom);
	newLayout->addWidget(&editor);
	newLayout->setSpacing(1);
	setLayout(newLayout);
}

void PortEditor::wideLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, 1, Qt::AlignTop);
	newLayout->addWidget(&editor, 4);
	setLayout(newLayout);
}

DoubleEditor::DoubleEditor(QWidget * parent) :
	QWidget(parent),
	precision(0)
{
	setFocusPolicy(Qt::NoFocus);
	label.setFocusPolicy(Qt::NoFocus);
	resetRegexp();
	editor.setValidator(&validator);
	editor.setInputMethodHints(Qt::ImhFormattedNumbersOnly);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
	connect(&editor, SIGNAL(textChanged(const QString &)),
			this, SLOT(textChangedSlot(const QString &)));

	setContentsMargins(0,0,0,0);
	wideLayout();
}

void DoubleEditor::resetRegexp()
{
	/*
	QString rx("[0-9]{0,3}(%1[0-9]{0,3})*(%2[0-9]{0,%3})?(%4)?");
	QString regx(rx.arg(
			QRegExp::escape(Config::locale.groupSeparator()),
			QRegExp::escape(Config::locale.decimalPoint()),
			QRegExp::escape(QString::number(precision)),
			QRegExp::escape(suffix)));
			*/
	QString rx("\\d*[%1%2]?\\d{0,%3}(%4)?");
	QString regx(rx.arg(
			QRegExp::escape(Config::locale.groupSeparator()),
			QRegExp::escape(Config::locale.decimalPoint()),
			QRegExp::escape(QString::number(precision)),
			QRegExp::escape(suffix)));
	DBG("Validator regx: ", regx);
	validator.setRegExp(QRegExp(regx));
}

void DoubleEditor::textChangedSlot(const QString & newText)
{
	int pos = editor.cursorPosition();
	QString str = newText;
	if(validator.validate(str, pos) != QValidator::Acceptable){
		DBG("Invalid text: ", str);
		return;
	}
	DBG("Value: ", value());

	if((suffix.size() && str.endsWith(suffix)) || !suffix.size()){
		valueChanged(value());
		valueChanged(editor.text());
		return;
	}

	pos = editor.cursorPosition();
	editor.blockSignals(true);
	editor.setText(str + suffix);
	editor.blockSignals(false);
	editor.setCursorPosition(pos);
	valueChanged(value());
	valueChanged(editor.text());
}

void DoubleEditor::setPrecision(unsigned prec)
{
	double val = value();
	precision = prec;
	resetRegexp();
	setValue(val);
}

void DoubleEditor::setSuffix(const QString & newSuffix)
{
	double val = value();
	if(newSuffix.length())
		suffix = " " + newSuffix;
	else
		suffix = "";
	resetRegexp();
	setValue(val);
}

const QString DoubleEditor::getSuffix()
{
	return suffix.trimmed();
}

void DoubleEditor::setValue(double v)
{
	//QString vStr(Config::locale.toString(v, 'f', precision));
	QString vStr(QString::number(v, 'f', precision));
	editor.blockSignals(true);
	editor.setText(vStr + suffix);
	editor.blockSignals(false);
	editor.setCursorPosition(vStr.length());
}

double DoubleEditor::value()
{
	QString str = editor.text();
	if(str.endsWith(suffix))
		str.chop(suffix.length());
	//double v = Config::locale.toDouble(str);
	double v = str.QString::toDouble();
	return v;
}

void DoubleEditor::narrowLayout()
{
	delete layout();
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, -1, Qt::AlignBottom);
	newLayout->addWidget(&editor);
	newLayout->setSpacing(1);
	setLayout(newLayout);
}

void DoubleEditor::wideLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, 1, Qt::AlignTop);
	newLayout->addWidget(&editor, 4);
	setLayout(newLayout);
}

QuantityEditor::QuantityEditor(QWidget * parent) :
	DoubleEditor(parent)
{
	setPrecision(3);
}

PriceEditor::PriceEditor(QWidget * parent) :
	DoubleEditor(parent)
{
	setPrecision(2);
}

FormCheckBox::FormCheckBox(QWidget * parent) :
	QWidget(parent)
{
	label.setFocusPolicy(Qt::NoFocus);
	setContentsMargins(0,0,0,0);

	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, -1, Qt::AlignVCenter);
	newLayout->addWidget(&box, -1, Qt::AlignLeft);
	setLayout(newLayout);
}

ComboBox::ComboBox(QWidget * parent) :
	QComboBox(parent)
{
}

QSize ComboBox::sizeHint() const
{
	QSize hint = QComboBox::sizeHint();
	return hint;
}

QSize ComboBox::minimumSizeHint() const
{
	QSize hint = QComboBox::minimumSizeHint();
	return hint;
}

Selector::Selector(QAbstractItemModel * model, int column, QWidget * parent) :
	QWidget(parent),
	tableViewPtr(new TableView),
	boxLayout(0),
	tableView(*tableViewPtr)
{
	(void)column;
	(void)model;
	label.setFocusPolicy(Qt::NoFocus);
	if(model){
		box.setModel(model);
		box.setModelColumn(column);
	}
	box.setView(tableViewPtr);
	if(box.lineEdit())
		box.lineEdit()->setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
	wideLayout();
	setContentsMargins(0,0,0,0);
}

void Selector::showEvent(QShowEvent * event)
{
	(void)event;
	tableView.horizontalHeader()->resizeSection(box.modelColumn(), box.width());
}

void Selector::resizeEvent(QResizeEvent * event)
{
	if(event->size() == event->oldSize())
		return;
	tableView.horizontalHeader()->resizeSection(box.modelColumn(), box.width());
}

void Selector::narrowLayout()
{
	delete layout();
	boxLayout = new VLayout;
	boxLayout->addWidget(&label, -1, Qt::AlignBottom);
	boxLayout->addWidget(&box);
	boxLayout->setSpacing(1);
	setLayout(boxLayout);
}

void Selector::wideLayout()
{
	delete layout();
	boxLayout = new HLayout;
	boxLayout->addWidget(&label, 1, Qt::AlignTop);
	boxLayout->addWidget(&box, 4);
	setLayout(boxLayout);
}

ComboSelector::ComboSelector(QAbstractItemModel * model, int column, QWidget * parent) :
	Selector(model, column, parent),
	editorPtr(new LineEditor),
	completerTableViewPtr(new TableView),
	editor(*editorPtr),
	completerTableView(*completerTableViewPtr)
{
	box.setEditable(true);
	box.setLineEdit(&editor);
	box.setInsertPolicy(QComboBox::NoInsert);
	box.completer()->setPopup(completerTableViewPtr);
	box.setCurrentIndex(-1);
	if(!model){
		tableView.horizontalHeader()->hide();
		completerTableView.horizontalHeader()->hide();
	}
	editor.setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
	editor.setValidator(0);
}

Text ComboSelector::text() const { return editor.text(); }
void ComboSelector::setText(const QString & str) { editor.setText(str); }

void ComboSelector::keyPressEvent(QKeyEvent * event)
{
	if(event->key() == Qt::Key_Return){
		return;
	}
	Selector::keyPressEvent(event);
}

void ComboSelector::narrowLayout()
{
	box.completer()->setCompletionMode(QCompleter::InlineCompletion);
	Selector::narrowLayout();
}

void ComboSelector::wideLayout()
{
	box.completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
	Selector::wideLayout();
}

DateTimeEdit::DateTimeEdit(QWidget * parent) :
	QDateTimeEdit(parent)
{
	QAbstractSpinBox::lineEdit()->setStyleSheet(
			"QLineEdit { margin: 0px; padding: 0px; }");
	setCalendarPopup(true);
	setDisplayFormat(Config::dateTimeFormat());
	setDateTime(QDateTime::currentDateTime());
}

QSize DateTimeEdit::sizeHint() const
{
	QSize hint = QDateTimeEdit::sizeHint();
	return hint;
}

QSize DateTimeEdit::minimumSizeHint() const
{
	QSize hint = QDateTimeEdit::minimumSizeHint();
	return hint;
}

DateTimeEditor::DateTimeEditor(QWidget * parent) :
	QWidget(parent)
{
	label.setFocusPolicy(Qt::NoFocus);
	wideLayout();
	setContentsMargins(0,0,0,0);
}

void DateTimeEditor::narrowLayout()
{
	delete layout();
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, 0, Qt::AlignBottom);
	newLayout->addWidget(&edit);
	newLayout->setSpacing(1);
	setLayout(newLayout);
}

void DateTimeEditor::wideLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, 1, Qt::AlignTop);
	newLayout->addWidget(&edit, 4);
	setLayout(newLayout);
}

CommentEditor::CommentEditor(QWidget * parent) :
	QWidget(parent)
{
	label.setFocusPolicy(Qt::NoFocus);
	setContentsMargins(0,0,0,0);
	wideLayout();
}

void CommentEditor::narrowLayout()
{
	delete layout();
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, 0, Qt::AlignBottom);
	newLayout->addWidget(&edit, 1);
	newLayout->setSpacing(1);
	setLayout(newLayout);
}

void CommentEditor::wideLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, 1, Qt::AlignTop);
	newLayout->addWidget(&edit, 4);
	setLayout(newLayout);
}

ButtonGroup::ButtonGroup(QWidget * parent) :
	QWidget(parent)
{
	label.setFocusPolicy(Qt::NoFocus);
	setContentsMargins(0,0,0,0);
	wideLayout();
}

void ButtonGroup::narrowLayout()
{
	delete layout();
	VLayout * newLayout = new VLayout;
	newLayout->addWidget(&label, -1, Qt::AlignBottom);
	QList<QAbstractButton*> list = group.buttons();
	for(int i = 0; i < list.size(); i++)
		newLayout->addWidget(list[i]);
	newLayout->setSpacing(1);
	setLayout(newLayout);
}

void ButtonGroup::mediumLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, -1, Qt::AlignTop);

	VLayout * vLayout = new VLayout;
	QList<QAbstractButton*> list = group.buttons();
	for(int i = 0; i < list.size(); i++)
		vLayout->addWidget(list[i]);
	newLayout->addLayout(vLayout);

	setLayout(newLayout);
}

void ButtonGroup::wideLayout()
{
	delete layout();
	HLayout * newLayout = new HLayout;
	newLayout->addWidget(&label, -1, Qt::AlignTop);
	QList<QAbstractButton*> list = group.buttons();
	for(int i = 0; i < list.size(); i++)
		newLayout->addWidget(list[i], -1, Qt::AlignLeft);
	setLayout(newLayout);
}

ControlBar::ControlBar(QWidget * parent) :
	QScrollArea(parent),
	updatingButtons(false),
	expanding(false),
	spacerBegin(false),
	spacerEnd(true),
	hideIfEmpty(true),
	scroller(this)
{
	setFocusPolicy(Qt::NoFocus);
	setContentsMargins(0,0,0,0);
	main.setContentsMargins(0,0,0,0);
	main.setFocusPolicy(Qt::NoFocus);

	setFrameStyle(QFrame::NoFrame);
	setWidget(&main);
	setWidgetResizable(true);
	setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred));
}
ControlBar::~ControlBar() {}

void ControlBar::addToolWidget(ToolWidget & tw)
{
	buttons.add(tw);
}

void ControlBar::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	bool save = hideIfEmpty;
	hideIfEmpty = false;
	updateButtons();
	hideIfEmpty = save;
}

void ControlBar::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;

	/* Now to update the buttons. */
	updateButtons();

	/* Lets make place for horizontal scrollbar iff it is to be shown. */
	int h = main.minimumSizeHint().height();
	bool v = horizontalScrollBar()->minimum() < horizontalScrollBar()->maximum();
	horizontalScrollBar()->setVisible(v);
	if(v)
		h += horizontalScrollBar()->height();
	setMinimumHeight(0 < h ? h : 0);
}

void ControlBar::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		updateButtons();
}

void ControlBar::applyLayout()
{
	HLayout * toolLayout = new HLayout;
	toolLayout->setSpacing(0);

	if(spacerBegin)
		toolLayout->addStretch(10000);

	unsigned s = buttons.size();
	for(unsigned i = 0; i < s; i++){
		toolLayout->addWidget(buttons[i].qwidget(),
				-1, Qt::AlignVCenter | Qt::AlignLeft);
		if(expanding && i != s-1)
			toolLayout->addStretch(10000);
	}

	if(spacerEnd)
		toolLayout->addStretch(10000);

	delete main.layout();
	main.setLayout(toolLayout);
	updateButtons();
}

void ControlBar::relayout()
{
	if(!main.layout())
		applyLayout();
}

void ControlBar::updateButtons()
{
	if(updatingButtons)
		return;
	updatingButtons = true;

	unsigned showCount = 0;

	{
		unsigned s = buttons.size();
		for(unsigned i = 0; i < s; i++){
			ToolWidget & tw = buttons[i];
			tw.expanding();
			tw.qwidget()->updateGeometry();
			if(tw.qwidget()->isVisible())
				showCount++;
		}
	}
	if(width() < main.sizeHint().width()) {
		unsigned s = buttons.size();
		for(unsigned i = 0; i < s; i++)
			buttons[i].wide();
	}
	if(width() < main.sizeHint().width()) {
		unsigned s = buttons.size();
		for(unsigned i = 0; i < s; i++)
			buttons[i].medium();
	}
	if(width() < main.sizeHint().width()) {
		unsigned s = buttons.size();
		for(unsigned i = 0; i < s; i++)
			buttons[i].narrow();
	}

	if(hideIfEmpty)
		setVisible(showCount);

	if(showCount)
		update();

	updatingButtons = false;
}

SCC TidToolBarContext = "ToolBar";
SCC TidBackButtonLabel = QT_TRANSLATE_NOOP("ToolBar", "Back");

ToolBar::ToolBar(QWidget * parent) :
	QWidget(parent),
	backButton(TidBackButtonLabel, TidToolBarContext,
			QKeySequence(Qt::ALT + Qt::Key_Escape))
{
	setFocusPolicy(Qt::NoFocus);
	setContentsMargins(0,0,0,0);
	ctrlBar.hideIfEmpty = false;

	infoLabel.hide();
}
ToolBar::~ToolBar() {}

void ToolBar::showEvent(QShowEvent *event)
{
	QWidget::showEvent(event);
	updateButtons();
}

void ToolBar::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	updateButtons();
}

void ToolBar::changeEvent(QEvent * event)
{
	QWidget::changeEvent(event);
	if(event->type() != QEvent::LanguageChange)
		return;
	updateButtons();
}

void ToolBar::addToolWidget(ToolWidget & tw)
{
	ctrlBar.addToolWidget(tw);
}

void ToolBar::updateButtons()
{
	backButton.setText(tr(TidBackButtonLabel));
	ctrlBar.updateButtons();
}

void ToolBar::applyLayout()
{
	QHBoxLayout * hLayout = new QHBoxLayout;
	hLayout->setContentsMargins(0,0,0,0);
	hLayout->addWidget(&ctrlBar);
	hLayout->addWidget(&backButton);

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->setSpacing(0);
	vLayout->setContentsMargins(0,0,0,0);
	vLayout->addWidget(&infoLabel);
	vLayout->addLayout(hLayout);

	delete QWidget::layout();
	QWidget::setLayout(vLayout);
}

void ToolBar::relayout()
{
	ctrlBar.relayout();
	applyLayout();
}

void ToolBar::setInfo(const QString & text)
{
	infoLabel.setText(text);
	if(text.size())
		infoLabel.show();
	else
		infoLabel.hide();
}

void ToolBar::clearInfo()
{
	if(!infoLabel.text().size())
		return;
	infoLabel.setText("");
	infoLabel.hide();
}

