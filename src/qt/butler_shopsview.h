/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SHOPSVIEW_H
#define BUTLER_SHOPSVIEW_H

#include <butler_pannview.h>
#include <butler_pannable.h>

#include <butler_query.h>

#include "butler_databases.h"

/*forwards*/
class QLabel;
class QTableView;
class QSqlTableModel;

class NewShopView;
class EditShopView;

class ShopsView : public PannView
{
private:
	Q_OBJECT

public:
	ShopsView(const QString & dbname, QWidget * parent = 0);
	~ShopsView();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	virtual void loadState();
	virtual void saveState();

private slots:
	void newShop();
	void finishedNewShop(int);
	void editShop();
	void finishedEditShop(int);
	void delShop();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);

private:
	const QString & dbname;
	ShopsModel & model;

	Pannable<QTableView> queryView;
	QSqlTableModel *queryTable;

	NewShopView *newShopView;
	EditShopView *editShopView;
};

#endif
