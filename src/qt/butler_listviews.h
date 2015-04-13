/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_LISTVIEWS_H
#define BUTLER_LISTVIEWS_H

#include <butler_pannview.h>
#include <butler_databases.h>

@include@ views.h

@ForTypes{Company,Partner,Account,Ware,Tag,Brand,Inventory@
@include@ simpleListViewDeclaration
@}ForTypes@

#endif

