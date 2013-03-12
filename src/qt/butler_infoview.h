/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_INFOVIEW_H
#define BUTLER_INFOVIEW_H

#include <QWidget>

/*forwards*/
class QAction;
class QToolButton;
class QMenu;
class QToolBar;
class QLabel;
class QTableView;
class QStandardItemModel;
class QSqlTableModel;
class QPlainTextEdit;

class InfoView : public QWidget
{
private:
	Q_OBJECT

public:
	InfoView(QWidget *parent = 0);
	~InfoView();

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();
};

#endif
