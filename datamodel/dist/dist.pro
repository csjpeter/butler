TEMPLATE = subdirs
SUBDIRS  = src

!include(../../butler.pri){
	message( "butler.pri can not found" )
}

#CONFIG += ordered

# desktop entry:
#desktop_entry.path = $${PREFIX}/share/applications
#desktop_entry.files = butler.desktop
#INSTALLS += desktop_entry


