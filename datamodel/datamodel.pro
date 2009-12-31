TEMPLATE = subdirs
SUBDIRS = src test

include(butler.pri)


system(./gen_prf $$APIVERSION $$PREFIX)
datamodel_prf.files = butler-datamodel-$$APIVERSION.prf
datamodel_prf.path = $$[QT_INSTALL_DATA]/mkspecs/features
INSTALLS += datamodel_prf


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

