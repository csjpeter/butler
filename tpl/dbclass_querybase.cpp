	sqlQuery.exec();

	list.clear();

@ForEachTableField@
	int @FieldName@No = sqlQuery.colIndex("@FieldName@");
@ForEachTableField}@

	DBG("----- Reading all @Type@s from db:");
	while (sqlQuery.next()) {
		DBG("Next row");
		@Type@ *record = new @Type@();
@ForEachTableField@
	record->@FieldName@ <<= sqlQuery.sqlValue(@FieldName@No);
@ForEachTableField}@
		list.add(record);
	}
	DBG("-----");
