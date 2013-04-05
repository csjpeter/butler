/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
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
		setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
		setAlternatingRowColors(true);
		setAutoScroll(false);
		setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
		setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
		setSelectionBehavior(QAbstractItemView::SelectRows);
		setSelectionMode(QAbstractItemView::SingleSelection);
		verticalHeader()->hide();
		setSortingEnabled(true);
		horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
		horizontalHeader()->setMovable(true);
		horizontalHeader()->setSortIndicatorShown(true);
		horizontalHeader()->setResizeMode(QHeaderView::Interactive);
		horizontalHeader()->setSortIndicator(0, Qt::AscendingOrder);
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		QTableView::resizeEvent(event);
		if(event->size() == event->oldSize())
			return;
		if(isVisible())
			resizeColumnsToContents();
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
		setWordWrap(true);
//		setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
	}
};

class DoubleEditor : public QDoubleSpinBox
{
private:
	Q_OBJECT
public:
	DoubleEditor(QWidget * parent = 0) :
		QDoubleSpinBox(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
		setButtonSymbols(QAbstractSpinBox::NoButtons);
		setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred));
	}

	PRIMITIVE_PROPERTY(bool, mandatory, isMandatory, setMandatory);
};

class LabelledDoubleEditor : public QWidget
{
private:
	Q_OBJECT
public:
	LabelledDoubleEditor(QWidget * parent = 0) :
		QWidget(parent)
	{
		wideLayout();
		setContentsMargins(0,0,0,0);
	}

	void narrowLayout()
	{
		delete layout();
		VLayout * newLayout = new VLayout;
		newLayout->addWidget(&label, 0, Qt::AlignBottom);
		newLayout->addWidget(&spin);
		newLayout->setSpacing(1);
		setLayout(newLayout);
	}

	void wideLayout()
	{
		delete layout();
		HLayout * newLayout = new HLayout;
		newLayout->addWidget(&label, 0, Qt::AlignTop);
		newLayout->addWidget(&spin);
		newLayout->setStretch(1, 0);
		setLayout(newLayout);
	}

	DoubleEditor spin;
	QLabel label;
};

class QuantityEditor : public LabelledDoubleEditor
{
private:
	Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0) :
		LabelledDoubleEditor(parent)
	{
		spin.setRange(0, INT_MAX);
		spin.setDecimals(3);
	}
};

class PriceEditor : public LabelledDoubleEditor
{
private:
	Q_OBJECT
public:
	PriceEditor(QWidget * parent = 0) :
		LabelledDoubleEditor(parent)
	{
		spin.setRange(0, INT_MAX);
		spin.setDecimals(2);
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
		if(model){
			box.setModel(model);
			box.setModelColumn(column);
		}
		box.setView(&tableView);
		//box.setSizeAdjustPolicy(QComboBox::AdjustToContents);
		box.setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		if(box.lineEdit())
			box.lineEdit()->setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		wideLayout();
		setContentsMargins(0,0,0,0);
	}

	void paintEvent(QPaintEvent *)
	{
		QStyleOption opt;
		opt.init(this);
		QPainter p(this);
		style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		if(event->size() == event->oldSize())
			return;
		if(tableView.isVisible())
			tableView.horizontalHeader()->resizeSection(
					box.modelColumn(), event->size().width());
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
		newLayout->addWidget(&box);
		newLayout->setStretch(1, 0);
		setLayout(newLayout);
	}

	PRIMITIVE_PROPERTY(bool, mandatory, isMandatory, setMandatory);

	QLabel label;
	QComboBox box;
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
		box.setLineEdit(&lineEdit);
		lineEdit.setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		box.completer()->setCompletionMode(QCompleter::PopupCompletion);
		box.completer()->setPopup(&completerTableView);
		if(!model){
			tableView.horizontalHeader()->hide();
			completerTableView.horizontalHeader()->hide();
		}
	}

	QString text() const { return lineEdit.text(); }
	void setText(const QString & str) { lineEdit.setText(str); }

	virtual void resizeEvent(QResizeEvent * event)
	{
		if(event->size() == event->oldSize())
			return;
		if(completerTableView.isVisible())
			completerTableView.horizontalHeader()->resizeSection(
					box.modelColumn(), event->size().width());
		Selector::resizeEvent(event);
	}

	QLineEdit lineEdit;
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

	QTextEdit edit;
	QLabel label;
};

class Button : public QPushButton
{
private:
	Q_OBJECT
public:
	Button(QWidget * parent = 0) :
		QPushButton(parent)
	{
	}
};

class ToolButton : public QToolButton
{
private:
	Q_OBJECT
public:
	ToolButton(const QIcon & icon, QWidget * parent = 0) :
		QToolButton(parent)
	{
		setIcon(icon);
	}

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
};

#endif
