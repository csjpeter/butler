/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#ifndef BUTLER_TEXTS_H
#define BUTLER_TEXTS_H

enum class TextVariant {
	Long = 0,
	Medium,
	Short
};

QString translationVariant(const QString & text, TextVariant variant);

#endif

