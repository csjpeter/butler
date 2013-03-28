/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WIDGETS_H
#define BUTLER_WIDGETS_H

#include <QtGui>
#include <QString>

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
		layout->addWidget(&label); \
		layout->addWidget(this); \
		return layout; \
	} \
	QHBoxLayout * landscape() \
	{ \
		QHBoxLayout * layout = new QHBoxLayout; \
		layout->addWidget(&label); \
		layout->addWidget(this); \
		return layout; \
	}

class QuantityEditor : public QDoubleSpinBox
{
private:
	Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0) :
		QDoubleSpinBox(parent),
		label(tr("Quantity:"))
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
		setRange(0, INT_MAX);
		setDecimals(3);
		setFrame(false);
		setButtonSymbols(QAbstractSpinBox::NoButtons);
	}

	EDITOR_LAYOUTS

	QLabel label;
};

class PriceEditor : public QDoubleSpinBox
{
private:
	Q_OBJECT
public:
	PriceEditor(QWidget * parent = 0) :
		QDoubleSpinBox(parent)
	{
		QAbstractSpinBox::lineEdit()->setStyleSheet(
				"QLineEdit { margin: 0px; padding: 0px; }");
		setRange(0, INT_MAX);
		setDecimals(2);
		setFrame(false);
		setButtonSymbols(QAbstractSpinBox::NoButtons);
	}

	EDITOR_LAYOUTS

	QLabel label;
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

class WareEditor : public QComboBox
{
private:
	Q_OBJECT
public:
	WareEditor(const QString & dbname, QWidget * parent = 0) :
		QComboBox(parent),
		label(tr("Common ware name:"))
	{
		setEditable(true);
		setLineEdit(&lineEdit);
		setModel(&waresModel(dbname));
		setModelColumn(WaresModel::Name);
		completer()->setCompletionMode(QCompleter::PopupCompletion);
	}

	QString text() const { return lineEdit.text(); }
	void setText(const QString & str) { lineEdit.setText(str); }

	EDITOR_LAYOUTS

	QLineEdit lineEdit;
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
	}

	QString text() const { return lineEdit.text(); }
	void setText(const QString & str) { lineEdit.setText(str); }

	EDITOR_LAYOUTS

	QLineEdit lineEdit;
	QLabel label;
};

class ShopSelector : public QComboBox
{
private:
	Q_OBJECT
public:
	ShopSelector(const QString & dbname, QWidget * parent = 0) :
		QComboBox(parent),
		label(tr("Shop (place of buy):"))
	{
		setModel(&shopsModel(dbname));
		setModelColumn(Shop::Name);
	}

	EDITOR_LAYOUTS

	QLabel label;
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

#endif
