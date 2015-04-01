@Type@Model::@Type@Model(SqlConnection & sql) :
	sql(sql), set(dataSet)
{
	operationListeners.add(*this);
	//dataSet = @Type@Set::fromDb(sql);
}

void @Type@Model::query()
{
	ModelResetGuard g(this);
	dataSet = @Type@Set::fromDb(sql);
}

bool @Type@Model::queryFilter(const @Type@ & modified)
{
	(void)(modified);
	return true;
}

