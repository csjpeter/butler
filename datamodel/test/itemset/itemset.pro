TEMPLATE = app

!include(../test.pri){
        message( "test.pri can not found" )
}

TARGET = itemset
SOURCES = item_set.cpp

