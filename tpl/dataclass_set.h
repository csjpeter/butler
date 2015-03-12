class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
public:
	csjp::PodArray<@Type@::Fields> ordering;
	bool ascending;

	const @Type@Set & operator=(@Type@Set && temp)
	{
		csjp::SorterOwnerContainer<@Type@>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const @Type@ & a, const @Type@ & b) const
	{
		bool ret;

		switch(ordering[0]) {
@ForEach{Field@
			case @Type@::@FieldEnumName@ :
				ret = a.@FieldName@ < b.@FieldName@;
				break;
@}@
			default:
				ret =
@ForEach{KeyField@
					a.@FieldName@ < b.@FieldName@ &&
@ForLastKeyField@
					a.@FieldName@ < b.@FieldName@;
@}@
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	@Type@Set() :
		csjp::SorterOwnerContainer<@Type@>(),
		ascending(true)
	{
@ForEach{Field@
		ordering.add(@Type@::@FieldEnumName@);
@}@
	}
	@Type@Set(const @Type@Set & ts) :
		csjp::SorterOwnerContainer<@Type@>(ts),
		ascending(true)
	{
@ForEach{Field@
		ordering.add(@Type@::@FieldEnumName@);
@}@
	}
	~@Type@Set() {}
};

