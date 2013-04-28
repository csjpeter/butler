/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARESVIEW_H
#define BUTLER_WARESVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class NewWareView;
class EditWareView;

class WaresView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	WaresView(const QString & dbname, QWidget * parent = 0);
	virtual ~WaresView();

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
	void newWare();
	void editWare();
	void delWare();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

private:
	const QString & dbname;
	WaresModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditWareView * newWareView;
	EditWareView * editWareView;
};

#endif
