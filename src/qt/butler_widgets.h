/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_WIDGETS_H
#define BUTLER_WIDGETS_H

#include <QtGui>
#include <QString>

#include <config.h>

#include <butler_kineticscroller.h>
#include "butler_pannview.h"
#include "butler_tagwidget.h"
#include "butler_databases.h"
#include "butler_config.h"
#include "butler_texts.h"

#define PRIMITIVE_PROPERTY(type, name, reader, writer) \
	Q_PROPERTY(type name READ reader WRITE writer); \
	type name; \
	type reader() const { return name; } \
	void writer(type newValue) \
	{ \
		name = newValue; \
		/* Here to load dynamic stylesheet */ \
		style()->unpolish(this); \
		style()->polish(this); \
		update(); \
	}

class TableView : public QTableView
{
private:
	Q_OBJECT
public:
	TableView(QWidget * parent = 0) :
		QTableView(parent),
		scroller(this)
	{
		setAlternatingRowColors(true);
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
//		setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
		setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
		setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
		setAutoScroll(true);
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setSelectionMode(QAbstractItemView::SingleSelection);
		verticalHeader()->hide();
		setSortingEnabled(true);
		horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		horizontalHeader()->setMovable(true);
		horizontalHeader()->setSortIndicatorShown(true);
//		horizontalHeader()->setResizeMode(QHeaderView::Interactive);
		horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	}

	KineticScroller scroller;
};

class HLayout : public QHBoxLayout
{
private:
	Q_OBJECT
public:
	HLayout() : QHBoxLayout()
	{
		setContentsMargins(0,0,0,0);
		setSpacing(3);
	}
};

class VLayout : public QVBoxLayout
{
private:
	Q_OBJECT
public:
	VLayout() : QVBoxLayout()
	{
		setContentsMargins(0,0,0,0);
		setSpacing(3);
	}
};

class InfoLabel : public QLabel
{
private:
	Q_OBJECT
public:
	InfoLabel() : QLabel()
	{
		setFocusPolicy(Qt::NoFocus);
		setWordWrap(true);
//		setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	}
};

class Button : public QPushButton
{
private:
	Q_OBJECT
public:
	Button(const QKeySequence & seq = QKeySequence(), QWidget * parent = 0) :
		QPushButton(parent),
		shortcut(seq, this)
	{
		setAutoDefault(false);
		connect(&shortcut, SIGNAL(activated()), this, SLOT(click()));
	}

	virtual void focusInEvent(QFocusEvent * event)
	{
		if(event->reason() == Qt::ActiveWindowFocusReason){
			if(focusWidget())
				focusWidget()->clearFocus();
			return;
		}
		QPushButton::focusInEvent(event);
	}
public:
	QShortcut shortcut;
};

class ToolButton : public QToolButton
{
private:
	Q_OBJECT
public:
	ToolButton(	const QIcon & icon,
			const QKeySequence & seq = QKeySequence(),
			QWidget * parent = 0) :
		QToolButton(parent),
		shortcut(seq, this)
	{
		setIcon(icon);
		connect(&shortcut, SIGNAL(activated()), this, SLOT(click()));
	}

	virtual void focusInEvent(QFocusEvent * event)
	{
		if(event->reason() == Qt::ActiveWindowFocusReason){
			if(focusWidget())
				focusWidget()->clearFocus();
			return;
		}
		QToolButton::focusInEvent(event);
	}
public:
	void narrowLayout()
	{
		setToolButtonStyle(Qt::ToolButtonIconOnly);
	}

	void mediumLayout()
	{
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
	}

	void wideLayout()
	{
		setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
	}

