public:

@For{Field@
	@.Type@ @.Name@;@.Comment@
@}@

	enum Fields {
@For{Field@
		@.EnumName@,
@}@
		NumOfFields
	};

public:
	@Type@() {}

	explicit @Type@(@For{KeyField@const @.Type@ & @.Name@, @-@const @.Type@ & @.Name@@}@) :
@For{KeyField@
		@.Name@(@.Name@),
@-@
		@.Name@(@.Name@)
@}@
	{}

	explicit @Type@(const @Type@ & orig) :
@For{Field@
		@.Name@(orig.@.Name@),
@-@
		@.Name@(orig.@.Name@)
@}@
	{}

	@Type@(@Type@ && temp) :
@For{Field@
		@.Name@(csjp::move_cast(temp.@.Name@)),
@-@
		@.Name@(csjp::move_cast(temp.@.Name@))
@}@
	{}

	~@Type@() {}

	@Type@& operator=(@Type@ && temp)
	{
@For{Field@
		@.Name@ = csjp::move_cast(temp.@.Name@);
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
				@.Name@ != other.@.Name@ ||
@-@
				@.Name@ != other.@.Name@
@}@
			)
			return false;
		return true;
	}

	bool isLess(const @Type@ & other) const
	{
		return @For{KeyField@@.Name@ < other.@.Name@ && @-@@.Name@ < other.@.Name@;@}@
	}

	bool isMore(const @Type@ & other) const
	{
		return @For{KeyField@other.@.Name@ < @.Name@ &&@-@other.@.Name@ < @.Name@;@}@
	}

	bool isLess(@For{KeyField@const @.Type@ & other@.Name@,@-@const @.Type@ & other@.Name@@}@) const
	{
		return @For{KeyField@@.Name@ < other@.Name@ &&@-@@.Name@ < other@.Name@;@}@
	}

	bool isMore(@For{KeyField@const @.Type@ & other@.Name@,@-@const @.Type@ & other@.Name@@}@) const
	{
		return @For{KeyField@other@.Name@ < @.Name@ &&@-@other@.Name@ < @.Name@;@}@
	}

