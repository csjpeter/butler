TEMPLATE = lib

!include(../butler.pri){
	message( "butler.pri can not found" )
}

debug{
	DEFINES += COMPONENT_NAME=\\\"\"ButlerDataModel\\\"\"
	DEFINES += COMPONENT_COLOR=VT_FG_CYAN
}

HEADERS		=	\
			butler_orderedset.h \
			ButlerOrderedSet \
			\
			butler_tag.h \
			ButlerTag \
			butler_tag_set.h \
			ButlerTagSet \
			butler_query.h \
			ButlerQuery \
			butler_query_set.h \
			ButlerQuerySet \
			butler_item.h \
			ButlerItem \
			butler_item_set.h \
			ButlerItemSet \
			\
			butler_tag_db.h \
			butler_item_db.h \
			butler_query_db.h \
			\
			butler_db.h \
			ButlerDb \
			\
			butler_debug.h \
			ButlerDebug

SOURCES		=	\
			\
			butler_tag.cpp \
			butler_tag_set.cpp \
			butler_query.cpp \
			butler_query_set.cpp \
			butler_item.cpp \
			butler_item_set.cpp \
			butler_debug.cpp

TARGET		= butler-datamodel

target.path = $$PREFIX/lib
INSTALLS += target

dev_headers.path = $$PREFIX/include/butler/datamodel-$$APIVERSION
dev_headers.files = $$HEADERS
INSTALLS += dev_headers

system(../gen_prf $$APIVERSION $$PREFIX)
datamodel_prf.path = $$PRF_DIR
datamodel_prf.files = butler-datamodel-$$APIVERSION"."prf
INSTALLS += datamodel_prf

message(------compilation in $(PWD)------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)
message(Qmake incdir:		$$QMAKE_INCDIR)
message(Qt Inc dir:		$$QMAKE_INCDIR_QT)
message(Defines:		$$DEFINES)
message(Config:			$$CONFIG)
message(Qt:			$$QT)
message(------------------)


