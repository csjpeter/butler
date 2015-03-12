	sqlQuery.exec();

	list.clear();

@ForEachTableField@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@}@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEachTableField@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@}@
		list.add(record);
	}
	DBG("-----");
