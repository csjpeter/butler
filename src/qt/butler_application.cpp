/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG

#include <QtGui>

//#include <config.h>

#include <csjp_file.h>

#include <butler_databases.h>
#include <butler_config.h>
#include <butler_kineticscroller.h>

#include "butler_application.h"

Application::Application(int & argc, char *argv[]) :
	QApplication(argc, argv),
	mainViewPtr(0)
{
	QCoreApplication::setOrganizationName(ORGANIZATION);
	QCoreApplication::setOrganizationDomain(ORGANIZATION_DOMAIN_NAME);
	QCoreApplication::setApplicationName(PKGNAME);
}

Application::~Application()
{
	delete mainViewPtr;
}

MainView & Application::mainView()
{
	if(!mainViewPtr)
		mainViewPtr = new MainView();
	return *mainViewPtr;
}

void Application::loadTranslation(const char * langCode)
{
	QString lang(langCode);
	if(!lang.length())
		lang = "en" ; //locale.name();
	lang.truncate(2);
	QString trFile(Path::translation(C_STR(lang)));
	LOG("Translation file to load: %", trFile);

	csjp::Object<QTranslator> translator(new QTranslator);
	if(!translator->load(trFile))
		throw csjp::ResourceError("Can not load language file %", trFile);

	qApp->installTranslator(translator.release());
}

bool Application::notify(QObject * receiver, QEvent * event)
{
	//DBG("receiver: % event: %", receiver, event);
	QString info;
	static bool altPressed = false;

	try {
		switch(event->type()){
		case QEvent::KeyPress:
			{
				QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

				if(keyEvent->matches(QKeySequence::Quit)){
					if(mainViewPtr)
						mainViewPtr->accept();
					else
						quit();
					break;
				}

				/* Close VKB if return/done is pressed on it. */
				if(keyEvent->key() == Qt::Key_Return)
					postEvent(receiver, new QEvent(
							  QEvent::CloseSoftwareInputPanel));
				if(keyEvent->key() == Qt::Key_Alt && altPressed == true)
					return true;//repeated alt press QT BUG return as if handled
				else
					altPressed = true;
				/*DBG("Key press % for %:%",
						keyEvent,
						receiver->metaObject()->className(), receiver);*/
			}
			break;
		case QEvent::KeyRelease:
			{
				QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
				if(keyEvent->key() == Qt::Key_Alt)
					altPressed = false;
				/*DBG("Key release % for %:%",
						keyEvent->key(),
						receiver->metaObject()->className(), receiver);*/
			}
			break;
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonRelease:
#ifdef DEBUG
/*			{
				QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
				QPoint pos = mouseEvent->globalPos();
				DBG("Mouse event at %, % for %:%",
						pos.x(), pos.y(),
						receiver->metaObject()->className(), receiver);
			break;
			}*/
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
		/*DBG("Event %:% for %:%",
				event->type(), event,
				receiver->metaObject()->className(), receiver);*/
		bool res = QApplication::notify(receiver, event);
		//DBG("Res ", (res) ? "true" : "false");
		return res;
	} catch(csjp::Exception& e) {
		EXCEPTION(e);
		for(const auto & str : e)
			info += "\n" + QString::fromUtf8(str);
	} catch(std::exception& e) {
		info = QString::fromUtf8(e.what());
	}
	if(info.size()){
		if(1024 < info.size()){
			info.resize(1024);
			info.append(" ...");
		}

		QMessageBox(	QMessageBox::Warning, "Exception thrown", info,
				QMessageBox::Ok, 0, Qt::Dialog).exec();
		return true;
	}

	return false; /* event was not handled */
}

