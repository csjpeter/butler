@Type@Model::@Type@Model(SqlConnection & sql) :
	sql(sql), db(sql), set(dataSet)
{
	operationListeners.add(*this);
	query();
}

void @Type@Model::query()
{
	ModelResetGuard g(this);
	db.query(dataSet);
}

bool @Type@Model::queryFilter(const @Type@ & modified)
{
	(void)(modified);
	return true;
}

