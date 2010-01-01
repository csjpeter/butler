
#FIXME it should be set from debian/rules whether to compile in debug mode or not. Note we need debug symbols but we dont want debug codes in release.

#
#	Default values ...
#

# use development prefix by default on x86
PREFIX = /opt/devroot
CONFIG = qt debug build_all warn_on butler-datamodel-0.1
QT = core sql
INCLUDEPATH = QMAKE_INCDIR QMAKE_INCDIR_QT QMAKE_INCDIR_THREAD
QTDIR_build:REQUIRES="contains(QT_CONFIG, large-config)"
OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp

#
#	Detections ...
#

# try to detect maemo environment 

MAEMOTARGET = $$system(cpp -dM /dev/null | grep ARM)
#message(MAEMOTARGET:	$$MAEMOTARGET)
!isEmpty(MAEMOTARGET){
	CONFIG += maemo
}
maemo{
	DEFINES += MAEMO
}

# set prefix /usr for armel target

ARMELTARGET = $$system(cpp -dM /dev/null | grep __ARMEL__)
ARMELTARGET = $$find(ARMELTARGET, __ARMEL__)
contains(ARMELTARGET, __ARMEL__){
	CONFIG += armel
}
armel{
	PREFIX = /usr
}

# if packaging is in progress use /usr prefix

exists(debian-packaging-started){
	PREFIX = /usr
	CONFIG += debianpackage
}

# read version number from version file

VERSION_MAJOR = $$system(head -n 1 version)
VERSION_MINOR = $$system(head -n 2 version | tail -n 1)
VERSION_PATCH = $$system(head -n 3 version | tail -n 1)


#
#	Configuration ...
#

PRF_DIR = $$[QT_INSTALL_DATA]/mkspecs/features

APIVERSION = $$VERSION_MAJOR"."$$VERSION_MINOR
VERSION = $$APIVERSION"."$$VERSION_PATCH

DEFINES += VERSION=$$VERSION
DEFINES += PREFIX=$$PREFIX

release {
	CONFIG += silent
	QMAKE_CXXFLAGS += -Werror
}

debug{
	QMAKE_CXXFLAGS_DEBUG +=-O0 -Werror
	DEFINES	+= DEBUG
	QMAKE_CXXFLAGS += -ftest-coverage -fprofile-arcs
	LIBS += -lgcov
}


