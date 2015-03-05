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

@IfSingleKeyBegin@
inline bool operator<(
@ForEachKeyFieldBegin@
		const @FieldType@ & a,
@ForEachKeyFieldEnd@
		const @Type@ & b)
{
	return b.isMore(a);
}

inline bool operator<(
		const @Type@ & a, 
@ForEachKeyFieldBegin@
		const @FieldType@ & b)
@ForEachKeyFieldEnd@
{
	return a.isLess(b);
}
@IfSingleKeyEnd@
