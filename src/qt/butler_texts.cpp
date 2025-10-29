/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2013 Csaszar, Peter
 */

#include <QtGlobal>
#include <QtGui>

#include <csjp_string.h>

#include <butler_macros.h>
#include <butler_texts.h>

QString translationVariant(const QString & text, TextVariant variant)
{
	QStringList list = text.split(QChar(0x9c), Qt::SkipEmptyParts);

	int lengthVariantIdx = static_cast<int>(variant);
	if(list.size() <= lengthVariantIdx)
		lengthVariantIdx = list.size() - 1;
	ENSURE(0 <= lengthVariantIdx, csjp::LogicError);

	return list.at(lengthVariantIdx);
}

