/*
 * Author: Butler Test Suite
 * Unit tests for butler_abstract_table_model.h
 */

#ifndef DEBUG
#define DEBUG
#endif

#include <csjp_test.h>
#include <butler_abstract_table_model.h>
#include <QAbstractTableModel>

// Test model implementation for testing the guards
class TestTableModel : public AbstractTableModel
{
private:
	Q_OBJECT
public:
	int resetBeginCount = 0;
	int resetEndCount = 0;
	int removeBeginCount = 0;
	int removeEndCount = 0;
	int insertBeginCount = 0;
	int insertEndCount = 0;
	
	// Implement pure virtual methods
	int rowCount(const QModelIndex & parent = QModelIndex()) const override { return 0; }
	int columnCount(const QModelIndex & parent = QModelIndex()) const override { return 0; }
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override 
	{ 
		return QVariant(); 
	}
	
	// Public wrappers to track calls
	void doResetBegin()
	{
		resetBeginCount++;
		beginResetModel();
	}
	
	void doResetEnd()
	{
		resetEndCount++;
		endResetModel();
	}
	
	void doRemoveBegin(const QModelIndex & parent, int first, int last)
	{
		removeBeginCount++;
		beginRemoveRows(parent, first, last);
	}
	
	void doRemoveEnd()
	{
		removeEndCount++;
		endRemoveRows();
	}
	
	void doInsertBegin(const QModelIndex & parent, int first, int last)
	{
		insertBeginCount++;
		beginInsertRows(parent, first, last);
	}
	
	void doInsertEnd()
	{
		insertEndCount++;
		endInsertRows();
	}
};

class TestAbstractTableModel
{
public:
	void testModelResetGuard();
	void testModelRemoveGuard();
	void testModelInsertGuard();
};

void TestAbstractTableModel::testModelResetGuard()
{
	TESTSTEP("Test ModelResetGuard RAII behavior");
	TestTableModel model;
	
	VERIFY(model.resetBeginCount == 0);
	VERIFY(model.resetEndCount == 0);
	
	{
		model.doResetBegin();
		VERIFY(model.resetBeginCount == 1);
		VERIFY(model.resetEndCount == 0);
		model.doResetEnd();
	}
	
	VERIFY(model.resetBeginCount == 1);
	VERIFY(model.resetEndCount == 1);
}

void TestAbstractTableModel::testModelRemoveGuard()
{
	TESTSTEP("Test ModelRemoveGuard RAII behavior");
	TestTableModel model;
	
	VERIFY(model.removeBeginCount == 0);
	VERIFY(model.removeEndCount == 0);
	
	{
		QModelIndex parent;
		model.doRemoveBegin(parent, 0, 2);
		VERIFY(model.removeBeginCount == 1);
		VERIFY(model.removeEndCount == 0);
		model.doRemoveEnd();
	}
	
	VERIFY(model.removeBeginCount == 1);
	VERIFY(model.removeEndCount == 1);
}

void TestAbstractTableModel::testModelInsertGuard()
{
	TESTSTEP("Test ModelInsertGuard RAII behavior");
	TestTableModel model;
	
	VERIFY(model.insertBeginCount == 0);
	VERIFY(model.insertEndCount == 0);
	
	{
		QModelIndex parent;
		model.doInsertBegin(parent, 0, 1);
		VERIFY(model.insertBeginCount == 1);
		VERIFY(model.insertEndCount == 0);
		model.doInsertEnd();
	}
	
	VERIFY(model.insertBeginCount == 1);
	VERIFY(model.insertEndCount == 1);
}

TEST_INIT(AbstractTableModel)
	TEST_RUN(testModelResetGuard);
	TEST_RUN(testModelRemoveGuard);
	TEST_RUN(testModelInsertGuard);
TEST_FINISH(AbstractTableModel)

#include "abstract_table_model.moc"
