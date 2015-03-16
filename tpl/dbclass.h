public:
	@Type@Db(SqlConnection & sql);
	virtual ~@Type@Db();
	static void tableInit(SqlConnection & sql);

private:
	@Type@Db();

public:
	void insert(const @Type@ &);
	void update(const @Type@ & orig, const @Type@ & modified);
	void del(const @Type@ &);
	void query(@Type@Set &@For{LinkField@, const @.Type@ & @.Name@@}@);

private:
	SqlConnection & sql;
