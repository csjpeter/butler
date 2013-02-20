/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_newtagview.h"
#include "butler_tagsmodel.h"
#include "butler_tagsmodel.h"


namespace Butler {

	NewTagView::NewTagView(QWidget *parent, TagsModel &m) :
		QDialog(parent),
		model(m)
	{
		setModal(true);
//		setWindowModality(Qt::WindowModal);
		setWindowTitle(tr("Add new tag"));
		
		QGridLayout *gridLayout = new QGridLayout();
		gridLayout->setColumnStretch(1, 1);
		setLayout(gridLayout);

		QLabel *label = NULL;

		/* short name on the top */
		label = new QLabel(tr("Name:"));
		gridLayout->addWidget(label, 0, 0, 1, 1);
		nameEditor = new QLineEdit;
		gridLayout->addWidget(nameEditor, 0, 1, 1, 3);

		/* description of tag */
		label = new QLabel(tr("Description:"));
		gridLayout->addWidget(label, 1, 0, 1, 1);
		descEditor = new QLineEdit;
		gridLayout->addWidget(descEditor, 1, 1, 1, 3);
		
		/* buttons: done */
		doneButton = new QPushButton;
		connect(doneButton, SIGNAL(clicked(bool)), this, SLOT(doneClickedSlot(bool)));
		doneButton->setText(tr("Done"));
		gridLayout->addWidget(doneButton, 3, 3, 1, 1);

		/* restore last state */
		loadState();
	}

	void NewTagView::showEvent(QShowEvent *event)
	{
		QDialog::showEvent(event);
	}

	void NewTagView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QDialog::closeEvent(event);
	}

	void NewTagView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("newtagview/position", QPoint()).toPoint();
		QSize size = settings.value("newtagview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void NewTagView::saveState()
	{
		QSettings settings(this);
		settings.setValue("newtagview/position", pos());
		settings.setValue("newtagview/size", size());
	}

	void NewTagView::mapToGui()
	{
		nameEditor->setText(tag.name);
		descEditor->setText(tag.description);
	}

	void NewTagView::mapFromGui()
	{
		tag.name = nameEditor->text();
		tag.description = descEditor->text();
	}

	void NewTagView::doneClickedSlot(bool toggled)
	{
		Q_UNUSED(toggled);

		mapFromGui();
		if(model.addNew(tag)){
			tag = Tag();
			mapToGui();
			return accept();
		}

		QMessageBox(	QMessageBox::Warning,
				tr("Add new tag failed"),
				model.error(),
				QMessageBox::Ok,
				0, Qt::Dialog).exec();
	}
}




