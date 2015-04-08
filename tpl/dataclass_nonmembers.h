inline bool operator==(const @Type@ & a, const @Type@ & b) { return a.isEqual(b); }
inline bool operator!=(const @Type@ & a, const @Type@ & b) { return !a.isEqual(b); }
inline bool operator<(const @Type@ & a, const @Type@ & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const @Type@ & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

@IfSingleKey{@
inline bool operator<(@For{KeyField@const @.Type@ & a,@}@ const @Type@ & b) { return b.isMore(a); }
inline bool operator<(const @Type@ & a, @For{KeyField@const @.Type@ & b)@}@ { return a.isLess(b); }
@IfSingleKey}@
