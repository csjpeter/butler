/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITTAGVIEW_H
#define BUTLER_EDITTAGVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_tag.h>

#include "butler_tagsmodel.h"
#include "butler_tagsmodel.h"

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
class QStandardItemModel;
class QListView;

namespace Butler {

class TagWidget;

class EditTagView : public QDialog
{
private:
	Q_OBJECT

public:
	EditTagView(QWidget *parent, TagsModel &);

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

private:
	Tag tag;
	TagsModel &model;
	QModelIndex cursor;

	QLineEdit *nameEditor;
	QLineEdit *descEditor;

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
};

}

#endif


