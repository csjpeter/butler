public:

@For{Field@
	@FieldType@ @FieldName@;@FieldComment@
@}@

	enum Fields {
@For{Field@
		@FieldEnumName@,
@}@
		NumOfFields
	};

public:
	@Type@() {}

	explicit @Type@(@For{KeyField@const @FieldType@ & @FieldName@, @Last@const @FieldType@ & @FieldName@@}@) :
@For{KeyField@
		@FieldName@(@FieldName@),
@Last@
		@FieldName@(@FieldName@)
@}@
	{}

	explicit @Type@(const @Type@ & orig) :
@For{Field@
		@FieldName@(orig.@FieldName@),
@Last@
		@FieldName@(orig.@FieldName@)
@}@
	{}

	@Type@(@Type@ && temp) :
@For{Field@
		@FieldName@(csjp::move_cast(temp.@FieldName@)),
@Last@
		@FieldName@(csjp::move_cast(temp.@FieldName@))
@}@
	{}

	~@Type@() {}

	@Type@& operator=(@Type@ && temp)
	{
@For{Field@
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
@For{Field@
				@FieldName@ != other.@FieldName@ ||
@Last@
				@FieldName@ != other.@FieldName@
@}@
			)
			return false;
		return true;
	}

	bool isLess(const @Type@ & other) const
	{
		return
@For{KeyField@
			@FieldName@ < other.@FieldName@ &&
@Last@
			@FieldName@ < other.@FieldName@;
@}@
	}

	bool isMore(const @Type@ & other) const
	{
		return
@For{KeyField@
			other.@FieldName@ < @FieldName@ &&
@Last@
			other.@FieldName@ < @FieldName@;
@}@
	}

	bool isLess(
@For{KeyField@
			const @FieldType@ & other@FieldName@,
@Last@
			const @FieldType@ & other@FieldName@) const
@}@
	{
		return
@For{KeyField@
			@FieldName@ < other@FieldName@ &&
@Last@
			@FieldName@ < other@FieldName@;
@}@
	}

	bool isMore(
@For{KeyField@
			const @FieldType@ & other@FieldName@,
@Last@
			const @FieldType@ & other@FieldName@) const
@}@
	{
		return
@For{KeyField@
			other@FieldName@ < @FieldName@ &&
@Last@
			other@FieldName@ < @FieldName@;
@}@
	}

