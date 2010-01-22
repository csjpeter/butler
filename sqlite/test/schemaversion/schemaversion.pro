TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = schemaversion
SOURCES = schemaversion.cpp

