public:

@ForEach{Field@
	@FieldType@ @FieldName@;@FieldComment@
@}@

	enum Fields {
@ForEach{Field@
		@FieldEnumName@,
@}@
		NumOfFields
	};

public:
	@Type@() {}

	explicit @Type@(
@ForEach{KeyField@
			const @FieldType@ & @FieldName@,
@ForLastKeyField@
			const @FieldType@ & @FieldName@ ) :
@}@
@ForEach{KeyField@
		@FieldName@(@FieldName@),
@ForLastKeyField@
		@FieldName@(@FieldName@)
@}@
	{}

	explicit @Type@(const @Type@ & orig) :
@ForEach{Field@
		@FieldName@(orig.@FieldName@),
@ForLastField@
		@FieldName@(orig.@FieldName@)
@}@
	{}

	@Type@(@Type@ && temp) :
@ForEach{Field@
		@FieldName@(csjp::move_cast(temp.@FieldName@)),
@ForLastField@
		@FieldName@(csjp::move_cast(temp.@FieldName@))
@}@
	{}

	~@Type@() {}

	@Type@& operator=(@Type@ && temp)
	{
@ForEach{Field@
		@FieldName@ = csjp::move_cast(temp.@FieldName@);
@}@
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
@ForEach{Field@
				@FieldName@ != other.@FieldName@ ||
@ForLastField@
				@FieldName@ != other.@FieldName@
@}@
			)
			return false;
		return true;
	}

	bool isLess(const @Type@ & other) const
	{
		return
@ForEach{KeyField@
			@FieldName@ < other.@FieldName@ &&
@ForLastKeyField@
			@FieldName@ < other.@FieldName@;
@}@
	}

	bool isMore(const @Type@ & other) const
	{
		return
@ForEach{KeyField@
			other.@FieldName@ < @FieldName@ &&
@ForLastKeyField@
			other.@FieldName@ < @FieldName@;
@}@
	}

	bool isLess(
@ForEach{KeyField@
			const @FieldType@ & other@FieldName@,
@ForLastKeyField@
			const @FieldType@ & other@FieldName@) const
@}@
	{
		return
@ForEach{KeyField@
			@FieldName@ < other@FieldName@ &&
@ForLastKeyField@
			@FieldName@ < other@FieldName@;
@}@
	}

	bool isMore(
@ForEach{KeyField@
			const @FieldType@ & other@FieldName@,
@ForLastKeyField@
			const @FieldType@ & other@FieldName@) const
@}@
	{
		return
@ForEach{KeyField@
			other@FieldName@ < @FieldName@ &&
@ForLastKeyField@
			other@FieldName@ < @FieldName@;
@}@
	}

