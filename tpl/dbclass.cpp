@Type@Db::@Type@Db(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("@Type@s"))
		sql.exec(	"CREATE TABLE @Type@s ("
				"name TEXT, "
				"description TEXT NOT NULL DEFAULT '', "
				"last_modified TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP, "
				""
				"PRIMARY KEY (name)"
				")"
				);

	cols = sql.columns("@Type@s");
	if(	!cols.has("name") ||
		!cols.has("description") ||
		!cols.has("last_modified")
	  )
		throw DbIncompatibleTableError(
			"Incompatible table @Type@s in the openend database.");
}

@Type@Db::~@Type@Db()
{
}

void @Type@Db::insert(const @Type@ & t)
{
	SqlQuery insertQuery(sql);
	SqlTransaction tr(sql);

	insertQuery.prepare("INSERT INTO @Type@s (name, description) VALUES (?, ?)");
	insertQuery.bindValue(0, t.name);
	insertQuery.bindValue(1, t.description);
	insertQuery.exec();

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@s SET name = ?, description = ? WHERE name = ?");
	sqlQuery.bindValue(0, modified.name);
	sqlQuery.bindValue(1, modified.description);
	sqlQuery.bindValue(2, orig.name);
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::del(const @Type@ & t)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM @Type@s WHERE name = ?");
	sqlQuery.bindValue(0, t.name);
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::query(@Type@Set & list)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT name, description FROM @Type@s");
	sqlQuery.exec();

	list.clear();

	int nameNo = sqlQuery.colIndex("name");
	int descriptionNo = sqlQuery.colIndex("description");

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
		record->name = sqlQuery.text(nameNo);
		record->description = sqlQuery.text(descriptionNo);
		list.add(record);
	}
	DBG("-----");

	tr.commit();
}
