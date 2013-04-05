/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include <butler_db.h>
#include <butler_config.h>
#include <butler_kineticscroller.h>

#include "butler_application.h"

Application::Application(int &argc, char *argv[]) :
	QApplication(argc, argv)
{
	QCoreApplication::setOrganizationName(ORGANIZATION);
	QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN_NAME);
	QCoreApplication::setApplicationName(PKGNAME);
}

Application::~Application()
{
}

bool Application::notify(QObject * receiver, QEvent * event)
{
	QString info;

	try {
		switch(event->type()){
		case QEvent::KeyPress:
			{
				QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
				/* Close VKB if return/done is pressed on it. */
				if(keyEvent->key() == Qt::Key_Return)
					postEvent(receiver, new QEvent(
							  QEvent::CloseSoftwareInputPanel));
				LOG("Key press 0x%x for %s:%p",
						keyEvent->key(),
						receiver->metaObject()->className(), receiver);
			}
			break;
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonRelease:
#ifdef DEBUG
			{
				QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
				const QPoint cpos = QCursor::pos();
				QPoint pos = mouseEvent->globalPos();
				DBG("Mouse event at %d, %d (cpos: %d, %d) for %s:%p",
						pos.x(), pos.y(), cpos.x(), cpos.y(),
						receiver->metaObject()->className(), receiver);
			}
#endif
		case QEvent::MouseMove:
			{
				QObject * o = receiver;
				while(o && !kineticScrollers.has(o))
					o = o->parent();
				if(!o)
					break;
				KineticScroller * scroller = kineticScrollers.query(o).scroller;
				if(scroller->eventHandler(receiver, event))
					return true;
			}
			break;
		default:
			break;
		}
		return QApplication::notify(receiver, event);
	} catch(csjp::Exception& e) {
		EXCEPTION(e);
		info = e.what();
	} catch(std::exception& e) {
		info = e.what();
	}

	QMessageBox(	QMessageBox::Warning, "Exception thrown", info,
			QMessageBox::Ok, 0, Qt::Dialog).exec();

	return false; /* event was not handled */
}
