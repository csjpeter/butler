/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_H
#define BUTLER_TAG_H

#include <QString>

class Tag
{
public:
	Tag();
	explicit Tag(const QString &_name);
	explicit Tag(const Tag &tag);
	~Tag();

	Tag& operator=(const Tag& tag);

	bool isEqual(const Tag &t) const;

	bool isLess(const Tag &t) const;
	
	bool isLess(const QString &s) const;
	bool isMore(const QString &s) const;

public:
	QString name;
	QString description;
	
	enum Fields {
		Name = 0,
		Description,
		NumOfFields
	};

private:
	void equal(const Tag &tag);
};

bool operator==(const Tag &a, const Tag &b);

bool operator!=(const Tag &a, const Tag &b);

bool operator<(const Tag &a, const Tag &b);
bool operator<(const QString &a, const Tag &b);
bool operator<(const Tag &a, const QString &b);

#endif

