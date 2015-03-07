@Type@Db::@Type@Db(SqlConnection & sql) :
	sql(sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(!tables.has("@Type@s"))
		sql.exec(	"CREATE TABLE @Type@s ("
@ForEachFieldBegin@
				"@FieldSqlDecl@, "
@ForEachFieldLast@
				"@FieldSqlDecl@"
@ForEachFieldEnd@
@ForEachConstraintBegin@
				", @Constraint@"
@ForEachConstraintEnd@
				")"
				);

	cols = sql.columns("@Type@s");
	if(
@ForEachFieldBegin@
		!cols.has("@FieldName@") ||
@ForEachFieldLast@
		!cols.has("@FieldName@")
@ForEachFieldEnd@
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

	sqlQuery.prepare("INSERT INTO @Type@s (@ForEachFieldBegin@@FieldName@, @ForEachFieldLast@@FieldName@@ForEachFieldEnd@ ) VALUES (@ForEachFieldBegin@?, @ForEachFieldLast@?@ForEachFieldEnd@)");
@ForEachFieldBegin@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@ForEachFieldEnd@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@s SET @ForEachFieldBegin@@FieldName@ = ?, @ForEachFieldLast@@FieldName@ = ?@ForEachFieldEnd@ WHERE @ForEachKeyFieldBegin@@FieldName@ = ? AND @ForEachKeyFieldLast@@FieldName@ = ?@ForEachKeyFieldEnd@");
@ForEachFieldBegin@
	sqlQuery.bindValue(@FieldIdx@, modified.@FieldName@);
@ForEachFieldEnd@
@ForEachKeyFieldBegin@
	sqlQuery.bindValue(@NumOfFields@+@FieldIdx@, orig.@FieldName@);
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

	sqlQuery.prepare("SELECT @ForEachFieldBegin@@FieldName@, @ForEachFieldLast@@FieldName@@ForEachFieldEnd@ FROM @Type@s");
	sqlQuery.exec();

	list.clear();

@ForEachFieldBegin@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@ForEachFieldEnd@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEachFieldBegin@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@ForEachFieldEnd@
		list.add(record);
	}
	DBG("-----");

	tr.commit();
}
