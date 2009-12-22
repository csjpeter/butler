!include(../../butler.pri){
        message( "butler.pri can not found" )
}

CONFIG += qtestlib
QT += testlib
INCLUDEPATH += ../../dist/src
LIBS += -L../../dist/src/ -lbutler_datamodel


message(------$(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)


