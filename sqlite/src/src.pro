TEMPLATE = lib

VERSION = 0.0.1

!include(../butler.pri){
	message( "butler.pri can not found" )
}

debug{
	DEFINES += COMPONENT_NAME=\\\"\"ButlerSqlite\\\"\"
	DEFINES += COMPONENT_COLOR=VT_FG_YELLOW
}

HEADERS		=	\
			butler_sqlite.h

SOURCES		=	\
			butler_sqlite.cpp \
			butler_tag_sqlite.cpp \
			butler_item_sqlite.cpp \
			butler_queryoptions_sqlite.cpp

QT += sql

TARGET		= butler_sqlite

target.path = $$PREFIX/lib
INSTALLS += target

message(------compilation in $(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)

