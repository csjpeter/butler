TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = orderedset
SOURCES = orderedset.cpp

