/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
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

