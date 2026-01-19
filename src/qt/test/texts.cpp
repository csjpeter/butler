/*
 * Author: Butler Test Suite
 * Unit tests for butler_texts.cpp
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <QtGui>
#include <csjp_test.h>
#include <butler_texts.h>
#include <QString>

class TestTexts
{
public:
	void testTranslationVariantLong();
	void testTranslationVariantMedium();
	void testTranslationVariantShort();
	void testTranslationVariantSingleOption();
	void testTranslationVariantTwoOptions();
};

void TestTexts::testTranslationVariantLong()
{
	TESTSTEP("Test translationVariant with Long variant");
	QString text = QString("Long text") + QChar(0x9c) + "Medium text" + QChar(0x9c) + "Short";
	QString result = translationVariant(text, TextVariant::Long);
	VERIFY(result == "Long text");
}

void TestTexts::testTranslationVariantMedium()
{
	TESTSTEP("Test translationVariant with Medium variant");
	QString text = QString("Long text") + QChar(0x9c) + "Medium text" + QChar(0x9c) + "Short";
	QString result = translationVariant(text, TextVariant::Medium);
	VERIFY(result == "Medium text");
}

void TestTexts::testTranslationVariantShort()
{
	TESTSTEP("Test translationVariant with Short variant");
	QString text = QString("Long text") + QChar(0x9c) + "Medium text" + QChar(0x9c) + "Short";
	QString result = translationVariant(text, TextVariant::Short);
	VERIFY(result == "Short");
}

void TestTexts::testTranslationVariantSingleOption()
{
	TESTSTEP("Test translationVariant with single option text");
	QString text = "Only one variant";
	
	// All variants should return the same single option
	QString resultLong = translationVariant(text, TextVariant::Long);
	QString resultMedium = translationVariant(text, TextVariant::Medium);
	QString resultShort = translationVariant(text, TextVariant::Short);
	
	VERIFY(resultLong == "Only one variant");
	VERIFY(resultMedium == "Only one variant");
	VERIFY(resultShort == "Only one variant");
}

void TestTexts::testTranslationVariantTwoOptions()
{
	TESTSTEP("Test translationVariant with two options");
	QString text = QString("Long version") + QChar(0x9c) + "Short";
	
	QString resultLong = translationVariant(text, TextVariant::Long);
	QString resultMedium = translationVariant(text, TextVariant::Medium);
	QString resultShort = translationVariant(text, TextVariant::Short);
	
	VERIFY(resultLong == "Long version");
	// Medium should fallback to last available (Short)
	VERIFY(resultMedium == "Short");
	VERIFY(resultShort == "Short");
}

TEST_INIT(Texts)
	TEST_RUN(testTranslationVariantLong);
	TEST_RUN(testTranslationVariantMedium);
	TEST_RUN(testTranslationVariantShort);
	TEST_RUN(testTranslationVariantSingleOption);
	TEST_RUN(testTranslationVariantTwoOptions);
TEST_FINISH(Texts)
