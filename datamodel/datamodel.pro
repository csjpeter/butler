TEMPLATE = subdirs
SUBDIRS = src test

!include(butler.pri){
	message( "butler.pri can not found" )
}

debianpackage{
	system(rm debian/config.data)
	system(echo "VERSION_MAJOR=$$VERSION_MAJOR" >> debian/config.data)
	system(echo "VERSION_MINOR=$$VERSION_MINOR" >> debian/config.data)
	system(echo "VERSION_PATCH=$$VERSION_PATCH" >> debian/config.data)
	system(echo "APIVERSION=$$APIVERSION" >> debian/config.data)
	system(echo "VERSION=$$VERSION" >> debian/config.data)
	system(echo "PREFIX=$$PREFIX" >> debian/config.data)
	system(echo "PRF_DIR=$$PRF_DIR" >> debian/config.data)
}

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
message(------Project informations-------)
message(Prefix:			$$PREFIX)
message(Api Version:		$$APIVERSION)
message(Version:		$$VERSION)
message(Defines:		$$DEFINES)

