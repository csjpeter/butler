/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WIDGETS_H
#define BUTLER_WIDGETS_H

#include <QtGui>
#include <QString>

#include <config.h>

#include "QsKineticScroller.h"
#include "butler_pannable.h"
#include "butler_pannview.h"
#include "butler_tagwidget.h"
#include "butler_databases.h"
#include "butler_config.h"

#define EDITOR_LAYOUTS \
	QVBoxLayout * portrait() \
	{ \
		QVBoxLayout * layout = new QVBoxLayout; \
		layout->addWidget(&label, 0, Qt::AlignBottom); \
		layout->addWidget(this); \
		layout->setSpacing(1); \
		return layout; \
	} \
	QHBoxLayout * landscape() \
	{ \
		QHBoxLayout * layout = new QHBoxLayout; \
		layout->addWidget(&label, 0, Qt::AlignTop); \
		layout->addWidget(this); \
		layout->setStretch(1, 0); \
		return layout; \
	}

class TableView : public QTableView
{
private:
	Q_OBJECT
public:
	TableView(QWidget * parent = 0) :
		QTableView(parent)
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

	void enableKineticScroll() { scroll.enableKineticScrollFor(this); }

	QsKineticScroller scroll;
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
		setSizePolicy(QSizePolicy(QSizePolicy::Ignored, QSizePolicy::Preferred));
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class QuantityEditor : public DoubleEditor
{
private:
	Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0) :
		DoubleEditor(parent)
	{
		label.setText(tr("Quantity:"));
		setRange(0, INT_MAX);
		setDecimals(3);
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
		setRange(0, INT_MAX);
		setDecimals(2);
	}
};

class UnitPriceEditor : public PriceEditor
{
private:
	Q_OBJECT
public:
	UnitPriceEditor(QWidget * parent = 0) :
		PriceEditor(parent)
	{
		label.setText(tr("Unit price:"));
	}
};

class GrossPriceEditor : public PriceEditor
{
private:
	Q_OBJECT
public:
	GrossPriceEditor(QWidget * parent = 0) :
		PriceEditor(parent)
	{
		label.setText(tr("Gross price:"));
	}
};

class Selector : public QComboBox
{
private:
	Q_OBJECT
public:
	Selector(QAbstractItemModel * model, int column, QWidget * parent = 0) :
		QComboBox(parent)
	{
		setModel(model);
		setModelColumn(column);
		setView(&tableView);
		//setSizeAdjustPolicy(QComboBox::AdjustToContents);
		setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
		if(lineEdit())
			lineEdit()->setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		QComboBox::resizeEvent(event);
		if(event->size() == event->oldSize())
			return;
		if(tableView.isVisible())
			tableView.horizontalHeader()->resizeSection(
					modelColumn(), event->size().width());
	}

	void enableKineticScroll() {
		tableView.enableKineticScroll();
	}

	TableView tableView;
};

class ComboSelector : public Selector
{
private:
	Q_OBJECT
public:
	ComboSelector(QAbstractItemModel * model, int column, QWidget * parent = 0) :
		Selector(model, column, parent)
	{
		setEditable(true);
		setLineEdit(&lineEdit);
		lineEdit.setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		completer()->setCompletionMode(QCompleter::PopupCompletion);
		completer()->setPopup(&completerTableView);
	}

	QString text() const { return lineEdit.text(); }
	void setText(const QString & str) { lineEdit.setText(str); }

	void enableKineticScroll() {
		tableView.enableKineticScroll();
		completerTableView.enableKineticScroll();
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
		QComboBox::resizeEvent(event);
		if(event->size() == event->oldSize())
			return;
		if(completerTableView.isVisible())
			completerTableView.horizontalHeader()->resizeSection(
					modelColumn(), event->size().width());
	}

	QLineEdit lineEdit;
	TableView completerTableView;
};

