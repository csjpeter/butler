/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_EDITSHOPVIEW_H
#define BUTLER_EDITSHOPVIEW_H

#include <butler_shop.h>
#include <butler_pannview.h>

class EditPartnerView : public PannView
{
private:
	Q_OBJECT
	MY_Q_OBJECT

public:
	EditPartnerView(const QString & dbname, QWidget * parent = 0);
	virtual ~EditPartnerView() {}

	void setCursor(const QModelIndex& index);

	void loadState();
	void saveState();

private:
	virtual void showEvent(QShowEvent *event);
	virtual void closeEvent(QCloseEvent *event);

	void mapToGui();
	void mapFromGui();

	virtual void changeEvent(QEvent * event);
	virtual void resizeEvent(QResizeEvent * event);

private slots:
	void retranslate();
	void applyLayout();
	void relayout();
	void updateToolButtonStates();
	void prevClickedSlot();
	void nextClickedSlot();
	void saveSlot();
	void resetSlot();

private:
	const QString & dbname;
	PartnersModel & model;
	QModelIndex cursor;
	Shop partner;

	Button doneButton;
	Button resetButton;
	Button prevButton;
	Button nextButton;

	InputEditor nameEditor;
	InputEditor storeNameEditor;
	InputEditor cityEditor;
	InputEditor addressEditor;
	InputEditor companyEditor;
};

#endif
