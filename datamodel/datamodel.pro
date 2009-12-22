TEMPLATE = subdirs
SUBDIRS = dist test

!include(../butler.pri){
	message( "butler.pri can not found" )
}


#QMAKE_EXTRA_TARGETS += check
#check.commands = @LD_LIBRARY_PATH=dist/src:$$(LD_LIBRARY_PATH) ./test/test_datamodel


#CONFIG += ordered

#include(debian/deb.pri)

message(------Qt informations-------)
message(Qt version:		$$[QT_VERSION])
message(Qt is installed in	$$[QT_INSTALL_PREFIX])
message(Qt resources can be found in the following locations:)
message(Documentation:		$$[QT_INSTALL_DOCS])
message(Header files:		$$[QT_INSTALL_HEADERS])
message(Libraries:		$$[QT_INSTALL_LIBS])
message(Binary files:		$$[QT_INSTALL_BINS])
message(Plugins:		$$[QT_INSTALL_PLUGINS])
message(Data files:		$$[QT_INSTALL_DATA])
message(Translation files:	$$[QT_INSTALL_TRANSLATIONS])
message(Settings:		$$[QT_INSTALL_SETTINGS])
message(Examples:		$$[QT_INSTALL_EXAMPLES])
message(Demonstrations:		$$[QT_INSTALL_DEMOS])
message(------Butler informations-------)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)

