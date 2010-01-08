TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = sqlitedb
SOURCES = sqlitedb.cpp

