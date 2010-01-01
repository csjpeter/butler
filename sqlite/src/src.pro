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
			butler_queryoptions_sqlite.cpp \
			butler_item_sqlite.cpp


TARGET = butler-sqlite
target.path = $$PREFIX/lib
INSTALLS += target

dev_headers.path = $$PREFIX/include/butler/sqlite-$$APIVERSION
dev_headers.files = $$HEADERS
INSTALLS += dev_headers

system(../gen_prf $$APIVERSION $$PREFIX)
sqlite_prf.path = $$PRF_DIR
sqlite_prf.files = butler-sqlite-$$APIVERSION"."prf
INSTALLS += sqlite_prf

message(------compilation in $(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)


