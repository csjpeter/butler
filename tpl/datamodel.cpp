Qt::ItemFlags @Type@Model::flags(const QModelIndex & index) const
{
	if(index.row() < (int)dataSet.size() && index.column() < @Type@::NumOfFields - 2)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else if(index.row() < (int)dataSet.size() && index.column() < @Type@::NumOfAllFields)
		return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
	else
		return Qt::NoItemFlags;
}

QVariant @Type@Model::data(const QModelIndex & index, int role) const 
{
	if(!index.isValid())
		return QVariant();

	if(index.parent().isValid())
		return QVariant();

	if(role != Qt::DisplayRole && role != Qt::EditRole)
		return QVariant();

	if((int)dataSet.size() <= index.row())
		return QVariant();

	switch(index.column()){
@For{Field@
		@IfNotSpec{@
		case @Type@::@.EnumName@ :
			return dataSet.queryAt(index.row()).@.Name@;
			break;
		@IfNotSpec}@@IfIsSpec{@
		/*case @Type@::@.EnumName@ :
			{
				QString s;
				s <<= dataSet.queryAt(index.row()).@.Name@;
				return s;
			}
			break;*/
		@IfIsSpec}@
@}@
@For{DerivedField@
		case @Type@::@.EnumName@ :
			return QVariant();
			break;
@}@
		default :
			return QVariant();
	}

	return QVariant();
}

QVariant @Type@Model::headerData(int section, Qt::Orientation orientation, int role) const
{

	if(role != Qt::DisplayRole)
		return QVariant();

	if(orientation != Qt::Horizontal)
		return QVariant("---");

	switch(section){
@For{Field@
		case @Type@::@.EnumName@ :
			return QVariant(tr(Tid@Type@Field@.EnumName@));
			break;
@}@
@For{DerivedField@
		case @Type@::@.EnumName@ :
			return QVariant(tr(Tid@Type@Field@.EnumName@));
			break;
@}@
		default :
			return QVariant();
	}

	return QVariant();
}

bool @Type@Model::setData(const QModelIndex & index, const QVariant & value, int role)
{
	if(!index.isValid())
		return false;

	if(index.parent().isValid())
		return false;

	if(role != Qt::EditRole)
		return false;

	if((int)dataSet.size() <= index.row())
		return false;

	int row = index.row();
	@Type@ modified(dataSet.queryAt(row));

	switch(index.column()){
@For{Field@
	@IfNotSpec{@
	case @Type@::@.EnumName@ :
		modified.@.Name@ <<= value;
		update(row, modified);
		break;
	@IfNotSpec}@@IfIsSpec{@
/*	case @Type@::@.EnumName@ :
		modified.setAs@.EnumName@(value.toString());
		update(row, modified);
		break;*/
	@IfIsSpec}@
@}@
@For{DerivedField@
		case @Type@::@.EnumName@ :
			break;
@}@
		default :
			return false;
	}

	dataChanged(index, index);
	return true;
}

bool @Type@Model::setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role)
{
	(void)section;
	(void)orientation;
	(void)value;
	(void)role;

	/* we dont want to change the headers at all */
	return false;
}

int @Type@Model::rowCount(const QModelIndex & parent) const
{
	(void)parent;

	return dataSet.size();
}

int @Type@Model::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return @Type@::NumOfAllFields;
}

bool @Type@Model::removeRows(
		int row, int count, const QModelIndex & parent)
{
	ModelRemoveGuard g(this, parent, row, row + count - 1);
	return true;
}

bool @Type@Model::insertRows(
		int row, int count, const QModelIndex & parent)
{
	ModelInsertGuard g(this, parent, row, row + count - 1);
	return true;
}

void @Type@Model::sort(int column, Qt::SortOrder order)
{
	bool ascending = (order == Qt::AscendingOrder);
	if(dataSet.ascending == ascending && dataSet.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	dataSet.ascending = ascending;
	dataSet.ordering.moveToFront(static_cast<@Type@::Fields>(column));
	dataSet.sort();
}

@IfSingleKey{@
int @Type@Model::index(@For{KeyField@const @.Type@ & @.Name@, @-@const @.Type@ & @.Name@@}@) const
{
	if(dataSet.has(@For{KeyField@@.Name@, @-@@.Name@@}@))
		return dataSet.index(@For{KeyField@@.Name@, @-@@.Name@@}@);
	else
		return -1;
}
@IfSingleKey}@

const @Type@& @Type@Model::data(int row) const
{
	return dataSet.queryAt(row);
}

void @Type@Model::del(int row)
{
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	dataSet.removeAt(row);
}

void @Type@Model::addNew(@Type@ & obj)
{
	ModelInsertGuard g(this, QModelIndex(), dataSet.size(), dataSet.size());
	dataSet.add(new @Type@(obj));
}

void @Type@Model::update(int row, @Type@ & modified)
{
	dataSet.queryAt(row) = modified;
	dataChanged(index(row, 0), index(row, @Type@::NumOfFields-1));
}

