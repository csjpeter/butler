
# common project wide defaults

# use development prefix by default on x86
PREFIX = /opt/devroot

# debug compilation options
CONFIG = qt debug build_all warn_on -Wall -Werror

QT = core

INCLUDEPATH = QMAKE_INCDIR QMAKE_INCDIR_QT QMAKE_INCDIR_THREAD
QTDIR_build:REQUIRES="contains(QT_CONFIG, large-config)"

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

DEFINES += VERSION=$$VERSION
DEFINES += PREFIX=$$PREFIX

release {
	CONFIG += silent
}

debug{
	QMAKE_CXXFLAGS_DEBUG +=-O0
	DEFINES	+= DEBUG
}

OBJECTS_DIR = ./tmp
MOC_DIR = ./tmp


