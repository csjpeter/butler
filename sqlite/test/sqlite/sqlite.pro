TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = sqlite
SOURCES = sqlite.cpp

