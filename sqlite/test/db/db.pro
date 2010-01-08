TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = db
SOURCES = db.cpp

