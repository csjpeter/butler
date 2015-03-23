void @Type@::tableInit(SqlConnection & sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	//for(auto & t : tables)
	//	LOG("table %s", t.str);

	if(tables.has("@type@")){
		cols = sql.columns("@type@");
		//LOG("Table @Type@");
		//for(auto & c : cols)
		//	LOG("column %s", c.str);
		if(
@For{TableField@
				!cols.has("@.name@") ||
@-@
				!cols.has("@.name@"))
@}@
			throw DbIncompatibleTableError(
				"Incompatible table @type@ in the openend database.");
		return;
	}

	sql.exec("CREATE TABLE @type@ ("
@For{TableField@
		"@.name@ @.SqlDecl@, "
@-@
		"@.name@ @.SqlDecl@"
@}@
@For{Constraint@
		", @Constraint@"
@}@
		")");

}
