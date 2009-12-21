
# some values to set manually

# use development prefix by default on x86
PREFIX = /opt/devroot

# debug compilation options
CONFIG = qt debug build_all warn_on -Wall -Werror
release {
	CONFIG += silent
}

QMAKE_CXXFLAGS_DEBUG +=-O0

# try to detect maemo environment 

MAEMOTARGET = $$system(cpp -dM /dev/null | grep ARM)
message(MAEMOTARGET:	$$MAEMOTARGET)
!isEmpty(MAEMOTARGET){
	CONFIG += maemo
}
maemo{
	DEFINES += MAEMO
}

#set prefix /usr for armel target

ARMELTARGET = $$system(cpp -dM /dev/null | grep __ARMEL__)
ARMELTARGET = $$find(ARMELTARGET, __ARMEL__)
contains(ARMELTARGET, __ARMEL__){
	CONFIG += armel
}

armel{
	PREFIX = /usr
}

DEFINES		+= PREFIX=$$PREFIX

debug{
	DEFINES	+= DEBUG
	DEFINES += COMPONENT_NAME=\\\"\"ButlerDataModel\\\"\"
	DEFINES += COMPONENT_COLOR=\\\"\"VT_FG_CYAN\\\"\"
}

