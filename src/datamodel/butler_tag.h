/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_H
#define BUTLER_TAG_H

#include <QDate>
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
	QDateTime lastModified; /* non editable */
	QString description;
	
	enum Fields {
		Name = 0,
		Description,
		LastModified,
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
