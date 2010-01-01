PKGROOT="../.."
!include($$PKGROOT/butler.pri){
	message( "$$PKGROOT/butler.pri can not found" )
}

CONFIG += qtestlib
QT += testlib
INCLUDEPATH += $$PKGROOT/src
QMAKE_LIBDIR += $$PKGROOT/src/
LIBS += -lbutler-sqlite


message(------$(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)


