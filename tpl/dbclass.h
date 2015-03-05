class @Type@Db
{
public:
	@Type@Db(SqlConnection & sql);
	virtual ~@Type@Db();
private:
	@Type@Db();

public:
	void insert(const @Type@ & t);
	void update(const @Type@ & orig, const @Type@ & modified);
	void del(const @Type@ & t);
	void query(@Type@Set & ts);

private:
	SqlConnection & sql;
};