	QShortcut shortcut;
};

class DoubleEditor : public QWidget
{
private:
	Q_OBJECT
public:
	DoubleEditor(QWidget * parent = 0) :
		QWidget(parent),
		precision(0)
	{
		setFocusPolicy(Qt::NoFocus);
		label.setFocusPolicy(Qt::NoFocus);
		resetRegexp();
		editor.setValidator(&validator);
		connect(&editor, SIGNAL(textChanged(const QString &)),
				this, SLOT(textChangedSlot(const QString &)));

		setContentsMargins(0,0,0,0);
		wideLayout();
	}
private:
	void resetRegexp()
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
		DBG("Validator regx: %s", C_STR(regx));
		validator.setRegExp(QRegExp(regx));
	}

signals:
	void valueChanged(double);
	void valueChanged(const QString &);

public slots:
	void textChangedSlot(const QString & newText)
	{
		int pos = editor.cursorPosition();
		QString str = newText;
		if(validator.validate(str, pos) != QValidator::Acceptable){
			DBG("Invalid text: %s", C_STR(str));
			return;
		}
		DBG("Value: %f", value());

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

	void setPrecision(unsigned prec = 0)
	{
		double val = value();
		precision = prec;
		resetRegexp();
		setValue(val);
	}

	void setSuffix(const QString & newSuffix = "")
	{
		double val = value();
		if(newSuffix.length())
			suffix = " " + newSuffix;
		else
			suffix = "";
		resetRegexp();
		setValue(val);
	}

	void setValue(double v)
	{
		//QString vStr(Config::locale.toString(v, 'f', precision));
		QString vStr(QString::number(v, 'f', precision));
		editor.blockSignals(true);
		editor.setText(vStr + suffix);
		editor.blockSignals(false);
		editor.setCursorPosition(vStr.length());
	}

public:
	double value()
	{
		QString str = editor.text();
		if(str.endsWith(suffix))
			str.chop(suffix.length());
		//double v = Config::locale.toDouble(str);
		double v = str.QString::toDouble();
		return v;
	}

	void narrowLayout()
	{
		delete layout();
		VLayout * newLayout = new VLayout;
		newLayout->addWidget(&label, 0, Qt::AlignBottom);
		newLayout->addWidget(&editor);
		newLayout->setSpacing(1);
		setLayout(newLayout);
	}

	void wideLayout()
	{
		delete layout();
		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignTop);
		newLayout->addWidget(&editor);
		newLayout->setStretch(1, 0);
		setLayout(newLayout);
	}
public:
	QLabel label;
	QLineEdit editor;
private:
	QString suffix;
	int precision;
	QRegExpValidator validator;
};

class QuantityEditor : public DoubleEditor
{
private:
	Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0) :
		DoubleEditor(parent)
	{
		setPrecision(3);
	}
};

class PriceEditor : public DoubleEditor
{
private:
	Q_OBJECT
public:
	PriceEditor(QWidget * parent = 0) :
		DoubleEditor(parent)
	{
		setPrecision(2);
	}
};

class FormCheckBox : public QWidget
{
private:
	Q_OBJECT
public:
	FormCheckBox(QWidget * parent = 0) :
		QWidget(parent)
	{
		label.setFocusPolicy(Qt::NoFocus);
		setContentsMargins(0,0,0,0);

		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignVCenter);
		newLayout->addWidget(&box, 0, Qt::AlignLeft);
		setLayout(newLayout);
	}

	QLabel label;
	QCheckBox box;
};

class ComboBox : public QComboBox
{
private:
	Q_OBJECT
public:
	ComboBox(QWidget * parent = 0) :
		QComboBox(parent)
	{
	}

	virtual QSize sizeHint() const
	{
		QSize hint = QComboBox::sizeHint();
		hint.setWidth(220);
		return hint;
	}

	virtual QSize minimumSizeHint() const
	{
		QSize hint = QComboBox::minimumSizeHint();
		hint.setWidth(220);
		return hint;
	}
};

class Selector : public QWidget
{
private:
	Q_OBJECT
public:
	Selector(QAbstractItemModel * model, int column, QWidget * parent = 0) :
		QWidget(parent)
	{
		label.setFocusPolicy(Qt::NoFocus);
		if(model){
			box.setModel(model);
			box.setModelColumn(column);
		}
		box.setView(&tableView);
		box.setSizeAdjustPolicy(QComboBox::AdjustToContents);
		//box.setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		if(box.lineEdit())
			box.lineEdit()->setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		wideLayout();
		setContentsMargins(0,0,0,0);
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		if(event->size() == event->oldSize())
			return;
		tableView.horizontalHeader()->resizeSection(box.modelColumn(), box.width());
	}

