class @Type@Set : public csjp::SorterOwnerContainer<@Type@>
{
public:
	@Type@::Fields ordering;
	bool ascending;

	virtual int compare(const @Type@ & a, const @Type@ & b) const
	{
		bool ret;

		switch(ordering) {
@ForEachFieldBegin@
			case @Type@::@FieldEnumName@ :
				ret = a.@FieldName@ < b.@FieldName@;
				break;
@ForEachFieldEnd@
			default:
				ret = a.@KeyName@ < b.@KeyName@;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	@Type@Set() :
		csjp::SorterOwnerContainer<@Type@>(),
		ordering(@Type@::@KeyEnumName@),
		ascending(true){}
	@Type@Set(const @Type@Set & ts) :
		csjp::SorterOwnerContainer<@Type@>(ts),
		ordering(@Type@::@KeyEnumName@),
		ascending(true){}
	~@Type@Set() {}
};
