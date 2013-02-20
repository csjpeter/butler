/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_WARE_H
#define BUTLER_WARE_H

#include <QString>

#include <csjp_owner_container.h>

#include <butler_tag_set.h>

class CategoryNameSet : public csjp::OwnerContainer<QString>
{
public:
	CategoryNameSet() : csjp::OwnerContainer<QString>() {}
	CategoryNameSet(CategoryNameSet &cns) : csjp::OwnerContainer<QString>(cns) {}
	~CategoryNameSet() {}
};

class Ware
{
public:
	Ware();
	explicit Ware(const QString &);
	explicit Ware(const Ware &);
	~Ware();
	
	Ware& operator=(const Ware &);

	bool isEqual(const Ware &) const;

	bool isLess(const Ware &) const;
	
	bool isLess(const QString &) const;
	bool isMore(const QString &) const;

public:
	QString name;
	QString unit;
	CategoryNameSet categories;
	TagNameSet tags;
	
	enum Fields {
		Name = 0,
		Unit,
		Categories,
		Tags,
		NumOfFields
	};

private:
	void copy(const Ware &);
};

bool operator==(const Ware &a, const Ware &b);
bool operator!=(const Ware &a, const Ware &b);

bool operator<(const Ware &a, const Ware &b);
bool operator<(const Ware &a, const QString &b);
bool operator<(const QString &a, const Ware &b);

#endif

