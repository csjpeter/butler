/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATAMODEL_H
#define BUTLER_DATAMODEL_H

#include <csjp_reference_container.h>

#include <butler_conversions.h>
#include <butler_abstract_table_model.h>
#include <butler_config.h>



class TagModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	TagSet dataSet;
public:
	const TagSet & set;

public:
	virtual ~TagModel() {
		/* FIXME
		 * Since TagModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Tagmodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Tag& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Tag &);
	void update(int row, Tag & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Tag & modified);
	static void objInserted(SqlConnection & sql, const Tag & obj);
	void objInsertListener(SqlConnection & sql, const Tag & obj);
	static void objChange(SqlConnection & sql, const Tag & orig, const Tag & modified);
	void objChangeListener(SqlConnection & sql, const Tag & orig, const Tag & modified);
	static void objRemoved(SqlConnection & sql, const Tag & removed);
	void objRemovedListener(SqlConnection & sql, const Tag & removed);

private:
	static csjp::RefArray<TagModel> operationListeners;

private:


public:
	TagModel(SqlConnection & sql);

};
inline bool operator==(const TagModel & a, const TagModel & b) { return &a == &b; }

class WareModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	WareSet dataSet;
public:
	const WareSet & set;

public:
	virtual ~WareModel() {
		/* FIXME
		 * Since WareModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Waremodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Ware& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Ware &);
	void update(int row, Ware & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Ware & modified);
	static void objInserted(SqlConnection & sql, const Ware & obj);
	void objInsertListener(SqlConnection & sql, const Ware & obj);
	static void objChange(SqlConnection & sql, const Ware & orig, const Ware & modified);
	void objChangeListener(SqlConnection & sql, const Ware & orig, const Ware & modified);
	static void objRemoved(SqlConnection & sql, const Ware & removed);
	void objRemovedListener(SqlConnection & sql, const Ware & removed);

private:
	static csjp::RefArray<WareModel> operationListeners;

private:


public:
	WareModel(SqlConnection & sql);

};
inline bool operator==(const WareModel & a, const WareModel & b) { return &a == &b; }

class CompanyModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	CompanySet dataSet;
public:
	const CompanySet & set;

public:
	virtual ~CompanyModel() {
		/* FIXME
		 * Since CompanyModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Companymodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Company& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Company &);
	void update(int row, Company & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Company & modified);
	static void objInserted(SqlConnection & sql, const Company & obj);
	void objInsertListener(SqlConnection & sql, const Company & obj);
	static void objChange(SqlConnection & sql, const Company & orig, const Company & modified);
	void objChangeListener(SqlConnection & sql, const Company & orig, const Company & modified);
	static void objRemoved(SqlConnection & sql, const Company & removed);
	void objRemovedListener(SqlConnection & sql, const Company & removed);

private:
	static csjp::RefArray<CompanyModel> operationListeners;

private:


public:
	CompanyModel(SqlConnection & sql);

};
inline bool operator==(const CompanyModel & a, const CompanyModel & b) { return &a == &b; }

class BrandModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	BrandSet dataSet;
public:
	const BrandSet & set;

public:
	virtual ~BrandModel() {
		/* FIXME
		 * Since BrandModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Brandmodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Brand& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Brand &);
	void update(int row, Brand & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Brand & modified);
	static void objInserted(SqlConnection & sql, const Brand & obj);
	void objInsertListener(SqlConnection & sql, const Brand & obj);
	static void objChange(SqlConnection & sql, const Brand & orig, const Brand & modified);
	void objChangeListener(SqlConnection & sql, const Brand & orig, const Brand & modified);
	static void objRemoved(SqlConnection & sql, const Brand & removed);
	void objRemovedListener(SqlConnection & sql, const Brand & removed);

private:
	static csjp::RefArray<BrandModel> operationListeners;

private:


public:
	BrandModel(SqlConnection & sql);

};
inline bool operator==(const BrandModel & a, const BrandModel & b) { return &a == &b; }

class InventoryModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	InventorySet dataSet;
public:
	const InventorySet & set;

public:
	virtual ~InventoryModel() {
		/* FIXME
		 * Since InventoryModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Inventorymodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Inventory& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Inventory &);
	void update(int row, Inventory & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Inventory & modified);
	static void objInserted(SqlConnection & sql, const Inventory & obj);
	void objInsertListener(SqlConnection & sql, const Inventory & obj);
	static void objChange(SqlConnection & sql, const Inventory & orig, const Inventory & modified);
	void objChangeListener(SqlConnection & sql, const Inventory & orig, const Inventory & modified);
	static void objRemoved(SqlConnection & sql, const Inventory & removed);
	void objRemovedListener(SqlConnection & sql, const Inventory & removed);

private:
	static csjp::RefArray<InventoryModel> operationListeners;

private:


public:
	InventoryModel(SqlConnection & sql);

};
inline bool operator==(const InventoryModel & a, const InventoryModel & b) { return &a == &b; }

class PartnerModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	PartnerSet dataSet;
public:
	const PartnerSet & set;

public:
	virtual ~PartnerModel() {
		/* FIXME
		 * Since PartnerModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Partnermodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Partner& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Partner &);
	void update(int row, Partner & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Partner & modified);
	static void objInserted(SqlConnection & sql, const Partner & obj);
	void objInsertListener(SqlConnection & sql, const Partner & obj);
	static void objChange(SqlConnection & sql, const Partner & orig, const Partner & modified);
	void objChangeListener(SqlConnection & sql, const Partner & orig, const Partner & modified);
	static void objRemoved(SqlConnection & sql, const Partner & removed);
	void objRemovedListener(SqlConnection & sql, const Partner & removed);

private:
	static csjp::RefArray<PartnerModel> operationListeners;

private:


public:
	PartnerModel(SqlConnection & sql);

};
inline bool operator==(const PartnerModel & a, const PartnerModel & b) { return &a == &b; }

class AccountModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	AccountSet dataSet;
public:
	const AccountSet & set;

public:
	virtual ~AccountModel() {
		/* FIXME
		 * Since AccountModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Accountmodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const Account& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Account &);
	void update(int row, Account & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Account & modified);
	static void objInserted(SqlConnection & sql, const Account & obj);
	void objInsertListener(SqlConnection & sql, const Account & obj);
	static void objChange(SqlConnection & sql, const Account & orig, const Account & modified);
	void objChangeListener(SqlConnection & sql, const Account & orig, const Account & modified);
	static void objRemoved(SqlConnection & sql, const Account & removed);
	void objRemovedListener(SqlConnection & sql, const Account & removed);

private:
	static csjp::RefArray<AccountModel> operationListeners;

private:


public:
	AccountModel(SqlConnection & sql);

};
inline bool operator==(const AccountModel & a, const AccountModel & b) { return &a == &b; }

class ItemQueryModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	ItemQuerySet dataSet;
public:
	const ItemQuerySet & set;

public:
	virtual ~ItemQueryModel() {
		/* FIXME
		 * Since ItemQueryModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some ItemQuerymodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const ItemQuery& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(ItemQuery &);
	void update(int row, ItemQuery & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const ItemQuery & modified);
	static void objInserted(SqlConnection & sql, const ItemQuery & obj);
	void objInsertListener(SqlConnection & sql, const ItemQuery & obj);
	static void objChange(SqlConnection & sql, const ItemQuery & orig, const ItemQuery & modified);
	void objChangeListener(SqlConnection & sql, const ItemQuery & orig, const ItemQuery & modified);
	static void objRemoved(SqlConnection & sql, const ItemQuery & removed);
	void objRemovedListener(SqlConnection & sql, const ItemQuery & removed);

private:
	static csjp::RefArray<ItemQueryModel> operationListeners;

private:


public:
	ItemQueryModel(SqlConnection & sql);

};
inline bool operator==(const ItemQueryModel & a, const ItemQueryModel & b) { return &a == &b; }

class PaymentQueryModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	PaymentQuerySet dataSet;
public:
	const PaymentQuerySet & set;

public:
	virtual ~PaymentQueryModel() {
		/* FIXME
		 * Since PaymentQueryModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some PaymentQuerymodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const Text & name) const NO_FCLOG;
	const PaymentQuery& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(PaymentQuery &);
	void update(int row, PaymentQuery & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const PaymentQuery & modified);
	static void objInserted(SqlConnection & sql, const PaymentQuery & obj);
	void objInsertListener(SqlConnection & sql, const PaymentQuery & obj);
	static void objChange(SqlConnection & sql, const PaymentQuery & orig, const PaymentQuery & modified);
	void objChangeListener(SqlConnection & sql, const PaymentQuery & orig, const PaymentQuery & modified);
	static void objRemoved(SqlConnection & sql, const PaymentQuery & removed);
	void objRemovedListener(SqlConnection & sql, const PaymentQuery & removed);

private:
	static csjp::RefArray<PaymentQueryModel> operationListeners;

private:


public:
	PaymentQueryModel(SqlConnection & sql);

};
inline bool operator==(const PaymentQueryModel & a, const PaymentQueryModel & b) { return &a == &b; }




class ItemModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	ItemSet dataSet;
public:
	const ItemSet & set;

public:
	virtual ~ItemModel() {
		/* FIXME
		 * Since ItemModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Itemmodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const DateTime & uploadDate) const NO_FCLOG;
	const Item& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Item &);
	void update(int row, Item & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Item & modified);
	static void objInserted(SqlConnection & sql, const Item & obj);
	void objInsertListener(SqlConnection & sql, const Item & obj);
	static void objChange(SqlConnection & sql, const Item & orig, const Item & modified);
	void objChangeListener(SqlConnection & sql, const Item & orig, const Item & modified);
	static void objRemoved(SqlConnection & sql, const Item & removed);
	void objRemovedListener(SqlConnection & sql, const Item & removed);

private:
	static csjp::RefArray<ItemModel> operationListeners;

private:


public:
	ItemModel(SqlConnection & sql, const WareModel & wmodel);
	const WareModel & wmodel;
private:
/*	QVariant dataUnitPrice(int row) const
	{
		auto & item = dataSet.queryAt(row);
		Double unitPrice(item.price / item.quantity);
		return QVariant(unitPrice.val);
	}
	QVariant dataQuantityWithUnit(int row) const
	{
		auto & item = dataSet.queryAt(row);
		int i = wmodel.index(item.name);
		QString val;
		val.setNum(item.quantity.val, 'g', 3);
		if(i != -1) {
			const Ware & w = wmodel.data(i);
			val += " ";
			val += w.unit;
		}
		return QVariant(val);
	}*/
public:
	ItemQuery opts;
	ItemQueryStat stat;
};
inline bool operator==(const ItemModel & a, const ItemModel & b) { return &a == &b; }


class PaymentModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	PaymentSet dataSet;
public:
	const PaymentSet & set;

public:
	virtual ~PaymentModel() {
		/* FIXME
		 * Since PaymentModel instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some Paymentmodel instances. */
		if(operationListeners.has(*this))
			operationListeners.remove(*this);
	}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const DateTime & uploadDate) const NO_FCLOG;
	const Payment& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(Payment &);
	void update(int row, Payment & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const Payment & modified);
	static void objInserted(SqlConnection & sql, const Payment & obj);
	void objInsertListener(SqlConnection & sql, const Payment & obj);
	static void objChange(SqlConnection & sql, const Payment & orig, const Payment & modified);
	void objChangeListener(SqlConnection & sql, const Payment & orig, const Payment & modified);
	static void objRemoved(SqlConnection & sql, const Payment & removed);
	void objRemovedListener(SqlConnection & sql, const Payment & removed);

private:
	static csjp::RefArray<PaymentModel> operationListeners;

private:


public:
	PaymentModel(SqlConnection & sql);
public:
	PaymentQuery opts;
	PaymentQueryStat stat;
};
inline bool operator==(const PaymentModel & a, const PaymentModel & b) { return &a == &b; }



class DatabaseDescriptorModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	DatabaseDescriptorSet & dataSet;
public:
	const DatabaseDescriptorSet & set;

