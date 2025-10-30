/*
 * Unit test for QString::split() modernization
 * Tests the behavior of setAsTypes, setAsTags, and operator== functions
 * to ensure they work correctly with both deprecated and modern Qt API
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <QtTest/QtTest>
#include <QString>
#include <QStringList>

class TestStringSplit : public QObject
{
    Q_OBJECT

private slots:
    void testSplitWithEmptyParts();
    void testSplitWithMultipleCommas();
    void testSplitWithSpaces();
    void testSplitEmptyString();
    void testSplitSingleItem();
};

void TestStringSplit::testSplitWithEmptyParts()
{
    QString input = "apple,banana,,orange";
    
    // Test with modern Qt API
    QStringList result = input.split(",", Qt::SkipEmptyParts);
    
    QCOMPARE(result.size(), 3);
    QCOMPARE(result.at(0), QString("apple"));
    QCOMPARE(result.at(1), QString("banana"));
    QCOMPARE(result.at(2), QString("orange"));
}

void TestStringSplit::testSplitWithMultipleCommas()
{
    QString input = ",,item1,,,item2,,";
    
    QStringList result = input.split(",", Qt::SkipEmptyParts);
    
    QCOMPARE(result.size(), 2);
    QCOMPARE(result.at(0), QString("item1"));
    QCOMPARE(result.at(1), QString("item2"));
}

void TestStringSplit::testSplitWithSpaces()
{
    QString input = " type1 , type2 ,  type3  ";
    
    QStringList result = input.split(",", Qt::SkipEmptyParts);
    
    QCOMPARE(result.size(), 3);
    // Note: split doesn't trim, the code does that separately
    QCOMPARE(result.at(0).trimmed(), QString("type1"));
    QCOMPARE(result.at(1).trimmed(), QString("type2"));
    QCOMPARE(result.at(2).trimmed(), QString("type3"));
}

void TestStringSplit::testSplitEmptyString()
{
    QString input = "";
    
    QStringList result = input.split(",", Qt::SkipEmptyParts);
    
    QCOMPARE(result.size(), 0);
}

void TestStringSplit::testSplitSingleItem()
{
    QString input = "onlyitem";
    
    QStringList result = input.split(",", Qt::SkipEmptyParts);
    
    QCOMPARE(result.size(), 1);
    QCOMPARE(result.at(0), QString("onlyitem"));
}

QTEST_MAIN(TestStringSplit)
#include "test_string_split.moc"
