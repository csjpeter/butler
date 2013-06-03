/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_SETTINGSVIEW_H
#define BUTLER_SETTINGSVIEW_H

#include <butler_pannview.h>
#include <butler_databases.h>

/*forwards*/
class NewTagView;
class EditTagView;

class SettingsView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	SettingsView(const QString & dbname, QWidget * parent = 0);
	virtual ~SettingsView();

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
	void newTag();
	void editTag();
	void delTag();
	void refresh();
	void sortIndicatorChangedSlot(int logicalIndex, Qt::SortOrder order);
	void currentIndexChanged(const QModelIndex & current, const QModelIndex & previous);

private:
	const QString & dbname;
	TagsModel & model;

	ToolButton addButton;
	ToolButton delButton;
	ToolButton editButton;
	ToolButton refreshButton;

	EditTagView * newTagView;
	EditTagView * editTagView;
};

#endif
