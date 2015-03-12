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
@For{Field@
			case @Type@::@.EnumName@ :
				ret = a.@.Name@ < b.@.Name@;
				break;
@}@
			default:
				ret =
@For{KeyField@
					a.@.Name@ < b.@.Name@ &&
@-@
					a.@.Name@ < b.@.Name@;
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
@For{Field@
		ordering.add(@Type@::@.EnumName@);
@}@
	}
	@Type@Set(const @Type@Set & ts) :
		csjp::SorterOwnerContainer<@Type@>(ts),
		ascending(true)
	{
@For{Field@
		ordering.add(@Type@::@.EnumName@);
@}@
	}
	~@Type@Set() {}
};