	void narrowLayout()
	{
		delete layout();
		VLayout * newLayout = new VLayout;
		newLayout->addWidget(&label, 0, Qt::AlignBottom);
		newLayout->addWidget(&box);
		newLayout->setSpacing(1);
		setLayout(newLayout);
	}

	void wideLayout()
	{
		delete layout();
		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignTop);
		newLayout->addWidget(&box, 0);
		setLayout(newLayout);
	}

	QLabel label;
	ComboBox box;
	TableView tableView;
};

class ComboSelector : public Selector
{
private:
	Q_OBJECT
public:
	ComboSelector(QAbstractItemModel * model = 0, int column = 0, QWidget * parent = 0) :
		Selector(model, column, parent)
	{
		box.setEditable(true);
		box.setLineEdit(&editor);
		box.setInsertPolicy(QComboBox::NoInsert);
		box.completer()->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
//		box.completer()->setCompletionMode(QCompleter::InlineCompletion);
		box.completer()->setPopup(&completerTableView);
		box.setCurrentIndex(-1);
		if(!model){
			tableView.horizontalHeader()->hide();
			completerTableView.horizontalHeader()->hide();
		}
		editor.setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		editor.setValidator(0);
	}

	QString text() const { return editor.text(); }
	void setText(const QString & str) { editor.setText(str); }

	virtual void keyPressEvent(QKeyEvent * event)
	{
		if(event->key() == Qt::Key_Return){
			return;
		}
		Selector::keyPressEvent(event);
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		if(event->size() == event->oldSize())
			return;
		completerTableView.horizontalHeader()->resizeSection(box.modelColumn(), box.width());
		Selector::resizeEvent(event);
	}

	QLineEdit editor;
	TableView completerTableView;
};

class DateTimeEdit : public QDateTimeEdit
{
private:
	Q_OBJECT
public:
	DateTimeEdit(QWidget * parent = 0) :
		QDateTimeEdit(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
		setCalendarPopup(true);
		setDisplayFormat(Config::dateTimeFormat());
		setDateTime(QDateTime::currentDateTime());
	}
};

class DateTimeEditor : public QWidget
{
private:
	Q_OBJECT
public:
	DateTimeEditor(QWidget * parent = 0) :
		QWidget(parent)
	{
		label.setFocusPolicy(Qt::NoFocus);
		wideLayout();
		setContentsMargins(0,0,0,0);
	}

	void narrowLayout()
	{
		delete layout();
		VLayout * newLayout = new VLayout;
		newLayout->addWidget(&label, 0, Qt::AlignBottom);
		newLayout->addWidget(&edit);
		newLayout->setSpacing(1);
		setLayout(newLayout);
	}

	void wideLayout()
	{
		delete layout();
		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignTop);
		newLayout->addWidget(&edit);
		newLayout->setStretch(1, 0);
		setLayout(newLayout);
	}

	DateTimeEdit edit;
	QLabel label;
};

class CommentEditor : public QWidget
{
private:
	Q_OBJECT
public:
	CommentEditor(QWidget * parent = 0) :
		QWidget(parent)
	{
		label.setFocusPolicy(Qt::NoFocus);
		setContentsMargins(0,0,0,0);
	}

	void narrowLayout()
	{
		delete layout();
		VLayout * newLayout = new VLayout;
		newLayout->addWidget(&label, 0, Qt::AlignBottom);
		newLayout->addWidget(&edit, 1);
		newLayout->setSpacing(1);
		setLayout(newLayout);
	}

	void wideLayout()
	{
		delete layout();
		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignTop);
		newLayout->addWidget(&edit, 1);
		setLayout(newLayout);
	}

	QTextEdit edit;
	QLabel label;
};

