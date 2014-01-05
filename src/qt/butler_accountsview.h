/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2014 Csaszar, Peter
 */

#ifndef BUTLER_ACCOUNTSVIEW_H
#define BUTLER_ACCOUNTSVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class EditAccountView;

class AccountsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	AccountsView(const QString & dbname, QWidget * parent = 0);
	virtual ~AccountsView();

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
	void newAccount();
	void editAccount();
	void delAccount();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const QString dbname;
private:
	AccountsModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditAccountView * newAccountView;
	EditAccountView * editAccountView;
};

#endif
