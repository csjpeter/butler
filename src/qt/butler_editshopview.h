/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITSHOPVIEW_H
#define BUTLER_EDITSHOPVIEW_H

#include <butler_pannview.h>
#include <QDataWidgetMapper>
#include <QModelIndex>

#include <butler_shop.h>

#include "butler_databases.h"

/*forwards*/
class QLineEdit;
class QTextEdit;
class QCheckBox;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;

class EditShopView : public PannView
{
private:
	Q_OBJECT

public:
	EditShopView(const QString & dbname, QWidget * parent = 0);

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
	const QString & dbname;
	ShopsModel & model;
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

#endif
