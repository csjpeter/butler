TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = tagset
SOURCES = tag_set.cpp

