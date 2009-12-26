TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = queryoptions
SOURCES = queryoptions.cpp

