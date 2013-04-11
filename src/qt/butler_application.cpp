/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#include <QtGui>

#include <config.h>

#include <butler_db.h>
#include <butler_databases.h>
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

/* Init the local database */
void Application::initLocalDb()
{
	if(Config::dbFileName == ""){
		QSettings settings;
		Config::dbFileName = settings.value("mainview/dbfile", QString()).toString();
	}
	if(Config::dbFileName == ""){
		QDir dir(QDir::homePath());

		if(!dir.exists(".butler"))
			dir.mkdir(".butler");
		Config::dbFileName = QDir::toNativeSeparators(
				QDir::homePath() + QString("/.butler/db.sqlite")
				);
	}
	csjp::Object<DatabaseDescriptor> sqlitedb(new DatabaseDescriptor);
	sqlitedb->name = Config::defaultDbName;
	sqlitedb->driver = "QSQLITE";
	sqlitedb->databaseName = QDir::toNativeSeparators(Config::dbFileName);
	LOG("Db file path: %s", C_STR(sqlitedb->databaseName));
	registerDatabase(sqlitedb);
}

void Application::loadTranslation(const char * langCode)
{
	QString lang(langCode);
	if(!lang.length())
		lang = "en" ; //locale.name();
	lang.truncate(2);
	QString trFile(Path::translation(C_STR(lang)));
	LOG("Translation file to load: %s", C_STR(trFile));

	csjp::Object<QTranslator> translator(new QTranslator);
	if(!translator->load(trFile))
		throw csjp::ResourceError("Can not load language file %s", C_STR(trFile));

	qApp->installTranslator(translator.release());
}

/* calculate screen physical properties and pixels per milimeter */
void Application::pixelPerMM()
{
	double dpix = qApp->desktop()->physicalDpiX();
	double dpiy = qApp->desktop()->physicalDpiY();
	LOG("Physical DpiX: %.2f, DpiY: %.2f", dpix, dpiy);

	dpix = qApp->desktop()->logicalDpiX();
	dpiy = qApp->desktop()->logicalDpiY();
	LOG("Logical DpiX: %.2f, DpiY: %.2f", dpix, dpiy);

	double xpxPerMM = dpix/25.4 * 1.07; /* correction based on experience */
	double ypxPerMM = dpiy/25.4 * 1.07; /* correction based on experience */
	Config::pxPerMM = (xpxPerMM + ypxPerMM) / 2;
	LOG("Horiz pixel per mm: %.2f, Vertic pixel per mm: %.2f, Avg pixel per mm: %.2f",
			xpxPerMM, ypxPerMM, Config::pxPerMM);

	double width = qApp->desktop()->width();
	double height = qApp->desktop()->height();
	LOG("Width: %.2f, Height: %.2f", width, height);

	double widthMM = width/xpxPerMM;
	double heightMM = height/ypxPerMM;
	LOG("Computed Horiz length in mm: %.2f, Vertic length in mm: %.2f", widthMM, heightMM);

	double qtWidthMM = qApp->desktop()->widthMM();
	double qtHeightMM = qApp->desktop()->heightMM();
	LOG("Qt given Horiz length in mm: %.2f, Vertic length in mm: %.2f", qtWidthMM, qtHeightMM);
}

/* Init the styleshhet */
void Application::loadCSS()
{
	/* Load css file containing sizes in milimeters */
	QString cssFileName(Path::css("application.css"));
	QFile cssFile(cssFileName);
	if(!cssFile.open(QIODevice::ReadOnly))
		throw csjp::FileError("Cant open css file: %s", C_STR(cssFileName));
	QString data(cssFile.readAll());

	/* creating new css code with calculated sizes in px for the current device */
	QString newCSS;
	int lastPos = 0;
	QRegExp regExp(" ([0-9]+\\.?[0-9]*)mm;");
	QStringList cssList = data.split(regExp);
	for(int i = 0; i < cssList.size(); i++){
		//LOG("%s", C_STR(cssList[i]));
		newCSS += cssList[i];
		int j = data.indexOf(regExp, lastPos);
		if(-1 < j){
			lastPos = j + regExp.matchedLength();
			double mm = regExp.cap(1).toDouble();
			int px = round(mm * Config::pxPerMM); /* math round */
			if(!px && 0.01 <= mm) /* except sizes less 1 but not zero */
				px = 1;
			newCSS += " ";
			newCSS += QString::number(px);
			newCSS += "px;";
		} else
			ENSURE(i == cssList.size() - 1, csjp::LogicError);
	}

	/* Finalizing icon paths */
	newCSS.replace("ICONS_DIR/", Path::icon(""));

	qApp->setStyleSheet(newCSS);
	DBG("CSS content:\n%s", C_STR(newCSS));
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
				DBG("Key press 0x%x for %s:%p",
						keyEvent->key(),
						receiver->metaObject()->className(), receiver);
			}
			break;
		case QEvent::MouseButtonPress:
		case QEvent::MouseButtonRelease:
#ifdef DEBUG
			{
				QMouseEvent* const mouseEvent = static_cast<QMouseEvent*>(event);
				QPoint pos = mouseEvent->globalPos();
				DBG("Mouse event at %d, %d for %s:%p",
						pos.x(), pos.y(),
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
