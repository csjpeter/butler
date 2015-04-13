
@Define{showEvent@
void Edit@Type@View::showEvent(QShowEvent *event)
{
	mapToGui();

	PannView::showEvent(event);
	nameEditor.editor.setFocus(Qt::OtherFocusReason);
	relayout();
}
@}Define@

@Define{closeEvent@
void Edit@Type@View::closeEvent(QCloseEvent *event)
{
	saveState();
	PannView::closeEvent(event);
}
@}Define@

@Define{loadState@
void Edit@Type@View::loadState()
{
	QString prefix(cursor.isValid() ? "Edit@Type@View" : "New@Type@View");
	PannView::loadState(prefix);
}
@}Define@

@Define{saveState@
void Edit@Type@View::saveState()
{
	QString prefix(cursor.isValid() ? "Edit@Type@View" : "New@Type@View");
	PannView::saveState(prefix);
}
@}Define@

@Define{changeEvent@
void Edit@Type@View::changeEvent(QEvent * event)
{
	PannView::changeEvent(event);
	if(event->type() == QEvent::LanguageChange)
		retranslate();
}
@}Define@

@Define{resizeEvent@
void Edit@Type@View::resizeEvent(QResizeEvent * event)
{
	if(layout() && (event->size() == event->oldSize()))
		return;
	relayout();
}
@}Define@

@Define{setCursor@
void Edit@Type@View::setCursor(const QModelIndex& index)
{
	ENSURE(index.isValid(), csjp::LogicError);
	ENSURE(index.model() == &model, csjp::LogicError);

	cursor = index;
	setWindowTitle(tr(TidEdit@Type@WindowTitle));
	mapToGui();
}
@}Define@

@Define{prevSlot@
void Edit@Type@View::prevSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (0<cursor.row()) ? (cursor.row()-1) : 0;
	setCursor(model.index(row, col));
}
@}Define@

@Define{nextSlot@
void Edit@Type@View::nextSlot()
{
	int col = cursor.column();
	unsigned row = cursor.row();
	row = (cursor.row() < model.rowCount() - 1) ?
			(cursor.row() + 1) : (model.rowCount() - 1);
	setCursor(model.index(row, col));
}
@}Define@

@Define{saveSlot@
void Edit@Type@View::saveSlot()
{
	mapFromGui();

	saveSlotSpec();

	if(cursor.isValid()){
		if(model.data(cursor.row()) != @type@)
			model.update(cursor.row(), @type@);
		auto row = model.set.index(@For{KeyField@@type@.@.Name@, @-@@type@.@.Name@@}@);
		setCursor(model.index(row, cursor.column()));
		updateToolButtonStates();
		toolBar.setInfo(tr(TidInfoEditSaved));
	} else {
		model.addNew(@type@);

		@type@ = @Type@();
		mapToGui();
		toolBar.setInfo(tr(TidInfoNewSaved));
		//nameEditor.editor.setFocus(Qt::OtherFocusReason);
	}
}
@}Define@

@Define{resetSlot@
void Edit@Type@View::resetSlot()
{
	mapToGui();
}
@}Define@

