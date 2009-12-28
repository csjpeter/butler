TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = item
SOURCES = item.cpp

