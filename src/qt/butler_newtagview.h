/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_NEWTAGVIEW_H
#define BUTLER_NEWTAGVIEW_H

#include <QDialog>

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
class QTableView;
class QScrollArea;
class QStandardItemModel;
class QListView;
class QDataWidgetMapper;
class QModelIndex;

#include <butler_tag.h>

#include "butler_tagsmodel.h"

class TagWidget;

class NewTagView : public QDialog
{
private:
	Q_OBJECT

public:
	NewTagView(QWidget *parent, TagsModel &);

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
	Tag tag;
	TagsModel &model;

	QLineEdit *nameEditor;
	QLineEdit *descEditor;

	QPushButton *doneButton;
};

#endif
