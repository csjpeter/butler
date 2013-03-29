/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASES_H
#define BUTLER_DATABASES_H

#include <csjp_exception.h>
#include <csjp_object.h>

#include <butler_database_descriptor.h>

#include <butler_db.h>
#include <butler_itemsmodel.h>
#include <butler_custommodel.h>
#include <butler_shoppingmodel.h>
#include <butler_tagsmodel.h>
#include <butler_shopsmodel.h>
#include <butler_waresmodel.h>
#include <butler_queriesmodel.h>

void registerDatabase(csjp::Object<DatabaseDescriptor> &);

const DatabaseDescriptor & databaseDescriptor(const QString & dbname);

csjp::Object<CustomModel> customModel(const QString & dbname);
ShoppingModel & shoppingModel(const QString & dbname);
TagsModel & tagsModel(const QString & dbname);
ShopsModel & shopsModel(const QString & dbname);
WaresModel & waresModel(const QString & dbname);
QueriesModel & queriesModel(const QString & dbname);

#endif
