TEMPLATE = app

VERSION		= 0.0.1

!include(../../butler.pri){
        message( "butler.pri can not found" )
}

CONFIG += qtestlib
QT += testlib
INCLUDEPATH += ../dist/src
LIBS += -L../dist/src/ -lbutler_datamodel

TARGET		= test_datamodel


SOURCES		=	\
			test_tag.cpp

#			butler_tag_set.cpp \
#			butler_item.cpp \
#			butler_queryoptions.cpp \
#			butler_debug.cpp


message(------$(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)