class ToolBar : public QWidget
{
private:
	Q_OBJECT
public:
	ToolBar(PannView * parent) :
		QWidget(parent),
		backShortcut(QKeySequence(Qt::ALT + Qt::Key_Escape), &backButton),
		scrollArea(0),
		scroller(&scrollArea)
	{
		setFocusPolicy(Qt::NoFocus);
		main.setFocusPolicy(Qt::NoFocus);
		scrollArea.setFocusPolicy(Qt::NoFocus);
		prev.setFocusPolicy(Qt::NoFocus);
		next.setFocusPolicy(Qt::NoFocus);
		setContentsMargins(0,0,0,0);
		prev.setText("<");
		next.setText(">");

		connect(&backShortcut, SIGNAL(activated()), &backButton, SLOT(click()));
		connect(&backButton, SIGNAL(clicked()), parent, SLOT(reject()));

		scrollArea.setFrameStyle(QFrame::NoFrame);
		scrollArea.setWidget(&main);
		scrollArea.setWidgetResizable(true);
		scrollArea.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		scrollArea.setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
//		setSizePolicy(QSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Fixed));

		QHBoxLayout * hLayout = new QHBoxLayout;
		hLayout->setContentsMargins(0,0,0,0);
		hLayout->addWidget(&prev);
		hLayout->addWidget(&scrollArea);
		hLayout->addWidget(&next);
		hLayout->addWidget(&backButton);
		QWidget::setLayout(hLayout);

		connect(&prev, SIGNAL(clicked()), this, SLOT(prevClicked()));
		connect(&next, SIGNAL(clicked()), this, SLOT(nextClicked()));
		connect(scrollArea.horizontalScrollBar(), SIGNAL(valueChanged(int)),
				this, SLOT(scrollValueChanged(int)));

		retranslate();
	}
	virtual ~ToolBar() {}

	virtual QLayout * layout() const
	{
		return main.layout();
	}

	virtual void setLayout(QLayout * layout)
	{
		delete main.layout();
		DBG("ToolBar::setLayout() 1 scrollarea.sizeHint(): %d",
				scrollArea.sizeHint().width());
		main.setLayout(layout);
		relayout();
		DBG("ToolBar::setLayout() 2 scrollarea.sizeHint(): %d",
				scrollArea.sizeHint().width());
	}

	virtual QSize sizeHint() const
	{
		QSize hint = main.sizeHint();
		hint.setWidth(hint.width() + backButton.sizeHint().width());
		return hint;
	}

	void retranslate()
	{
		backButton.setText(qtTrId(TidBackButtonLabel));

		if(main.layout())
			relayout();
	}

	void relayout()
	{
		prev.hide();
		next.hide();

		DBG("ToolBar::relayout() main-hint-width: %d, backbutton-hint-width: %d, width: %d"
				, main.sizeHint().width(), backButton.sizeHint().width(), width());
		if(main.sizeHint().width() + backButton.sizeHint().width() <= width())
			return;

		scrollValueChanged(scrollArea.horizontalScrollBar()->value());
	}

	virtual void changeEvent(QEvent * event)
	{
		QWidget::changeEvent(event);
		if(event->type() == QEvent::LanguageChange)
			retranslate();
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		if(layout() && (event->size() == event->oldSize()))
			return;
		DBG("ToolBar::resizeEvent event->size(): %d", event->size().width());
		relayout();
	}

private slots:
	void prevClicked()
	{
		QScrollBar * bar = scrollArea.horizontalScrollBar();
		bar->setValue(bar->value() - bar->singleStep());
	}

	void nextClicked()
	{
		QScrollBar * bar = scrollArea.horizontalScrollBar();
		bar->setValue(bar->value() + bar->singleStep());
	}

	void scrollValueChanged(int newValue)
	{
		QScrollBar * bar = scrollArea.horizontalScrollBar();

		DBG("min: %d, max: %d, value: %d, newValue: %d",
				bar->minimum(), bar->maximum(), bar->value(), newValue);

		if(bar->maximum() <= newValue)
			next.hide();
		else
			next.show();
		if(newValue <= bar->minimum())
			prev.hide();
		else
			prev.show();
	}

public:
	Button backButton;

private:
	QShortcut backShortcut;
	QScrollArea scrollArea;
	QWidget main;

	KineticScroller scroller;
	QToolButton prev;
	QToolButton next;
};

#endif
