/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DEBUG_H
#define BUTLER_DEBUG_H


/* big hack :( */
void _nothing();
#define NOTHING _nothing()


#ifdef DEBUG
#ifndef DEBUG_VERBOSE_LEVEL
/* The higher value causes less debug messages. */
#define DEBUG_VERBOSE_LEVEL 1
#endif
#define vDebug(v, ...) (DEBUG_VERBOSE_LEVEL <= v) ? \
			qDebug(__VA_ARGS__) : NOTHING
#else
#define vDebug(v, ...) ;
#endif
#define v1Debug(...) vDebug(1, __VA_ARGS__)
#define v2Debug(...) vDebug(2, __VA_ARGS__)
#define v3Debug(...) vDebug(3, __VA_ARGS__)
#define v4Debug(...) vDebug(4, __VA_ARGS__)
#define v5Debug(...) vDebug(5, __VA_ARGS__)



#ifndef DEBUG
#define NO_RUNTIME_BACKTRACE
#endif

#ifndef RUNTIME_BACKTRACE_VERBOSE_LEVEL
/* The higher value causes less debug messages. */
#define RUNTIME_BACKTRACE_VERBOSE_LEVEL 1
#endif



#ifdef NO_COLOR
#define VT_GRAPHICS_MODE(code) ""
#else
#define VT_GRAPHICS_MODE(code) "\e\["#code"m"
#endif

#define VT_TA_ALLOFF        VT_GRAPHICS_MODE(0)
#define VT_TA_BOLD          VT_GRAPHICS_MODE(1)
#define VT_TA_UNDERSCORE    VT_GRAPHICS_MODE(4)
#define VT_TA_BLINK         VT_GRAPHICS_MODE(5)
#define VT_TA_REVERSE       VT_GRAPHICS_MODE(7)
#define VT_TA_CONCEALED     VT_GRAPHICS_MODE(8)

#define VT_FG_BLACK         VT_GRAPHICS_MODE(30)
#define VT_FG_RED           VT_GRAPHICS_MODE(31)
#define VT_FG_GREEN         VT_GRAPHICS_MODE(32)
#define VT_FG_YELLOW        VT_GRAPHICS_MODE(33)
#define VT_FG_BLUE          VT_GRAPHICS_MODE(34)
#define VT_FG_MAGENTA       VT_GRAPHICS_MODE(35)
#define VT_FG_CYAN          VT_GRAPHICS_MODE(36)
#define VT_FG_WHITE         VT_GRAPHICS_MODE(37)

#define VT_BG_BLACK         VT_GRAPHICS_MODE(40)
#define VT_BG_RED           VT_GRAPHICS_MODE(41)
#define VT_BG_GREEN         VT_GRAPHICS_MODE(42)
#define VT_BG_YELLOW        VT_GRAPHICS_MODE(43)
#define VT_BG_BLUE          VT_GRAPHICS_MODE(44)
#define VT_BG_MAGENTA       VT_GRAPHICS_MODE(45)
#define VT_BG_CYAN          VT_GRAPHICS_MODE(46)
#define VT_BG_WHITE         VT_GRAPHICS_MODE(47)




#ifndef NO_RUNTIME_BACKTRACE
void _runtimeBacktraceLeaveConstructor(
		void *ptr, const char *funcName);
void _runtimeBacktraceLeaveDestructor(
		void *ptr, const char *funcName);
void _reportLeakSuspections();
void _runtimeBacktraceEnter(
		void *ptr, const char *funcName, int verbose = __INT_MAX__);
void _runtimeBacktraceLeave(
		void *ptr, const char *funcName, int verbose = __INT_MAX__);
#define ENTER_STATIC_FUNCTION() \
	_runtimeBacktraceEnter((void*)(0), __PRETTY_FUNCTION__)
#define LEAVE_STATIC_FUNCTION() \
	_runtimeBacktraceLeave((void*)(0),  __PRETTY_FUNCTION__)
#define ENTER_FUNCTION() \
	_runtimeBacktraceEnter((void*)(this), __PRETTY_FUNCTION__)
#define LEAVE_FUNCTION() \
	_runtimeBacktraceLeave((void*)(this),  __PRETTY_FUNCTION__)
#define ENTER_STATIC_FUNCTION_VERBOSE(v) \
	(RUNTIME_BACKTRACE_VERBOSE_LEVEL <= v) ? \
        _runtimeBacktraceEnter((void*)(0), __PRETTY_FUNCTION__, v) : \
	NOTHING ;
#define LEAVE_STATIC_FUNCTION_VERBOSE(v) \
	(RUNTIME_BACKTRACE_VERBOSE_LEVEL <= v) ? \
        _runtimeBacktraceLeave((void*)(0),  __PRETTY_FUNCTION__, v) : \
	NOTHING ;
#define ENTER_FUNCTION_VERBOSE(v) \
	(RUNTIME_BACKTRACE_VERBOSE_LEVEL <= v) ? \
        _runtimeBacktraceEnter((void*)(this), __PRETTY_FUNCTION__, v) : \
	NOTHING ;
#define LEAVE_FUNCTION_VERBOSE(v) \
	(RUNTIME_BACKTRACE_VERBOSE_LEVEL <= v) ? \
        _runtimeBacktraceLeave((void*)(this),  __PRETTY_FUNCTION__, v) : \
	NOTHING ;
#define ENTER_CONSTRUCTOR() \
	ENTER_FUNCTION()
#define LEAVE_CONSTRUCTOR() LEAVE_FUNCTION(); \
	_runtimeBacktraceLeaveConstructor((void*)(this), __PRETTY_FUNCTION__)
#define ENTER_DESTRUCTOR() \
	ENTER_FUNCTION()
#define LEAVE_DESTRUCTOR() LEAVE_FUNCTION(); \
	_runtimeBacktraceLeaveDestructor((void*)(this), __PRETTY_FUNCTION__)
#else
#define ENTER_STATIC_FUNCTION() ;
#define LEAVE_STATIC_FUNCTION() ;
#define ENTER_FUNCTION() ;
#define LEAVE_FUNCTION() ;
#define ENTER_STATIC_FUNCTION_VERBOSE(v) NOTHING
#define LEAVE_STATIC_FUNCTION_VERBOSE(v) NOTHING
#define ENTER_FUNCTION_VERBOSE(v) NOTHING
#define LEAVE_FUNCTION_VERBOSE(v) NOTHING
#define ENTER_CONSTRUCTOR() ;
#define LEAVE_CONSTRUCTOR() ;
#define ENTER_DESTRUCTOR() ;
#define LEAVE_DESTRUCTOR() ;
#endif


#endif

