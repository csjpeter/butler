/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITWAREVIEW_H
#define BUTLER_EDITWAREVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_ware.h>

#include "butler_waresmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QScrollArea;
class QListView;

namespace Butler {

class TagWidget;

class EditWareView : public QDialog
{
private:
	Q_OBJECT

public:
	EditWareView(QWidget *parent, WaresModel &);

	void setCursor(const QModelIndex& index);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void saveSlot();
	void prevClickedSlot();
	void nextClickedSlot();
	void closeSlot();

private:
	Ware ware;
	WaresModel &model;
	QModelIndex cursor;

	QLineEdit *nameEditor;
	QLineEdit *unitEditor;
	QLineEdit *categoriesEditor;
	TagWidget *tagsSelector;

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
	QPushButton *closeButton;
};

}

#endif


