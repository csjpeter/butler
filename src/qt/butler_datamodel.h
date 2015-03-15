/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATAMODEL_H
#define BUTLER_DATAMODEL_H

#include <csjp_reference_container.h>

#include <butler_abstract_table_model.h>
#include <butler_dbclasses.h>
#include <butler_config.h>

@declare@ Tag
class TagModel : public AbstractTableModel
{
@include@ datamodel.h
};

#endif