public:
	DatabaseDescriptorModel(DatabaseDescriptorSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~DatabaseDescriptorModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	int index(const csjp::String & name) const NO_FCLOG;
	const DatabaseDescriptor& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(DatabaseDescriptor &);
	void update(int row, DatabaseDescriptor & modified);

private:

};
inline bool operator==(const DatabaseDescriptorModel & a, const DatabaseDescriptorModel & b) { return &a == &b; }

class WareTypeModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	WareTypeSet & dataSet;
public:
	const WareTypeSet & set;

public:
	WareTypeModel(WareTypeSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~WareTypeModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const WareType& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(WareType &);
	void update(int row, WareType & modified);

private:

};
inline bool operator==(const WareTypeModel & a, const WareTypeModel & b) { return &a == &b; }

class WareTagModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	WareTagSet & dataSet;
public:
	const WareTagSet & set;

public:
	WareTagModel(WareTagSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~WareTagModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const WareTag& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(WareTag &);
	void update(int row, WareTag & modified);

private:

};
inline bool operator==(const WareTagModel & a, const WareTagModel & b) { return &a == &b; }

class ItemQueryWithTagModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	ItemQueryWithTagSet & dataSet;
public:
	const ItemQueryWithTagSet & set;

public:
	ItemQueryWithTagModel(ItemQueryWithTagSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~ItemQueryWithTagModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const ItemQueryWithTag& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(ItemQueryWithTag &);
	void update(int row, ItemQueryWithTag & modified);

private:

};
inline bool operator==(const ItemQueryWithTagModel & a, const ItemQueryWithTagModel & b) { return &a == &b; }

class ItemQueryWithoutTagModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	ItemQueryWithoutTagSet & dataSet;
public:
	const ItemQueryWithoutTagSet & set;

public:
	ItemQueryWithoutTagModel(ItemQueryWithoutTagSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~ItemQueryWithoutTagModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const ItemQueryWithoutTag& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(ItemQueryWithoutTag &);
	void update(int row, ItemQueryWithoutTag & modified);

private:

};
inline bool operator==(const ItemQueryWithoutTagModel & a, const ItemQueryWithoutTagModel & b) { return &a == &b; }

class ItemQueryWareModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	ItemQueryWareSet & dataSet;
public:
	const ItemQueryWareSet & set;

public:
	ItemQueryWareModel(ItemQueryWareSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~ItemQueryWareModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const ItemQueryWare& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(ItemQueryWare &);
	void update(int row, ItemQueryWare & modified);

private:

};
inline bool operator==(const ItemQueryWareModel & a, const ItemQueryWareModel & b) { return &a == &b; }

class ItemQueryPartnerModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	ItemQueryPartnerSet & dataSet;
public:
	const ItemQueryPartnerSet & set;

public:
	ItemQueryPartnerModel(ItemQueryPartnerSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~ItemQueryPartnerModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const ItemQueryPartner& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(ItemQueryPartner &);
	void update(int row, ItemQueryPartner & modified);

private:

};
inline bool operator==(const ItemQueryPartnerModel & a, const ItemQueryPartnerModel & b) { return &a == &b; }

class PaymentQueryPartnerModel : public AbstractTableModel
{
private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	PaymentQueryPartnerSet & dataSet;
public:
	const PaymentQueryPartnerSet & set;

public:
	PaymentQueryPartnerModel(PaymentQueryPartnerSet & dataSet) : dataSet(dataSet), set(dataSet) {}
	virtual ~PaymentQueryPartnerModel() {}

	/*virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()); const NO_FCLOG*/
	virtual QModelIndex index(
			int row, int column,
			const QModelIndex & parent = QModelIndex()) const
			{ return QAbstractTableModel::index(row, column, parent); }
	virtual Qt::ItemFlags flags(
			const QModelIndex & index) const NO_FCLOG;
	virtual QVariant data(
			const QModelIndex & index, int role = Qt::DisplayRole) const NO_FCLOG;
	virtual QVariant headerData(
			int section, Qt::Orientation orientation,
			int role = Qt::DisplayRole) const NO_FCLOG;
	virtual bool setData(
			const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);
	virtual bool setHeaderData(
			int section, Qt::Orientation orientation, const QVariant & value,
			int role = Qt::EditRole);
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual int columnCount(const QModelIndex & parent = QModelIndex()) const NO_FCLOG;
	virtual bool removeRows(int row, int count, const QModelIndex & parent=QModelIndex());
	virtual bool insertRows(int row, int count, const QModelIndex & parent=QModelIndex());

public slots:
	virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

public:
	const PaymentQueryPartner& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(PaymentQueryPartner &);
	void update(int row, PaymentQueryPartner & modified);

private:

};
inline bool operator==(const PaymentQueryPartnerModel & a, const PaymentQueryPartnerModel & b) { return &a == &b; }



#endif

