/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWSHOPVIEW_H
#define BUTLER_NEWSHOPVIEW_H

#include <QDialog>

/*forwards*/
class QAction;
class QToolButton;
class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QScrollArea;
class QStandardShopModel;
class QListView;
class QStandardShopModel;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_shop.h>

#include "butler_shopsmodel.h"

namespace Butler {

class TagWidget;

class NewShopView : public QDialog
{
private:
	Q_OBJECT

public:
	NewShopView(QWidget *parent, ShopsModel &);

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();

	void mapToGui();
	void mapFromGui();

private slots:
	void doneClickedSlot(bool);

private:
	Shop shop;
	ShopsModel &model;

	QLineEdit *nameEditor;
	QLineEdit *storeNameEditor;
	QLineEdit *cityEditor;
	QLineEdit *addressEditor;
	QLineEdit *companyEditor;

	QPushButton *doneButton;
};

}

#endif

