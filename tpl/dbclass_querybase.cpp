	sqlQuery.exec();

	list.clear();

@ForEachTableField@
	int @.Name@No = sqlQuery.colIndex("@.Name@");
@}@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEachTableField@
	record->@.Name@ <<= sqlQuery.sqlValue(@.Name@No);
@}@
		list.add(record);
	}
	DBG("-----");
