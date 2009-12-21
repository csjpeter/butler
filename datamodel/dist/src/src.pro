TEMPLATE = lib

!include(../custom.pri){
	message( "No custom build options specified" )
}

QT = core sql

HEADERS		=	\
			butler_tag.h \
			ButlerTag \
			butler_tag_set.h \
			ButlerTagSet \
			butler_item.h \
			ButlerItem \
			butler_queryoptions.h \
			ButlerQueryOptions \
			\
			butler_tag_db.h \
			butler_item_db.h \
			butler_queryoptions_db.h \
			\
			butler_db.h \
			ButlerDb \
			\
			butler_debug.h \
			ButlerDebug

SOURCES		=	\
			butler_tag.cpp \
			butler_tag_set.cpp \
			butler_item.cpp \
			butler_queryoptions.cpp \
			butler_debug.cpp

TARGET		= butler_datamodel
VERSION		= 0.0.1

INCLUDEPATH	= QMAKE_INCDIR QMAKE_INCDIR_QT QMAKE_INCDIR_THREAD
QTDIR_build:REQUIRES="contains(QT_CONFIG, large-config)"

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp

target.path = $$PREFIX/bin
INSTALLS += target

message(------$(PWD)------)
message(Butler project locations:)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------)

