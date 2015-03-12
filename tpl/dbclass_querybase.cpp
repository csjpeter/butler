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
