public:

@ForEachFieldBegin@
	@FieldType@ @FieldName@;@FieldComment@
@ForEachFieldEnd@

	enum Fields {
@ForEachFieldBegin@
		@FieldEnumName@,
@ForEachFieldEnd@
		NumOfFields
	};

public:
	@Type@() {}

	explicit @Type@(
@ForEachKeyFieldBegin@
			const @FieldType@ & @FieldName@,
@ForEachKeyFieldLast@
			const @FieldType@ & @FieldName@ ) :
@ForEachKeyFieldEnd@
@ForEachKeyFieldBegin@
		@FieldName@(@FieldName@),
@ForEachKeyFieldLast@
		@FieldName@(@FieldName@)
@ForEachKeyFieldEnd@
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
		return
@ForEachKeyFieldBegin@
			@FieldName@ < other.@FieldName@ &&
@ForEachKeyFieldLast@
			@FieldName@ < other.@FieldName@;
@ForEachKeyFieldEnd@
	}

	bool isMore(const @Type@ & other) const
	{
		return
@ForEachKeyFieldBegin@
			other.@FieldName@ < @FieldName@ &&
@ForEachKeyFieldLast@
			other.@FieldName@ < @FieldName@;
@ForEachKeyFieldEnd@
	}

	bool isLess(
@ForEachKeyFieldBegin@
			const @FieldType@ & other@FieldName@,
@ForEachKeyFieldLast@
			const @FieldType@ & other@FieldName@) const
@ForEachKeyFieldEnd@
	{
		return
@ForEachKeyFieldBegin@
			@FieldName@ < other@FieldName@ &&
@ForEachKeyFieldLast@
			@FieldName@ < other@FieldName@;
@ForEachKeyFieldEnd@
	}

	bool isMore(
@ForEachKeyFieldBegin@
			const @FieldType@ & other@FieldName@,
@ForEachKeyFieldLast@
			const @FieldType@ & other@FieldName@) const
@ForEachKeyFieldEnd@
	{
		return
@ForEachKeyFieldBegin@
			other@FieldName@ < @FieldName@ &&
@ForEachKeyFieldLast@
			other@FieldName@ < @FieldName@;
@ForEachKeyFieldEnd@
	}

