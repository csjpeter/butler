void @Type@::tableInit(SqlConnection & sql)
{
	SqlColumns cols;
	const SqlTableNames & tables = sql.tables();

	if(tables.has("@Type@")){
		cols = sql.columns("@Type@");
		if(
@For{TableField@
				!cols.has("@.Name@") ||
@-@
				!cols.has("@.Name@"))
@}@
			throw DbIncompatibleTableError(
				"Incompatible table @Type@ in the openend database.");
		return;
	}

	sql.exec("CREATE TABLE @Type@ ("
@For{TableField@
		"@.SqlDecl@, "
@-@
		"@.SqlDecl@"
@}@
@For{Constraint@
		", @Constraint@"
@}@
		")");

}
