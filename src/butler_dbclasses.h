/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DBCLASSES_H
#define BUTLER_DBCLASSES_H

#include <butler_sql_connection.h>
#include <butler_item_db.h>
#include <butler_queries_db.h>

@declare@ Tag
class TagDb
{
@include@ dbclass.h
};

@declare@ WareType
class WareTypeDb
{
@include@ dbclass.h
};

@declare@ WareTag
class WareTagDb
{
@include@ dbclass.h
};

@declare@ Ware
class WareDb
{
@include@ dbclass.h
};

@declare@ Company
class CompanyDb
{
@include@ dbclass.h
};

@declare@ Brand
class BrandDb
{
@include@ dbclass.h
};

@declare@ Inventory
class InventoryDb
{
@include@ dbclass.h
};

@declare@ Partner
class PartnerDb
{
@include@ dbclass.h
};

@declare@ Account
class AccountDb
{
@include@ dbclass.h
};

@declare@ Payment
class PaymentDb
{
@include@ dbclass.h
};

@declare@ Item

@declare@ QueryWithTag
class QueryWithTagDb
{
@include@ dbclass.h
};

@declare@ QueryWithoutTag
class QueryWithoutTagDb
{
@include@ dbclass.h
};

@declare@ QueryWare
class QueryWareDb
{
@include@ dbclass.h
};

@declare@ QueryPartner
class QueryPartnerDb
{
@include@ dbclass.h
};

@declare@ Query
class QueryDb
{
@include@ dbclass.h
};

#endif
