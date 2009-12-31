TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = dbifc
SOURCES = dbifc.cpp

