@Type@Db::@Type@Db(SqlConnection & sql) : sql(sql) {}

@Type@Db::~@Type@Db() {}

void @Type@Db::tableInit(SqlConnection & sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(tables.has("@Type@s")){
		cols = sql.columns("@Type@s");
		if(
@ForEach{TableField@
				!cols.has("@FieldName@") ||
@ForLastTableField@
				!cols.has("@FieldName@"))
@ForEachTableField}@
			throw DbIncompatibleTableError(
				"Incompatible table @Type@s in the openend database.");
		return;
	}

	sql.exec("CREATE TABLE @Type@s ("
@ForEach{TableField@
		"@FieldSqlDecl@, "
@ForLastTableField@
		"@FieldSqlDecl@"
@ForEachTableField}@
@ForEach{Constraint@
		", @Constraint@"
@ForEachConstraint}@
		")");

}

void @Type@Db::insert(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO @Type@s (@TableFieldList@) VALUES ("
			"@ForEach{TableField@?, @ForLastTableField@?@ForEachTableField}@)");
@ForEach{TableField@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@ForEachTableField}@
	sqlQuery.exec();

@ForEach{SetField@
	@FieldSetSubType@Db db@FieldSetSubType@(sql);
	for(auto& item : obj.@FieldName@)
		db@FieldSetSubType@.insert(item);
@ForEachSetField}@

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@s SET @ForEach{TableField@@FieldName@ = ?, @ForLastTableField@@FieldName@ = ?@ForEachTableField}@ WHERE @ForEach{KeyField@@FieldName@ = ? AND @ForLastKeyField@@FieldName@ = ?@ForEachKeyField}@");
@ForEach{TableField@
	sqlQuery.bindValue(@FieldIdx@, modified.@FieldName@);
@ForEachTableField}@
@ForEach{KeyField@
	sqlQuery.bindValue(@NumOfTableFields@+@FieldIdx@, orig.@FieldName@);
@ForEachKeyField}@
	sqlQuery.exec();

@ForEach{SetField@
	@FieldSetSubType@Db db@FieldSetSubType@(sql);
	for(auto& item : modified.@FieldName@){
		if(orig.@FieldName@.has(item))
			continue;
		db@FieldSetSubType@.insert(item);
	}
	for(auto& item : orig.@FieldName@){
		if(modified.@FieldName@.has(item))
			continue;
		db@FieldSetSubType@.del(item);
	}
@ForEachSetField}@

	tr.commit();
}

void @Type@Db::del(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM @Type@s WHERE @ForEach{KeyField@@FieldName@ = ? AND @ForLastKeyField@@FieldName@ = ?@ForEachKeyField}@");
@ForEach{KeyField@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@ForEachKeyField}@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::query(@Type@Set & list@ForEach{LinkField@, const @FieldType@ & _@FieldName@@ForEachLinkField}@)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT @TableFieldList@ FROM @Type@"
@IfHasLinkField{@
			" WHERE @ForEach{LinkField@@FieldName@ = ?, @ForLastLinkField@@FieldName@ = ?@ForEachLinkField}@"
@IfHasLinkField}@
			);
@ForEach{LinkField@
	sqlQuery.bindValue(@FieldIdx@, _@FieldName@);
@ForEachLinkField}@
	sqlQuery.exec();

	list.clear();

@ForEach{TableField@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@ForEachTableField}@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEach{TableField@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@ForEachTableField}@
		list.add(record);
	}
	DBG("-----");

	tr.commit();

@ForEach{SetField@
	@FieldSetSubType@Db db@FieldSetSubType@(sql);
	for(auto& item : list)
		db@FieldSetSubType@.query(item.@FieldName@@ForEach{KeyField@, item.@FieldName@@ForEachKeyField}@);
@ForEachSetField}@
}
