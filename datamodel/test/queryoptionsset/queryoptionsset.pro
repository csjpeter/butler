TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = queryoptionsset
SOURCES = queryoptions_set.cpp

