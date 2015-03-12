inline bool operator==(const @Type@ & a, const @Type@ & b)
{
	return a.isEqual(b);
}

inline bool operator!=(const @Type@ & a, const @Type@ & b)
{
	return !a.isEqual(b);
}

inline bool operator<(const @Type@ & a, const @Type@ & b)
{
	return a.isLess(b);
}

@IfSingleKey{@
inline bool operator<(
@ForEach{KeyField@
		const @FieldType@ & a,
@}@
		const @Type@ & b)
{
	return b.isMore(a);
}

inline bool operator<(
		const @Type@ & a, 
@ForEach{KeyField@
		const @FieldType@ & b)
@}@
{
	return a.isLess(b);
}
@IfSingleKey}@
