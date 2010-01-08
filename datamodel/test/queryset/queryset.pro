TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = queryset
SOURCES = query_set.cpp

