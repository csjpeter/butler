@Type@Db::@Type@Db(SqlConnection & sql) : sql(sql) {}

@Type@Db::~@Type@Db() {}

void @Type@Db::tableInit(SqlConnection & sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(tables.has("@Type@s")){
		cols = sql.columns("@Type@s");
		if(
@For{TableField@
				!cols.has("@FieldName@") ||
@Last@
				!cols.has("@FieldName@"))
@}@
			throw DbIncompatibleTableError(
				"Incompatible table @Type@s in the openend database.");
		return;
	}

	sql.exec("CREATE TABLE @Type@s ("
@For{TableField@
		"@FieldSqlDecl@, "
@Last@
		"@FieldSqlDecl@"
@}@
@For{Constraint@
		", @Constraint@"
@}@
		")");

}

void @Type@Db::insert(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO @Type@s (@TableFieldList@) VALUES ("
			"@For{TableField@?, @Last@?@}@)");
@For{TableField@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@}@
	sqlQuery.exec();

@For{SetField@
	@FieldSetSubType@Db db@FieldSetSubType@(sql);
	for(auto& item : obj.@FieldName@)
		db@FieldSetSubType@.insert(item);
@}@

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@s SET @For{TableField@@FieldName@ = ?, @Last@@FieldName@ = ?@}@ WHERE @For{KeyField@@FieldName@ = ? AND @Last@@FieldName@ = ?@}@");
@For{TableField@
	sqlQuery.bindValue(@FieldIdx@, modified.@FieldName@);
@}@
@For{KeyField@
	sqlQuery.bindValue(@NumOfTableFields@+@FieldIdx@, orig.@FieldName@);
@}@
	sqlQuery.exec();

@For{SetField@
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
@}@

	tr.commit();
}

void @Type@Db::del(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM @Type@s WHERE @For{KeyField@@FieldName@ = ? AND @Last@@FieldName@ = ?@}@");
@For{KeyField@
	sqlQuery.bindValue(@FieldIdx@, obj.@FieldName@);
@}@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::query(@Type@Set & list@For{LinkField@, const @FieldType@ & _@FieldName@@}@)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT @TableFieldList@ FROM @Type@"
@IfHasLinkField{@
			" WHERE @For{LinkField@@FieldName@ = ?, @Last@@FieldName@ = ?@}@"
@IfHasLinkField}@
			);
@For{LinkField@
	sqlQuery.bindValue(@FieldIdx@, _@FieldName@);
@}@
	sqlQuery.exec();

	list.clear();

@For{TableField@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@}@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@For{TableField@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@}@
		list.add(record);
	}
	DBG("-----");

	tr.commit();

@For{SetField@
	@FieldSetSubType@Db db@FieldSetSubType@(sql);
	for(auto& item : list)
		db@FieldSetSubType@.query(item.@FieldName@@For{KeyField@, item.@FieldName@@}@);
@}@
}
