
#define PRIVATE_COLOR blue
#define FOREIGN_COLOR orange
#define ADD_COLOR green
#define REMOVE_COLOR red

#define DEFAULTS \
	compound=true; \
	concentrate=false; \
	rankdir=BT; \
	nojustify=true; \
	labeljust=c \
	fontname=Times \
	fontsize=14 \
	node [fontname="Times", fontsize=12, style=solid]; \
	edge [fontname=Courier, labelfontsize=12, dir=both]

#define LAYER(layerlist) node [layer="layerlist"];

/* feature, etc to be remove */
#define REMOVE node [color=REMOVE_COLOR];
/* feature, etc not yet implemented */
#define ADD node [color=ADD_COLOR];
/* not part of the api of the relevant package */
#define PRIVATE node [fontcolor=PRIVATE_COLOR, color=PRIVATE_COLOR]; edge [fontcolor=PRIVATE_COLOR, color=PRIVATE_COLOR]; fontcolor=PRIVATE_COLOR; color=PRIVATE_COLOR;
/* used components provided by some other package */
#define FOREIGN node [fontcolor=FOREIGN_COLOR, color=FOREIGN_COLOR]; fontcolor=FOREIGN_COLOR; color=FOREIGN_COLOR;

/*
	Class diagrams
*/

/* any class not matching the ones below */
#define CLASS node [shape=record];
/* class that has type parameter */
#define TEMPLATE node [shape=record];
/* has no language contraints but similar like type parameterized interface */
#define CONCEPT node [shape=record, style=rounded];
/* fully abstract class having no defined method nor data member */
#define INTERFACE node [shape=record, style=rounded];
/* class having at least one undefined method */
#define ABSTRACT node [shape=record, style=rounded];

/* bidirectional: both actor knows about the other */
#define ASSOCIATION edge [arrowhead=none, arrowtail=none];
/* unidirectional: only 'one' knows about 'other'; the direction of the arrow points on 'other', known by 'one' */
#define KNOWLEDGE edge [arrowhead=ovee, arrowtail=none];
/* unidirectional: only child knows about parent */
#define INHERITANCE edge [arrowhead=onormal, arrowtail=none];
/* unidirectional: only implementor knows about the interface implemented */
#define IMPLEMENTATION edge [arrowhead=onormal, arrowtail=none, style=dashed];
/* unidirectional: only conforming one knows about the concept to conform to */
#define CONFORMANCE edge [arrowhead=onormal, arrowtail=none, style=dotted];
/* unidirectional: only whole knows about part; lifecycle of part not depends on whole */
#define AGGREGATION edge [arrowhead=ovee, arrowtail=odiamond];
/* unidirectional: only whole knows about part; lifecycle of part is determined by lifecycle of whole */
#define COMPOSITION edge [arrowhead=ovee, arrowtail=diamond];
/* unidirectional: only user knows about the used one; some operation on user will invoke operation on the used one */
#define DEPENDENCY edge [arrowhead=ovee, arrowtail=none, style=dashed];

/*
	Other diagrams
*/

#define DEPLOYED node [shape=box3d];
#define PACKAGE node [shape=tab];
#define COMPONENT node [shape=component];
#define NOSHAPE node [shape=none];

/* unidirectional: only package knows about what class it contains */
#define CONTAINMENT edge [arrowhead=none, arrowtail=odot];

/*
	Special node contents
*/

#define SQL_HTML(ID, def) ID \
	[shape=plaintext, style=solid, label=< \
	<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0"> \
	def \
	</TABLE> \
	>];
#define SQL_HTML_NAME(name) <TR><TD ALIGN="center" PORT="table">name</TD></TR><TR><TD></TD></TR>
#define HTML_ATTR(port, desc) <TR><TD ALIGN="left" PORT="port">desc</TD></TR>
#define ATTR_ADD(desc) <FONT COLOR="ADD_COLOR">desc</FONT>
#define ATTR_REMOVE(desc) <FONT COLOR="REMOVE_COLOR">desc</FONT>


#define CLASS_HTML(ID, def) ID \
	[shape=plaintext, style=solid, label=< \
	<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0"> \
	def \
	</TABLE> \
	>];
#define CLASS_HTML_NAME(name) <TR><TD ALIGN="center" PORT="object">name</TD></TR><TR><TD></TD></TR>
#define CLASS_HTML_METHOD(port, desc) <TR><TD ALIGN="left" PORT="port">desc()\n2nd line</TD></TR>
#define CLASS_HTML_MEMBER(port, desc) <TR><TD ALIGN="left" PORT="port">desc</TD></TR>

