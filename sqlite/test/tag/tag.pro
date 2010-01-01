TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = tag
SOURCES = tag.cpp

