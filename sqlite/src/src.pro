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
			butler_sqlite_db.h \
			\
			butler_sqlite_tag.h \
			\
			butler_sqlite_querytags.h \
			butler_sqlite_queries.h \
			\
			butler_sqlite_itemtags.h \
			butler_sqlite_purchaseditem.h \
			butler_sqlite_item.h \
			\
			butler_sqlitedb.h

SOURCES		=	\
			butler_sqlite_db.cpp \
			\
			butler_sqlite_tag.cpp \
			\
			butler_sqlite_querytags.cpp \
			butler_sqlite_queries.cpp \
			\
			butler_sqlite_itemtags.cpp \
			butler_sqlite_purchaseditem.cpp \
			butler_sqlite_item.cpp \
			\
			butler_sqlitedb.cpp


TARGET = butler-sqlite
target.path = $$PREFIX/lib
INSTALLS += target

dev_headers.path = $$PREFIX/include/butler/sqlite-$$APIVERSION
dev_headers.files = butler_sqlitedb.h
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


