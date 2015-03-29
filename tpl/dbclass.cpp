@Type@Db::@Type@Db(SqlConnection & sql) : sql(sql) {}

@Type@Db::~@Type@Db() {}

void @Type@Db::insert(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("INSERT INTO @Type@ (@TableFieldList@) VALUES ("
			"@For{TableField@?, @-@?@}@)");
@For{TableField@
	sqlQuery.bindValue(@.Idx@, obj.@.Name@);
@}@
	sqlQuery.exec();

@For{SetField@
	@.SetSubType@Db db@.SetSubType@(sql);
	for(auto& item : obj.@.Name@)
		db@.SetSubType@.insert(item);
@}@

	tr.commit();
}

void @Type@Db::update(const @Type@ & orig, const @Type@ & modified)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("UPDATE @Type@ SET @For{TableField@@.Name@ = ?, @-@@.Name@ = ?@}@ "
			"WHERE @For{KeyField@@.Name@ = ? AND @-@@.Name@ = ?@}@");
@For{TableField@
	sqlQuery.bindValue(@.Idx@, modified.@.Name@);
@}@
@For{KeyField@
	sqlQuery.bindValue(@NumOfTableFields@+@.Idx@, orig.@.Name@);
@}@
	sqlQuery.exec();

@For{SetField@
	@.SetSubType@Db db@.SetSubType@(sql);
	for(auto& item : modified.@.Name@){
		if(orig.@.Name@.has(item))
			continue;
		db@.SetSubType@.insert(item);
	}
	for(auto& item : orig.@.Name@){
		if(modified.@.Name@.has(item))
			continue;
		db@.SetSubType@.del(item);
	}
@}@

	tr.commit();
}

void @Type@Db::del(const @Type@ & obj)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("DELETE FROM @Type@ WHERE @For{KeyField@@.Name@ = ? AND @-@@.Name@ = ?@}@");
@For{KeyField@
	sqlQuery.bindValue(@.Idx@, obj.@.Name@);
@}@
	sqlQuery.exec();

	tr.commit();
}

void @Type@Db::query(@Type@Set & list@For{LinkField@, const @.Type@ & _@.Name@@}@)
{
	SqlQuery sqlQuery(sql);
	SqlTransaction tr(sql);

	sqlQuery.prepare("SELECT @TableFieldList@ FROM @Type@"
@IfHasLinkField{@
			" WHERE @For{LinkField@@.Name@ = ?, @-@@.Name@ = ?@}@"
@}IfHasLinkField@
			);
@For{LinkField@
	sqlQuery.bindValue(@.Idx@, _@.Name@);
@}@
	sqlQuery.exec();

	list.clear();

@For{TableField@
	int @.Name@No = sqlQuery.colIndex("@.Name@");
@}@

	DBG("----- Reading all @Type@ from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@For{TableField@
	record->@.Name@ <<= sqlQuery.sqlValue(@.Name@No);
@}@
		list.add(record);
	}
	DBG("-----");

	tr.commit();

@For{SetField@
	@.SetSubType@Db db@.SetSubType@(sql);
	for(auto& item : list)
		db@.SetSubType@.query(item.@.Name@@For{KeyField@, item.@.Name@@}@);
@}@
}
