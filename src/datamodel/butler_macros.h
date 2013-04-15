/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_MACROS_H
#define BUTLER_MACROS_H

#define C_STR(QStr) QStr.toUtf8().constData()

#define SAVE_VIEW_STATE(view) \
	if(view && view->isVisible()){ \
		view->saveState(); \
		settings.setValue(prefix + "/" #view, true); \
	} else \
		settings.setValue(prefix + "/" #view, false)

#endif

