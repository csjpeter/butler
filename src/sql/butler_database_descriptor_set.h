/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_DATABASE_DESCRIPTOR_SET_H
#define BUTLER_DATABASE_DESCRIPTOR_SET_H

#include <QString>

#include <butler_item.h>
#include <csjp_sorter_owner_container.h>

class DatabaseDescriptorSet : public csjp::SorterOwnerContainer<DatabaseDescriptor>
{
public:
	DatabaseDescriptorSet() :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(),
		ordering(DatabaseDescriptor::Name),
		ascending(true){}
	DatabaseDescriptorSet(const DatabaseDescriptorSet &is) :
		csjp::SorterOwnerContainer<DatabaseDescriptor>(is),
		ordering(DatabaseDescriptor::Name),
		ascending(true){}
	~DatabaseDescriptorSet() {}

	DatabaseDescriptor& query(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::query<QString>(str);}

	bool has(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::has<QString>(str);}

	unsigned index(const QString & str) const {
		return csjp::SorterOwnerContainer<DatabaseDescriptor>::index<QString>(str);}

	virtual int compare(const DatabaseDescriptor &a, const DatabaseDescriptor &b) const;

	DatabaseDescriptor::Fields ordering;
	bool ascending;
};

#endif
