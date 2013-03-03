/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITSHOPVIEW_H
#define BUTLER_EDITSHOPVIEW_H

#include <QDialog>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_shop.h>

#include "butler_shopsmodel.h"
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
class QStandardShopModel;
class QListView;
class QStandardShopModel;

namespace Butler {

class TagWidget;

class EditShopView : public QDialog
{
private:
	Q_OBJECT

public:
	EditShopView(QWidget *parent, ShopsModel &);

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
	Shop shop;
	ShopsModel &model;
	QModelIndex cursor;

	QLineEdit *nameEditor;
	QLineEdit *storeNameEditor;
	QLineEdit *cityEditor;
	QLineEdit *addressEditor;
	QLineEdit *companyEditor;

	QPushButton *prevButton;
	QPushButton *saveButton;
	QPushButton *nextButton;
};

}

#endif


