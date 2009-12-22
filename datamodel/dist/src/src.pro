TEMPLATE = lib

VERSION = 0.0.1

!include(../../../butler.pri){
	message( "butler.pri can not found" )
}

debug{
	DEFINES += COMPONENT_NAME=\\\"\"ButlerDataModel\\\"\"
	DEFINES += COMPONENT_COLOR=\\\"\"VT_FG_CYAN\\\"\"
}

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

target.path = $$PREFIX/bin
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


