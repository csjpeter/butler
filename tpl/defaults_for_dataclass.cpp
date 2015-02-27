public:

@ForEachFieldBegin@
		@FieldType@ @FieldName@;	// @FieldComment@
@ForEachFieldEnd@

	enum Fields {
@ForEachFieldBegin@
		@FieldEnumName@,
@ForEachFieldEnd@
		NumOfFields
	};

public:
	@Type@() {}

	explicit @Type@(const @KeyType@ & @KeyName@) :
		@KeyName@(@KeyName@)
	{}

	explicit @Type@(const @Type@ & orig) :
@ForEachFieldBegin@
		@FieldName@(orig.@FieldName@),
@ForEachFieldLast@
		@FieldName@(orig.@FieldName@)
@ForEachFieldEnd@
	{}

	@Type@(@Type@ && temp) :
@ForEachFieldBegin@
		@FieldName@(csjp::move_cast(temp.@FieldName@)),
@ForEachFieldLast@
		@FieldName@(csjp::move_cast(temp.@FieldName@))
@ForEachFieldEnd@
	{}

	~@Type@() {}

	@Type@& operator=(@Type@ && temp)
	{
@ForEachFieldBegin@
		@FieldName@ = csjp::move_cast(temp.@FieldName@);
@ForEachFieldEnd@
		return *this;
	}

	@Type@& operator=(const @Type@ & orig)
	{
		@Type@ copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const @Type@ & other) const
	{
		if(
@ForEachFieldBegin@
				@FieldName@ != other.@FieldName@ ||
@ForEachFieldLast@
				@FieldName@ != other.@FieldName@
@ForEachFieldEnd@
				)
				return false;
		return true;
	}

	bool isLess(const @Type@ & other) const
	{
		return @KeyName@ < other.@KeyName@;
	}

	bool isMore(const @Type@ & other) const
	{
		return other.@KeyName@ < @KeyName@;
	}

	bool isLess(const @KeyType@ & otherKey) const
	{
		return @KeyName@ < otherKey;
	}

	bool isMore(const @KeyType@ & otherKey) const
	{
		return otherKey < @KeyName@;
	}

