/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_BRANDSVIEW_H
#define BUTLER_BRANDSVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class EditBrandView;

class BrandsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	BrandsView(const csjp::String & dbname, QWidget * parent = 0);
	virtual ~BrandsView();

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
	void newBrand();
	void editBrand();
	void delBrand();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

public:
	const csjp::String dbname;
private:
	BrandModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	TableView tableView;

	EditBrandView * newBrandView;
	EditBrandView * editBrandView;
};

#endif
