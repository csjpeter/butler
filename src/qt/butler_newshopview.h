/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWSHOPVIEW_H
#define BUTLER_NEWSHOPVIEW_H

#include <butler_pannview.h>

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
class QScrollArea;
class QStandardShopModel;
class QListView;
class QStandardShopModel;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_shop.h>

#include "butler_databases.h"

class TagWidget;

class NewShopView : public PannView
{
private:
	Q_OBJECT

public:
	NewShopView(const QString & dbname, QWidget *parent);

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
	const QString & dbname;
	ShopsModel & model;

	QLineEdit *nameEditor;
	QLineEdit *storeNameEditor;
	QLineEdit *cityEditor;
	QLineEdit *addressEditor;
	QLineEdit *companyEditor;

	QPushButton *doneButton;
};

#endif