class ShopSelector : public Selector
{
private:
	Q_OBJECT
public:
	ShopSelector(const QString & dbname, QWidget * parent = 0) :
		Selector(&shopsModel(dbname), Shop::Name, parent),
		label(tr("Shop (place of buy):"))
	{
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class WareEditor : public ComboSelector
{
private:
	Q_OBJECT
public:
	WareEditor(const QString & dbname, QWidget * parent = 0) :
		ComboSelector(&waresModel(dbname), Ware::Name, parent),
		label(tr("Common ware name:"))
	{
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class CategoryEditor : public QComboBox
{
private:
	Q_OBJECT
public:
	CategoryEditor(QWidget * parent = 0) :
		QComboBox(parent),
		label(tr("Ware (sub)category:"))
	{
		setEditable(true);
		setLineEdit(&lineEdit);
		completer()->setCompletionMode(QCompleter::PopupCompletion);
		lineEdit.setStyleSheet("QLineEdit { margin: 0px; padding: 0px; }");
		setView(&tableView);
	}

	QString text() const { return lineEdit.text(); }
	void setText(const QString & str) { lineEdit.setText(str); }

	EDITOR_LAYOUTS

	QLineEdit lineEdit;
	QLabel label;
	TableView tableView;
};

class DateTimeEditor : public QDateTimeEdit
{
private:
	Q_OBJECT
public:
	DateTimeEditor(QWidget * parent = 0) :
		QDateTimeEdit(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
		setCalendarPopup(true);
		setDisplayFormat(Config::dateTimeFormat());
		setDateTime(QDateTime::currentDateTime());
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class PurchaseDateTimeEditor : public DateTimeEditor
{
private:
	Q_OBJECT
public:
	PurchaseDateTimeEditor(QWidget * parent = 0) :
		DateTimeEditor(parent)
	{
		label.setText(tr("Date of purchase:"));
	}
};

class UploadDateTimeEditor : public DateTimeEditor
{
private:
	Q_OBJECT
public:
	UploadDateTimeEditor(QWidget * parent = 0) :
		DateTimeEditor(parent)
	{
		setEnabled(false);
		label.setText(tr("Date of upload:"));
	}
};

class CommentEditor : public QTextEdit
{
private:
	Q_OBJECT
public:
	CommentEditor(QWidget * parent = 0) :
		QTextEdit(parent),
		label(tr("Comments:"))
	{
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class DoneButton : public QPushButton
{
private:
	Q_OBJECT
public:
	DoneButton(QWidget * parent = 0) :
		QPushButton(tr("Done"), parent)
	{
	}
};

class OnStockCheckBox : public QCheckBox
{
private:
	Q_OBJECT
public:
	OnStockCheckBox(QWidget * parent = 0) :
		QCheckBox(tr("on stock"), parent)
	{
	}
};

class BoughtCheckBox : public QCheckBox
{
private:
	Q_OBJECT
public:
	BoughtCheckBox(QWidget * parent = 0) :
		QCheckBox(tr("bought"), parent)
	{
	}
};

class ToolButton : public QToolButton
{
private:
	Q_OBJECT
public:
	ToolButton(QWidget * parent = 0) :
		QToolButton(parent)
	{
	}

	ToolButton * portrait()
	{
//		setToolButtonStyle(Qt::ToolButtonIconOnly);
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		return this;
	}

	ToolButton * landscape()
	{
		setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		return this;
	}
};

#define TOOL_BUTTON(class_name, image_name) \
	class class_name : public ToolButton \
	{ \
	private: \
		Q_OBJECT \
	public: \
		class_name(const QString & text, QWidget * parent = 0) : \
			ToolButton(parent) \
		{ \
			setText(text); \
			setIcon(QIcon(Path::icon(image_name))); \
		} \
		virtual ~class_name() {} \
	}

TOOL_BUTTON(AddToolButton, "add.png");
TOOL_BUTTON(EditToolButton, "edit.png");
TOOL_BUTTON(DelToolButton, "delete.png");
TOOL_BUTTON(DropToolButton, "trash.png");
TOOL_BUTTON(FilterToolButton, "filter.png");
//TOOL_BUTTON(WareEditToolButton, "ware.png");
TOOL_BUTTON(BuyToolButton, "buy.png");
TOOL_BUTTON(TagToolButton, "tag.png");

#endif
