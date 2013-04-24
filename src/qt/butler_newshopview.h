/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWSHOPVIEW_H
#define BUTLER_NEWSHOPVIEW_H

#include <butler_pannview.h>

/*forwards*/
class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QLabel;
class QScrollArea;
class QListView;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_shop.h>

#include "butler_databases.h"

class NewShopView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	NewShopView(const QString & dbname, QWidget * parent = 0);
	virtual ~NewShopView() {}

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

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
