
@Define{prevOrNextClicked@
void Edit@Type@View::prevClickedSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}

void Edit@Type@View::nextClickedSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (cursor.row() < model.rowCount() - 1) ?
			(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}
@}Define@

