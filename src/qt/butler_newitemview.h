/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWITEMVIEW_H
#define BUTLER_NEWITEMVIEW_H

#include <QDialog>

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QDoubleSpinBox;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QScrollArea;
class QStandardItemModel;
class QListView;
class QDataWidgetMapper;
class QModelIndex;
class QComboBox;

#include <butler_item.h>

#include "butler_shoppingmodel.h"
#include "butler_tagsmodel.h"

namespace Butler {

class TagWidget;

class NewItemView : public QDialog
{
private:
	Q_OBJECT

public:
	NewItemView(QWidget *parent, ShoppingModel &);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void doneClickedSlot(bool);
	void nameEditFinishedSlot();
	void nameCurrentIndexChangedSlot(int index);

private:
	Item item;
	ShoppingModel &model;

	TagWidget *tagwidget;

	QLineEdit *nameEditor;
	QComboBox *nameBox;
	QLineEdit *categoryEditor;
	QComboBox *categoryBox;
	QDoubleSpinBox *quantityEditor;
	QLabel *unitLabel;
	QTextEdit *commentEditor;

	QPushButton *doneButton;
};

}

#endif

