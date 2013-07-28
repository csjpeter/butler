/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_COMPANYVIEW_H
#define BUTLER_COMPANYVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class EditCompanyView;

class CompanyView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	CompanyView(const QString & dbname, QWidget * parent = 0);
	virtual ~CompanyView();

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
	void newCompany();
	void editCompany();
	void delCompany();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const QString dbname;
private:
	CompanyModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditCompanyView * newCompanyView;
	EditCompanyView * editCompanyView;
};

#endif
