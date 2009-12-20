TEMPLATE = subdirs
SUBDIRS  = src

#CONFIG += ordered

!include(custom.pri){
	message( "No custom build options specified" )
}


# for check:
#check.target = check
#check.CONFIG = recursive
#QMAKE_EXTRA_TARGETS += check

# desktop entry:
#desktop_entry.path = $${PREFIX}/share/applications
#desktop_entry.files = duihelp.desktop
#INSTALLS += desktop_entry




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
message(------------------------)
message(Butler project locations:)
message(Destdir:		$$DESTDIR)
message(Prefix:			$$PREFIX)

