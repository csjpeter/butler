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
	}
};

class LabelledDoubleEditor : public QWidget
{
private:
	Q_OBJECT

	enum class ViewState {
		Narrow,
		Wide
	};
public:
	LabelledDoubleEditor(const QString & text, QWidget * parent = 0) :
		QWidget(parent),
		label(text),
		state(ViewState::Narrow)
	{
		spin.setSizePolicy(QSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum));
	}

	virtual QSize sizeHint() const
	{
		return prefSize;
	}

	virtual QSize minimumSizeHint() const
	{
		return minSize;
	}

	void updateGeometry()
	{
		QHBoxLayout layout;
		layout.addWidget(&label);
		layout.addWidget(&spin);
		prefSize = layout.sizeHint();

		QVBoxLayout vlayout;
		vlayout.addWidget(&label);
		vlayout.addWidget(&spin);
		minSize = vlayout.sizeHint();

		QWidget::updateGeometry();
	LOG("UpdateGeom: "
		"min hint width: %i, min hint height: %i, "
		"hint width: %i, hint height: %i, "
		"width: %i, height: %i",
		minimumSizeHint().width(), minimumSizeHint().height(),
		sizeHint().width(), sizeHint().height(),
		width(), height());
	}

	virtual void resizeEvent(QResizeEvent * event)
	{
	LOG(	"min hint width: %i, min hint height: %i, "
		"hint width: %i, hint height: %i, "
		"width: %i, height: %i",
		minimumSizeHint().width(), minimumSizeHint().height(),
		sizeHint().width(), sizeHint().height(),
		width(), height());
		if(layout() && (event->size() == event->oldSize() || !isVisible()))
			return;
		ViewState newState = ViewState::Narrow;
		if(prefSize.width() <= event->size().width())
			newState = ViewState::Wide;
		if(state == newState)
			return;
		state = newState;
		QLayout * oldLayout = layout();
		if(oldLayout){
			QLayoutItem * child;
			while((child = oldLayout->takeAt(0)) != 0)
				delete child;
			delete oldLayout;
		}
		QBoxLayout * layout = 0;
		if(state == ViewState::Narrow)
			layout = new QHBoxLayout;
		if(state == ViewState::Wide)
			layout = new QHBoxLayout;

		layout->addWidget(&label);
		layout->addWidget(&spin);
		setLayout(layout);
	}

	DoubleEditor spin;
	QLabel label;
private:
	QSize prefSize;
	QSize minSize;
	ViewState state;
};

class QuantityEditor : public LabelledDoubleEditor
{
private:
	Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0) :
		LabelledDoubleEditor(tr("Quantity:"), parent)
	{
		spin.setRange(0, INT_MAX);
		spin.setDecimals(3);
		updateGeometry();
	}
};

class PriceEditor : public LabelledDoubleEditor
{
private:
	Q_OBJECT
public:
	PriceEditor(const QString & text, QWidget * parent = 0) :
		LabelledDoubleEditor(text, parent)
	{
		spin.setRange(0, INT_MAX);
		spin.setDecimals(2);
		updateGeometry();
	}
};

class UnitPriceEditor : public PriceEditor
{
private:
	Q_OBJECT
public:
	UnitPriceEditor(QWidget * parent = 0) :
		PriceEditor(tr("Unit price:"), parent)
	{
		updateGeometry();
	}
};

class GrossPriceEditor : public PriceEditor
{
private:
	Q_OBJECT
public:
	GrossPriceEditor(QWidget * parent = 0) :
		PriceEditor(tr("Gross price:"), parent)
	{
		updateGeometry();
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
	ToolButton(const QString & text, const QIcon & icon, QWidget * parent = 0) :
		QToolButton(parent)
	{
		setText(text);
		setIcon(icon);
	}

	ToolButton * portrait()
	{
		setToolButtonStyle(Qt::ToolButtonIconOnly);
		//setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		return this;
	}

	ToolButton * landscape()
	{
		//setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
		setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
		return this;
	}
};

#endif
