Qt::ItemFlags @Type@Model::flags(const QModelIndex & index) const
{
	if(index.row() < (int)set.size() && index.column() < @Type@::NumOfFields - 2)
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

	if((int)set.size() <= index.row())
		return QVariant();

	switch(index.column()){
@For{Field@
		case @Type@::@.EnumName@ :
			return set.queryAt(index.row()).@.Name@;
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
			return QVariant(tr(Tid@Type@Field@.Name@));
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

	if((int)set.size() <= index.row())
		return false;

	switch(index.column()){
@For{Field@
		case @Type@::@.EnumName@ :
			set.queryAt(index.row()).@.Name@ <<= value;
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

	return set.size();
}

int @Type@Model::columnCount(const QModelIndex & parent) const
{
	(void)parent;

	return @Type@::NumOfFields;
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
	if(set.ascending == ascending && set.ordering[0] == column)
		return;

	ModelResetGuard g(this);
	set.ascending = ascending;
	set.ordering.moveToFront(static_cast<@Type@::Fields>(column));
	set.sort();
}

int @Type@Model::index(const Text & name) const
{
	if(set.has(name))
		return set.index(name);
	else
		return -1;
}

const @Type@& @Type@Model::data(int row)
{
	return set.queryAt(row);
}

void @Type@Model::del(int row)
{
	@Type@ & obj = set.queryAt(row);
	db.del(obj);
	ModelRemoveGuard g(this, QModelIndex(), row, row);
	set.removeAt(row);
}

void @Type@Model::addNew(@Type@ & obj)
{
	db.insert(obj);
	ModelInsertGuard g(this, QModelIndex(), set.size(), set.size());
	set.add(new @Type@(obj));
}

void @Type@Model::update(int row, @Type@ & modified)
{
	@Type@ & orig = set.queryAt(row);
	db.update(orig, modified);
	orig = modified;
	dataChanged(index(row, 0), index(row, @Type@::NumOfFields-1));
}

void @Type@Model::query()
{
	ModelResetGuard g(this);
	db.query(set);
}

