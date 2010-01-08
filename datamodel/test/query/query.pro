TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = query
SOURCES = query.cpp

