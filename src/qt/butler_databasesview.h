/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASESVIEW_H
#define BUTLER_DATABASESVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class EditDbDescView;

class DatabasesView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	DatabasesView(QWidget * parent = 0);
	virtual ~DatabasesView();

	virtual void loadState();
	virtual void saveState();

signals:
	void activeDbChanged();

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
	void newDbDesc();
	void editDbDesc();
	void delDbDesc();
	void useDbDesc();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

private:
	ToolButton addButton;
	ToolButton editButton;
	ToolButton delButton;
	ToolButton useButton;

	TableView tableView;

	EditDbDescView * newDbDescView;
	EditDbDescView * editDbDescView;
};

#endif
