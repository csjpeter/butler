private:
	Q_OBJECT
	MY_Q_OBJECT;

private:
	SqlConnection & sql;
	@Type@Set dataSet;
public:
	const @Type@Set & set;

public:
	virtual ~@Type@Model() {
		/* FIXME
		 * Since @Type@Model instances are held in a static storage class container somewhere,
		 * it might happen on destruction time, that itemOperationListener (which is also a
		 * static storage class object) got destroyed before some @Type@model instances. */
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
	int index(@For{KeyField@const @.Type@ & @.Name@, @-@const @.Type@ & @.Name@@}@) const NO_FCLOG;
	const @Type@& data(int row) const NO_FCLOG;
	void del(int row);
	void addNew(@Type@ &);
	void update(int row, @Type@ & modified);
	void query();

protected:
	/* return true if 'modified' should be in query list */
	bool queryFilter(const @Type@ & modified);
	static void objInserted(SqlConnection & sql, const @Type@ & obj);
	void objInsertListener(SqlConnection & sql, const @Type@ & obj);
	static void objChange(SqlConnection & sql, const @Type@ & orig, const @Type@ & modified);
	void objChangeListener(SqlConnection & sql, const @Type@ & orig, const @Type@ & modified);
	static void objRemoved(SqlConnection & sql, const @Type@ & removed);
	void objRemovedListener(SqlConnection & sql, const @Type@ & removed);

private:
	static csjp::ReferenceContainer<@Type@Model> operationListeners;

private:

