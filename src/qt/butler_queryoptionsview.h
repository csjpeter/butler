/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_QUERYOPTIONSVIEW_H
#define BUTLER_QUERYOPTIONSVIEW_H

#include <QDialog>

#include "butler_tagsmodel.h"

/*forwards*/
class QAction;
class QToolButton;
class QRadioButton;
class QButtonGroup;
class QLineEdit;
class QTextEdit;
class QDateTimeEdit;
class QPushButton;
class QMenu;
class QToolBar;
class QLabel;
class QComboBox;
class QCheckBox;
class QTableView;
class QScrollArea;
class QListView;
class QStandardItemModel;

namespace Butler {

class TagWidget;

class QueryOptionsView : public QDialog
{
private:
	Q_OBJECT

public:
	QueryOptionsView(Query &query, QWidget *parent = 0);
	virtual ~QueryOptionsView();

protected:

private:
	void showEvent(QShowEvent *event);
	void closeEvent(QCloseEvent *event);

	void loadState();
	void saveState();
	
	void mapToGui();
	void mapFromGui();
	
//	QString stringSetToString(const csjp::OwnerContainer<QString> &strSet);

private slots:
	void okClickedSlot(bool);
	void selectAllClickedSlot(bool);
	void selectNoneClickedSlot(bool);

private:
	Query &query;

	QLineEdit *nameEditor;
	QDateTimeEdit *startDate;
	QDateTimeEdit *endDate;
	QCheckBox *wareFilter;
	QComboBox *wareBox;
	QCheckBox *shopFilter;
	QComboBox *shopBox;
	TagWidget *tagsSelector;
	TagWidget *withoutTagsSelector;

	QButtonGroup *stockOptions;
	QRadioButton *stockOptAll;
	QRadioButton *stockOptOnStock;
	QRadioButton *stockOptUsedUp;
	
	QButtonGroup *tagOptions;
	QRadioButton *tagOptAllMatch;
	QRadioButton *tagOptAnyMatch;

	QPushButton *selectAllButton;
	QPushButton *selectNoneButton;
	QPushButton *okButton;
};

}

#endif

