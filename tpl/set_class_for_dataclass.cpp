class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
public:
	csjp::Array<@Type@::Fields> ordering;
	bool ascending;

	virtual int compare(const @Type@ & a, const @Type@ & b) const
	{
		bool ret;

		switch(ordering[0]) {
@ForEachFieldBegin@
			case @Type@::@FieldEnumName@ :
				ret = a.@FieldName@ < b.@FieldName@;
				break;
@ForEachFieldEnd@
			default:
				ret =
@ForEachKeyFieldBegin@
					a.@FieldName@ < b.@FieldName@ &&
@ForEachKeyFieldLast@
					a.@FieldName@ < b.@FieldName@;
@ForEachKeyFieldEnd@
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
@ForEachFieldBegin@
		ordering.setCapacity(ordering.capacity+1);
		ordering.add(@Type@::@FieldEnumName@);
@ForEachFieldEnd@
	}
	@Type@Set(const @Type@Set & ts) :
		csjp::SorterOwnerContainer<@Type@>(ts),
		ascending(true)
	{
@ForEachFieldBegin@
		ordering.setCapacity(ordering.capacity+1);
		ordering.add(@Type@::@FieldEnumName@);
@ForEachFieldEnd@
	}
	~@Type@Set() {}
};
