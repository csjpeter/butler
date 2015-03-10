@Type@Db::@Type@Db(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("@Type@s"))
		sql.exec(	"CREATE TABLE @Type@s ("
@ForEachTableFieldBegin@
				"@FieldSqlDecl@, "
@ForEachTableFieldLast@
				"@FieldSqlDecl@"
@ForEachTableFieldEnd@
@ForEachConstraintBegin@
				", @Constraint@"
@ForEachConstraintEnd@
				")"
				);

	cols = sql.columns("@Type@s");
	if(
@ForEachTableFieldBegin@
		!cols.has("@FieldName@") ||
@ForEachTableFieldLast@
		!cols.has("@FieldName@")
@ForEachTableFieldEnd@
	  )
		throw DbIncompatibleTableError(
			"Incompatible table @Type@s in the openend database.");
}

@Type@Db::~@Type@Db()
{
}

void @Type@Db::insert(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO @Type@s (@ForEachTableFieldBegin@@FieldName@, @ForEachTableFieldLast@@FieldName@@ForEachTableFieldEnd@ ) VALUES (@ForEachTableFieldBegin@?, @ForEachTableFieldLast@?@ForEachTableFieldEnd@)");
@ForEachTableFieldBegin@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@ForEachTableFieldEnd@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@s SET @ForEachTableFieldBegin@@FieldName@ = ?, @ForEachTableFieldLast@@FieldName@ = ?@ForEachTableFieldEnd@ WHERE @ForEachKeyFieldBegin@@FieldName@ = ? AND @ForEachKeyFieldLast@@FieldName@ = ?@ForEachKeyFieldEnd@");
@ForEachTableFieldBegin@
	sqlQuery.bindValue(@FieldIdx@, modified.@FieldName@);
@ForEachTableFieldEnd@
@ForEachKeyFieldBegin@
	sqlQuery.bindValue(@NumOfTableFields@+@FieldIdx@, orig.@FieldName@);
@ForEachKeyFieldEnd@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::del(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM @Type@s WHERE @ForEachKeyFieldBegin@@FieldName@ = ? AND @ForEachKeyFieldLast@@FieldName@ = ?@ForEachKeyFieldEnd@");
@ForEachKeyFieldBegin@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@ForEachKeyFieldEnd@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::query(@Type@Set & list)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT @ForEachTableFieldBegin@@FieldName@, @ForEachTableFieldLast@@FieldName@@ForEachTableFieldEnd@ FROM @Type@s");
	sqlQuery.exec();

	list.clear();

@ForEachTableFieldBegin@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@ForEachTableFieldEnd@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEachTableFieldBegin@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@ForEachTableFieldEnd@
		list.add(record);
	}
	DBG("-----");

	tr.commit();
}
