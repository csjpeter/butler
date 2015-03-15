private:
	Q_OBJECT
	MY_Q_OBJECT;

public:
		@Type@Model(@Type@Db & db) : db(db), set(dataSet) { query(); }
		virtual ~@Type@Model() {}

	/*	virtual QModelIndex index(
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
		const @Type@& data(int row) NO_FCLOG;
		void del(int row);
		void addNew(@Type@ &);
		void update(int row, @Type@ & modified);
		void query();

	private:
		@Type@Db & db;
		@Type@Set dataSet;
public:
	const @Type@Set & set;
