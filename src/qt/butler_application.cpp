/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

//#define DEBUG

#include <QtGui>

#include <config.h>

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

/* calculate screen physical properties and pixels per milimeter */
void Application::pixelPerMM()
{
	double dpix = qApp->desktop()->physicalDpiX();
	double dpiy = qApp->desktop()->physicalDpiY();
	LOG("Physical DpiX: %, DpiY: %", dpix, dpiy);

	dpix = qApp->desktop()->logicalDpiX();
	dpiy = qApp->desktop()->logicalDpiY();
	LOG("Logical DpiX: %, DpiY: %", dpix, dpiy);

	double xpxPerMM = dpix/25.4 * 1.07; /* correction based on experience */
	double ypxPerMM = dpiy/25.4 * 1.07; /* correction based on experience */

	dpix = qApp->desktop()->physicalDpiX();
	dpiy = qApp->desktop()->physicalDpiY();
	xpxPerMM = dpix/25.4 * 1.07; /* correction based on experience */
	ypxPerMM = dpiy/25.4 * 1.07; /* correction based on experience */

	Config::pxPerMM = (xpxPerMM + ypxPerMM) / 2;
	LOG("Horiz pixel per mm: %, Vertic pixel per mm: %, Avg pixel per mm: %",
			xpxPerMM, ypxPerMM, Config::pxPerMM);

	double width = qApp->desktop()->width();
	double height = qApp->desktop()->height();
	LOG("Width: %, Height: %", width, height);

	double widthMM = width/xpxPerMM;
	double heightMM = height/ypxPerMM;
	LOG("Computed Horiz length in mm: %, Vertic length in mm: %", widthMM, heightMM);

	double qtWidthMM = qApp->desktop()->widthMM();
	double qtHeightMM = qApp->desktop()->heightMM();
	LOG("Qt given Horiz length in mm: %, Vertic length in mm: %", qtWidthMM, qtHeightMM);

	double base = 1000 * 1000;
	Config::scaleFactor = log( (widthMM * heightMM) / (50.0 * 60.0) * base ) / log(base);
	LOG("Computed scaleFactor: %", Config::scaleFactor);
}

/* Init the styleshhet */
void Application::loadCSS()
{
	/* Load css file containing sizes in milimeters */
	QString cssFileName(Path::css("application.css"));
	QFile cssFile(cssFileName);
	if(!cssFile.open(QIODevice::ReadOnly))
		throw csjp::FileError("Cant open css file: %", cssFileName);
	QString data(cssFile.readAll());

	/* creating new css code with calculated sizes in px for the current device */
	QString newCSS;
	int lastPos = 0;
	QRegExp regExp(" ([0-9]+\\.?[0-9]*)mm");
	QStringList cssList = data.split(regExp);
	for(int i = 0; i < cssList.size(); i++){
		//LOG("%", cssList[i]);
		newCSS += cssList[i];
		int j = data.indexOf(regExp, lastPos);
		if(-1 < j){
			lastPos = j + regExp.matchedLength();
			double mm = regExp.cap(1).toDouble();
			int px = round(mm * Config::pxPerMM * Config::scaleFactor); /* math round */
			if(!px && 0.01 <= mm) /* except sizes less 1 but not zero */
				px = 1;
			newCSS += " ";
			newCSS += QString::number(px);
			newCSS += "px";
		} else
			ENSURE(i == cssList.size() - 1, csjp::LogicError);
	}

	/* Finalizing icon paths */
	newCSS.replace("ICONS_DIR/", Path::icon(""));

	/* joining breaked long lines */
	newCSS.replace("\\\n", "");

	qApp->setStyleSheet(newCSS);
	DBG("CSS content:\n", newCSS);
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
