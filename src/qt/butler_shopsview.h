/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_PARTNERSVIEW_H
#define BUTLER_PARTNERSVIEW_H

#include <butler_pannview.h>
#include <butler_query.h>
#include <butler_databases.h>

/*forwards*/
class NewShopView;
class EditShopView;

class PartnersView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	PartnersView(const QString & dbname, QWidget * parent = 0);
	virtual ~PartnersView();

	virtual void loadState();
	virtual void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void retranslate();
	void applyLayout();
	void relayout();
	void updateToolButtonStates();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);
	virtual void keyPressEvent(QKeyEvent * event);

private slots:
	void newShop();
	void editShop();
	void delShop();
	void refresh();

private:
	const QString & dbname;
	ShopsModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	NewShopView *newShopView;
	EditShopView *editShopView;
};

#endif
