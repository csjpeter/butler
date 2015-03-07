class @Type@Db
{
public:
	@Type@Db(SqlConnection & sql);
	virtual ~@Type@Db();
private:
	@Type@Db();

public:
	void insert(const @Type@ &);
	void update(const @Type@ & orig, const @Type@ & modified);
	void del(const @Type@ &);
	void query(@Type@Set &);

private:
	SqlConnection & sql;
};
