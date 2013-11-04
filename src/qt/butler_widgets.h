/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_WIDGETS_H
#define BUTLER_WIDGETS_H

#include <QtGui>
#include <QString>

#include <csjp_sorter_reference_container.h>

#include <config.h>

#include "butler_databases.h"

#include <butler_kineticscroller.h>

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

enum class LayoutState {
	Narrow = 0,
	Medium,
	Wide,
	Expanding
};

class ToolWidget
{
public:
	virtual ~ToolWidget(){}
	virtual void narrow() = 0;
	virtual void medium() = 0;
	virtual void wide() = 0;
	virtual void expanding() = 0;
	virtual QWidget * qwidget() = 0;
};

class TableView : public QTableView
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	TableView(QWidget * parent = 0);
	virtual void loadState(const QString & pref);
	virtual void saveState(const QString & pref);

signals:
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

protected slots:
	virtual void currentChanged(const QModelIndex & current, const QModelIndex & previous);

private:
	KineticScroller scroller;
};

class HLayout : public QHBoxLayout
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	HLayout();
};

class VLayout : public QVBoxLayout
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	VLayout();
};

class Label : public QLabel
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	Label();
};

class InfoLabel : public Label
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	InfoLabel();
};

class Button : public QPushButton, public ToolWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	Button(		const char * textId,
			const char * textIdContext,
			const QKeySequence & seq = QKeySequence(),
			QWidget * parent = 0);
	virtual void focusInEvent(QFocusEvent * event);
	virtual void narrow();
	virtual void medium();
	virtual void wide();
	virtual void expanding();
	virtual QWidget * qwidget();

public:
	const char * textId;
	const char * textIdContext;
	QShortcut shortcut;
};

class ToolButton : public QToolButton, public ToolWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ToolButton(	const QIcon & icon,
			const char * textId,
			const char * textIdContext,
			const QKeySequence & seq = QKeySequence(),
			QWidget * parent = 0);
	virtual void focusInEvent(QFocusEvent * event);

public:
	virtual void narrow();
	virtual void medium();
	virtual void wide();
	virtual void expanding();
	virtual QWidget * qwidget();

	const char * textId;
	const char * textIdContext;
	QShortcut shortcut;
};

class LineEditor : public QLineEdit
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	LineEditor();
	virtual void focusInEvent(QFocusEvent * e);
	virtual void focusOutEvent(QFocusEvent * e);
	virtual void keyPressEvent(QKeyEvent * e);
	virtual void mousePressEvent(QMouseEvent * event);
	virtual void inputMethodEvent(QInputMethodEvent * event);
	Text text() { return Text(this->QLineEdit::text()); }

private:
	bool justFocusedIn;
};

class InputEditor : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	InputEditor(QWidget * parent = 0);
	virtual ~InputEditor();
	void narrowLayout();
	void wideLayout();

public:
	QLabel label;
	LineEditor editor;
};

class PortEditor : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	PortEditor(QWidget * parent = 0);

signals:
	void valueChanged(double);
	void valueChanged(const QString &);

public slots:
	void textChangedSlot(const QString & str);
	void setValue(unsigned v);

public:
	unsigned value();
	void narrowLayout();
	void wideLayout();

public:
	QLabel label;
	LineEditor editor;
private:
	QRegExpValidator validator;
};

class DoubleEditor : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	DoubleEditor(QWidget * parent = 0);
private:
	void resetRegexp();

signals:
	void valueChanged(double);
	void valueChanged(const QString &);

public slots:
	void textChangedSlot(const QString & newText);
	void setPrecision(unsigned prec = 0);
	void setSuffix(const QString & newSuffix = "");
	void setValue(double v);

public:
	double value();
	void narrowLayout();
	void wideLayout();

public:
	QLabel label;
	LineEditor editor;
private:
	QString suffix;
	int precision;
	QRegExpValidator validator;
};

class QuantityEditor : public DoubleEditor
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	QuantityEditor(QWidget * parent = 0);
};

class PriceEditor : public DoubleEditor
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	PriceEditor(QWidget * parent = 0);
};

class FormCheckBox : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	FormCheckBox(QWidget * parent = 0);
public:
	QLabel label;
	QCheckBox box;
};

class ComboBox : public QComboBox
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ComboBox(QWidget * parent = 0);
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
};

class Selector : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	Selector(QAbstractItemModel * model, int column, QWidget * parent = 0);
	virtual void showEvent(QShowEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void narrowLayout();
	virtual void wideLayout();

public:
	QLabel label;
	ComboBox box;

private:
	TableView * tableViewPtr;
public:
	QBoxLayout * boxLayout; /* This should be valid all the time. */
	TableView & tableView; /* Ownership held by QComboBox */
};

class ComboSelector : public Selector
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ComboSelector(QAbstractItemModel * model = 0, int column = 0, QWidget * parent = 0);

	Text text() const;
	void setText(const QString & str);
	virtual void keyPressEvent(QKeyEvent * event);
	virtual void narrowLayout();
	virtual void wideLayout();

private:
	LineEditor * editorPtr; /* Ownership held by QComboBox */
	TableView * completerTableViewPtr; /* Ownership held by QComboBox's QCompleter */
public:
	LineEditor & editor; /* Ownership held by QComboBox */
	TableView & completerTableView; /* Ownership held by QComboBox's QCompleter */
};

class DateTimeEdit : public QDateTimeEdit
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	DateTimeEdit(QWidget * parent = 0);
	virtual QSize sizeHint() const;
	virtual QSize minimumSizeHint() const;
};

class DateTimeEditor : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	DateTimeEditor(QWidget * parent = 0);
	void narrowLayout();
	void wideLayout();

	DateTimeEdit edit;
	QLabel label;
};

class CommentEditor : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	CommentEditor(QWidget * parent = 0);
	void narrowLayout();
	void wideLayout();

	QTextEdit edit;
	QLabel label;
};

class ButtonGroup : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ButtonGroup(QWidget * parent = 0);
	void narrowLayout();
	void mediumLayout();
	void wideLayout();

	QButtonGroup group;
	QLabel label;
};

class ControlBar : public QScrollArea
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ControlBar(QWidget * parent = 0);
	virtual ~ControlBar();
	void addToolWidget(ToolWidget & tw);
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent * event);
	virtual void changeEvent(QEvent * event);
	void applyLayout();
	void relayout();
	void updateButtons();

public:
	bool updatingButtons;
	bool expanding;
	bool spacerBegin;
	bool spacerEnd;
	bool hideIfEmpty;

private:
	QWidget main;

	KineticScroller scroller;
	csjp::SorterReferenceContainer<ToolWidget> buttons;
};

class ToolBar : public QWidget
{
private:
	Q_OBJECT
	MY_Q_OBJECT
public:
	ToolBar(QWidget * parent = 0);
	virtual ~ToolBar();
	void showEvent(QShowEvent *event);
	void resizeEvent(QResizeEvent * event);
	virtual void changeEvent(QEvent * event);
	void addToolWidget(ToolWidget & tw);
	void updateButtons();
	void applyLayout();
	void relayout();

public slots:
	void setInfo(const QString & text);
	void clearInfo();

public:
	InfoLabel infoLabel;
	ControlBar ctrlBar;
	Button backButton;
};

#endif
