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

inline bool operator<(const @KeyType@ & a, const @Type@ & b)                                 
{                                                                                            
       return b.isMore(a);                                                                   
}                                                                                            

inline bool operator<(const @Type@ & a, const @KeyType@ & b)                                 
{                                                                                            
       return a.isLess(b);                                                                   
}                                                                                            

