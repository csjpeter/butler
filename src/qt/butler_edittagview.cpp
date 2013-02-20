/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include "butler_config.h"
#include "butler_edittagview.h"
#include "butler_tagsmodel.h"
#include "butler_tagsmodel.h"


namespace Butler {

	EditTagView::EditTagView(QWidget *parent, TagsModel &m) :
		QDialog(parent),
		model(m)
	{
		setModal(true);
//		setWindowModality(Qt::WindowModal);
		setWindowTitle(tr("Edit tag"));

		QHBoxLayout *hbox = NULL;
		QLabel *label = NULL;
		
		QVBoxLayout *layout = new QVBoxLayout(this);
		setLayout(layout);
		
		QGridLayout *gridLayout = new QGridLayout();
		gridLayout->setColumnStretch(1, 1);
		layout->addLayout(gridLayout);

		/* short name on the top */
		label = new QLabel(tr("Name:"));
		gridLayout->addWidget(label, 0, 0, 1, 1);
		nameEditor = new QLineEdit;
		gridLayout->addWidget(nameEditor, 0, 1, 1, 1);

		/* description of tag */
		label = new QLabel(tr("Description:"));
		gridLayout->addWidget(label, 1, 0, 1, 1);
		descEditor = new QLineEdit;
		gridLayout->addWidget(descEditor, 1, 1, 1, 1);

		/* buttons: prev, save, next */
		hbox = new QHBoxLayout();
		layout->addLayout(hbox);

		prevButton = new QPushButton;
		prevButton->setText(tr("Prev"));
		prevButton->setAutoDefault(false);
		hbox->addWidget(prevButton);

		saveButton = new QPushButton;
		saveButton->setText(tr("Save"));
		saveButton->setAutoDefault(false);
		saveButton->setDefault(true);
		hbox->addWidget(saveButton);

		nextButton = new QPushButton;
		nextButton->setText(tr("Next"));
		nextButton->setAutoDefault(false);
		hbox->addWidget(nextButton);

		/* restore last state */
		loadState();

		connect(prevButton, SIGNAL(clicked()),
				this, SLOT(prevClickedSlot()));
		connect(nextButton, SIGNAL(clicked()),
				this, SLOT(nextClickedSlot()));
		connect(saveButton, SIGNAL(clicked()),
				this, SLOT(saveSlot()));
	}

	void EditTagView::showEvent(QShowEvent *event)
	{
		QDialog::showEvent(event);
	}

	void EditTagView::closeEvent(QCloseEvent *event)
	{
		saveState();

		QDialog::closeEvent(event);
	}

	void EditTagView::loadState()
	{
		QSettings settings(this);
		QPoint pos = settings.value("edittagview/position", QPoint()).toPoint();
		QSize size = settings.value("edittagview/size", QSize()).toSize();
		if(size.isValid())
			resize(size);
		move(pos);
	}

	void EditTagView::saveState()
	{
		QSettings settings(this);
		settings.setValue("edittagview/position", pos());
		settings.setValue("edittagview/size", size());
	}

	void EditTagView::mapToGui()
	{
		tag = Tag(model.tag(cursor.row()));

		nameEditor->setText(tag.name);
		descEditor->setText(tag.description);
	}

	void EditTagView::mapFromGui()
	{
		tag.name = nameEditor->text();
		tag.description = descEditor->text();
	}
	
	void EditTagView::setCursor(const QModelIndex& index)
	{
		ENSURE(index.model() == &model, csjp::LogicError);

		cursor = index;
		mapToGui();
		prevButton->setEnabled(cursor.row() > 0);
		nextButton->setEnabled(cursor.row() < model.rowCount() - 1);
	}

	void EditTagView::prevClickedSlot()
	{
		int col = cursor.column();
		int row = (0<cursor.row()) ? (cursor.row()-1) : 0;
		setCursor(model.index(row, col));
	}

	void EditTagView::nextClickedSlot()
	{
		int col = cursor.column();
		int row = (cursor.row() < model.rowCount() - 1) ?
			(cursor.row() + 1) : (model.rowCount() - 1);
		setCursor(model.index(row, col));
	}

	void EditTagView::saveSlot()
	{
		mapFromGui();
		if(model.update(cursor.row(), tag)){
			return accept();
		}

		QMessageBox(	QMessageBox::Warning,
				tr("Update tag failed"),
				model.error(),
				QMessageBox::Ok,
				0, Qt::Dialog).exec();
	}
}



