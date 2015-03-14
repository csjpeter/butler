/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_TYPES_H
#define BUTLER_TYPES_H

enum class QueryStockOptions {
	AllItemChanges,
	Gains,
	Looses
};

enum class QueryTagOptions {
	MatchAll,
	MatchAny
};

#endif
