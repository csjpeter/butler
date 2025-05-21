/**
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2015 Csaszar, Peter
 */

#ifndef BUTLER_DATACLASSES_H
#define BUTLER_DATACLASSES_H

#include <math.h>
#include <float.h>

#include <csjp_string.h>
#include <csjp_object.h>
#include <csjp_pod_array.h>
#include <csjp_array.h>
#include <csjp_json.h>
#include <csjp_owner_container.h>
#include <csjp_sorter_owner_container.h>

#include <butler_datetime.h>
#include <butler_text.h>

#include <butler_sql_connection.h>


typedef csjp::OwnerContainer<Text> StringSet;

class ItemQueryStat{
public:
	ItemQueryStat() : cheapestUnitPrice(DBL_MAX) {}
	UInt itemCount;
	Double sumQuantity;
	Double sumPrice;
	Double avgPrice;
	Double cheapestUnitPrice;
	Double mostExpUnitPrice;
	Double queryTime; /** How much time the query took. */
};

class PaymentQueryStat{
public:
	PaymentQueryStat() : cheapestPrice(DBL_MAX) {}
	UInt paymentCount;
	Double sumPrice;
	Double avgPrice;
	Double cheapestPrice;
	Double mostExpPrice;
	Double queryTime; /** How much time the query took. */
};






class Tag{
	public:

	Text name;
	Text description;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Description,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Tag() {}

	explicit Tag(const Text & name) :
		name(name)
	{}

	explicit Tag(const Tag & orig) :
		name(orig.name),
		description(orig.description),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Tag(Tag && temp) :
		name(csjp::move_cast(temp.name)),
		description(csjp::move_cast(temp.description)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Tag() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= description;
		json["description"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Tag& operator=(Tag && temp)
	{
		name = csjp::move_cast(temp.name);
		description = csjp::move_cast(temp.description);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Tag& operator=(const Tag & orig)
	{
		Tag copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Tag & other) const
	{
		if(
				name != other.name ||
				description != other.description ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Tag & other) const
	{
		return name < other.name;	}

	bool isMore(const Tag & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Tag fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Tag & orig) const;

};
inline bool operator==(const Tag & a, const Tag & b) { return a.isEqual(b); }
inline bool operator!=(const Tag & a, const Tag & b) { return !a.isEqual(b); }
inline bool operator<(const Tag & a, const Tag & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Tag & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Tag & b) { return b.isMore(a); }
inline bool operator<(const Tag & a, const Text & b) { return a.isLess(b); }

class TagSet : public csjp::SorterOwnerContainer<Tag>
{
	public:
	csjp::PodArray<Tag::Fields> ordering;
	bool ascending;

	const TagSet & operator=(TagSet && temp)
	{
		csjp::SorterOwnerContainer<Tag>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Tag & a, const Tag & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Tag::Name :
				ret = a.name < b.name;
				break;
			case Tag::Description :
				ret = a.description < b.description;
				break;
			case Tag::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Tag::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	TagSet() :
		csjp::SorterOwnerContainer<Tag>(),
		ascending(true)
	{
		ordering.add(Tag::Name);
		ordering.add(Tag::Description);
		ordering.add(Tag::LastModified);
		ordering.add(Tag::Deleted);
	}
	TagSet(const TagSet & ts) :
		csjp::SorterOwnerContainer<Tag>(ts),
		ascending(true)
	{
		ordering.add(Tag::Name);
		ordering.add(Tag::Description);
		ordering.add(Tag::LastModified);
		ordering.add(Tag::Deleted);
	}
	~TagSet() {}


	
	static TagSet fromDb(SqlConnection & sql);

};

class WareType{
	public:

	Text ware;
	Text type;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Ware,
		Type,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	WareType() {}

	explicit WareType(const Text & ware, const Text & type) :
		ware(ware),
		type(type)
	{}

	explicit WareType(const WareType & orig) :
		ware(orig.ware),
		type(orig.type),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	WareType(WareType && temp) :
		ware(csjp::move_cast(temp.ware)),
		type(csjp::move_cast(temp.type)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~WareType() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= ware;
		json["ware"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= type;
		json["type"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	WareType& operator=(WareType && temp)
	{
		ware = csjp::move_cast(temp.ware);
		type = csjp::move_cast(temp.type);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	WareType& operator=(const WareType & orig)
	{
		WareType copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const WareType & other) const
	{
		if(
				ware != other.ware ||
				type != other.type ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const WareType & other) const
	{
		return ware < other.ware && type < other.type;	}

	bool isMore(const WareType & other) const
	{
		return other.ware < ware &&other.type < type;	}

	bool isLess(const Text & otherware,const Text & othertype) const
	{
		return ware < otherware &&type < othertype;	}

	bool isMore(const Text & otherware,const Text & othertype) const
	{
		return otherware < ware &&othertype < type;	}


	
	static WareType fromDb(SqlConnection & sql, const Text & _ware, const Text & _type);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const WareType & orig) const;

};
inline bool operator==(const WareType & a, const WareType & b) { return a.isEqual(b); }
inline bool operator!=(const WareType & a, const WareType & b) { return !a.isEqual(b); }
inline bool operator<(const WareType & a, const WareType & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const WareType & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class WareTypeSet : public csjp::SorterOwnerContainer<WareType>
{
	public:
	csjp::PodArray<WareType::Fields> ordering;
	bool ascending;

	const WareTypeSet & operator=(WareTypeSet && temp)
	{
		csjp::SorterOwnerContainer<WareType>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const WareType & a, const WareType & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case WareType::Ware :
				ret = a.ware < b.ware;
				break;
			case WareType::Type :
				ret = a.type < b.type;
				break;
			case WareType::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case WareType::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.ware < b.ware &&
					a.type < b.type;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	WareTypeSet() :
		csjp::SorterOwnerContainer<WareType>(),
		ascending(true)
	{
		ordering.add(WareType::Ware);
		ordering.add(WareType::Type);
		ordering.add(WareType::LastModified);
		ordering.add(WareType::Deleted);
	}
	WareTypeSet(const WareTypeSet & ts) :
		csjp::SorterOwnerContainer<WareType>(ts),
		ascending(true)
	{
		ordering.add(WareType::Ware);
		ordering.add(WareType::Type);
		ordering.add(WareType::LastModified);
		ordering.add(WareType::Deleted);
	}
	~WareTypeSet() {}


	
	static WareTypeSet fromDb(SqlConnection & sql, const Text & _ware);

};

class WareTag{
	public:

	Text ware;
	Text tag;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Ware,
		Tag,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	WareTag() {}

	explicit WareTag(const Text & ware, const Text & tag) :
		ware(ware),
		tag(tag)
	{}

	explicit WareTag(const WareTag & orig) :
		ware(orig.ware),
		tag(orig.tag),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	WareTag(WareTag && temp) :
		ware(csjp::move_cast(temp.ware)),
		tag(csjp::move_cast(temp.tag)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~WareTag() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= ware;
		json["ware"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= tag;
		json["tag"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	WareTag& operator=(WareTag && temp)
	{
		ware = csjp::move_cast(temp.ware);
		tag = csjp::move_cast(temp.tag);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	WareTag& operator=(const WareTag & orig)
	{
		WareTag copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const WareTag & other) const
	{
		if(
				ware != other.ware ||
				tag != other.tag ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const WareTag & other) const
	{
		return ware < other.ware && tag < other.tag;	}

	bool isMore(const WareTag & other) const
	{
		return other.ware < ware &&other.tag < tag;	}

	bool isLess(const Text & otherware,const Text & othertag) const
	{
		return ware < otherware &&tag < othertag;	}

	bool isMore(const Text & otherware,const Text & othertag) const
	{
		return otherware < ware &&othertag < tag;	}


	
	static WareTag fromDb(SqlConnection & sql, const Text & _ware, const Text & _tag);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const WareTag & orig) const;

};
inline bool operator==(const WareTag & a, const WareTag & b) { return a.isEqual(b); }
inline bool operator!=(const WareTag & a, const WareTag & b) { return !a.isEqual(b); }
inline bool operator<(const WareTag & a, const WareTag & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const WareTag & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class WareTagSet : public csjp::SorterOwnerContainer<WareTag>
{
	public:
	csjp::PodArray<WareTag::Fields> ordering;
	bool ascending;

	const WareTagSet & operator=(WareTagSet && temp)
	{
		csjp::SorterOwnerContainer<WareTag>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const WareTag & a, const WareTag & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case WareTag::Ware :
				ret = a.ware < b.ware;
				break;
			case WareTag::Tag :
				ret = a.tag < b.tag;
				break;
			case WareTag::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case WareTag::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.ware < b.ware &&
					a.tag < b.tag;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	WareTagSet() :
		csjp::SorterOwnerContainer<WareTag>(),
		ascending(true)
	{
		ordering.add(WareTag::Ware);
		ordering.add(WareTag::Tag);
		ordering.add(WareTag::LastModified);
		ordering.add(WareTag::Deleted);
	}
	WareTagSet(const WareTagSet & ts) :
		csjp::SorterOwnerContainer<WareTag>(ts),
		ascending(true)
	{
		ordering.add(WareTag::Ware);
		ordering.add(WareTag::Tag);
		ordering.add(WareTag::LastModified);
		ordering.add(WareTag::Deleted);
	}
	~WareTagSet() {}


	
	static WareTagSet fromDb(SqlConnection & sql, const Text & _ware);

};


inline bool operator<(const Text & a, const WareType & b) { return b.type.isMore(a); }
inline bool operator<(const WareType & a, const Text & b) { return a.type.isLess(b); }
inline bool operator<(const Text & a, const WareTag & b) { return b.tag.isMore(a); }
inline bool operator<(const WareTag & a, const Text & b) { return a.tag.isLess(b); }


class Ware
{
	public:

	Text name;
	Text unit;
	Text icon;
	WareTypeSet types;
	WareTagSet tags;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Unit,
		Icon,
		Types,
		Tags,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Ware() {}

	explicit Ware(const Text & name) :
		name(name)
	{}

	explicit Ware(const Ware & orig) :
		name(orig.name),
		unit(orig.unit),
		icon(orig.icon),
		types(orig.types),
		tags(orig.tags),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Ware(Ware && temp) :
		name(csjp::move_cast(temp.name)),
		unit(csjp::move_cast(temp.unit)),
		icon(csjp::move_cast(temp.icon)),
		types(csjp::move_cast(temp.types)),
		tags(csjp::move_cast(temp.tags)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Ware() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= unit;
		json["unit"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= icon;
		json["icon"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Ware& operator=(Ware && temp)
	{
		name = csjp::move_cast(temp.name);
		unit = csjp::move_cast(temp.unit);
		icon = csjp::move_cast(temp.icon);
		types = csjp::move_cast(temp.types);
		tags = csjp::move_cast(temp.tags);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Ware& operator=(const Ware & orig)
	{
		Ware copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Ware & other) const
	{
		if(
				name != other.name ||
				unit != other.unit ||
				icon != other.icon ||
				types != other.types ||
				tags != other.tags ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Ware & other) const
	{
		return name < other.name;	}

	bool isMore(const Ware & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Ware fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Ware & orig) const;


	/* non-transactional */
	void setAsTypes(const QString & str)
	{
		QStringList sl;
		sl = str.split(",", Qt::SkipEmptyParts);
		for(auto & s : sl)
			s = s.trimmed();
		for(auto & type : types)
			if(!sl.contains(type.type))
				const_cast<WareType &>(type).deleted = true;
		for(auto & s : sl){
			Text type(s);
			if(!types.has(type))
				types.add(new WareType(name, type));
		}
	}
#if 0
	/* non-transactional */
	void setAsTypes(const QStringList & strings)
	{
		for(auto & type : types)
			if(!strings.contains(type.type))
				const_cast<WareType &>(type).deleted = true;
		for(auto & string : strings){
			Text type(string);
			if(!types.has(type))
				types.add(new WareType(name, type));
		}
	}

	/* non-transactional */
	void setAsTags(const QString & str)
	{
		QStringList sl;
		sl = str.split(",", QString::SkipEmptyParts);
		for(auto & s :sl)
			s = s.trimmed();
		for(auto & tag : tags)
			if(!sl.contains(tag.tag))
				const_cast<WareTag &>(tag).deleted = true;
		for(auto & s : sl){
			Text tag(s);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}

	/* non-transactional */
	void setAsTags(const QStringList & strings)
	{
		for(auto & tag : tags)
			if(!strings.contains(tag.tag))
				const_cast<WareTag &>(tag).deleted = true;
		for(auto & string : strings){
			Text tag(string);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}
#endif
	/* non-transactional */
	void setAsTags(const StringSet & stringSet)
	{
		for(auto & tag : tags)
			if(!stringSet.has(tag.tag))
				const_cast<WareTag &>(tag).deleted = true;
		for(auto & string : stringSet){
			Text tag(string);
			if(!tags.has(tag))
				tags.add(new WareTag(name, tag));
		}
	}

};
inline bool operator==(const Ware & a, const Ware & b) { return a.isEqual(b); }
inline bool operator!=(const Ware & a, const Ware & b) { return !a.isEqual(b); }
inline bool operator<(const Ware & a, const Ware & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Ware & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Ware & b) { return b.isMore(a); }
inline bool operator<(const Ware & a, const Text & b) { return a.isLess(b); }

class WareSet : public csjp::SorterOwnerContainer<Ware>
{
	public:
	csjp::PodArray<Ware::Fields> ordering;
	bool ascending;

	const WareSet & operator=(WareSet && temp)
	{
		csjp::SorterOwnerContainer<Ware>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Ware & a, const Ware & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Ware::Name :
				ret = a.name < b.name;
				break;
			case Ware::Unit :
				ret = a.unit < b.unit;
				break;
			case Ware::Icon :
				ret = a.icon < b.icon;
				break;
			case Ware::Types :
				ret = a.types < b.types;
				break;
			case Ware::Tags :
				ret = a.tags < b.tags;
				break;
			case Ware::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Ware::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	WareSet() :
		csjp::SorterOwnerContainer<Ware>(),
		ascending(true)
	{
		ordering.add(Ware::Name);
		ordering.add(Ware::Unit);
		ordering.add(Ware::Icon);
		ordering.add(Ware::Types);
		ordering.add(Ware::Tags);
		ordering.add(Ware::LastModified);
		ordering.add(Ware::Deleted);
	}
	WareSet(const WareSet & ts) :
		csjp::SorterOwnerContainer<Ware>(ts),
		ascending(true)
	{
		ordering.add(Ware::Name);
		ordering.add(Ware::Unit);
		ordering.add(Ware::Icon);
		ordering.add(Ware::Types);
		ordering.add(Ware::Tags);
		ordering.add(Ware::LastModified);
		ordering.add(Ware::Deleted);
	}
	~WareSet() {}


	
	static WareSet fromDb(SqlConnection & sql);

};



class Company{
	public:

	Text name;	// Tesco Global Áruházak
	Text country;	// Magyarország
	Text city;	// Budaörs
	Text postalCode;	// 2040
	Text address;	// Kinizsi út 1-3.
	Text taxId;	// 10307078-2-44
	Text icon;	// base64 repr of an image
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Country,
		City,
		PostalCode,
		Address,
		TaxId,
		Icon,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Company() {}

	explicit Company(const Text & name) :
		name(name)
	{}

	explicit Company(const Company & orig) :
		name(orig.name),
		country(orig.country),
		city(orig.city),
		postalCode(orig.postalCode),
		address(orig.address),
		taxId(orig.taxId),
		icon(orig.icon),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Company(Company && temp) :
		name(csjp::move_cast(temp.name)),
		country(csjp::move_cast(temp.country)),
		city(csjp::move_cast(temp.city)),
		postalCode(csjp::move_cast(temp.postalCode)),
		address(csjp::move_cast(temp.address)),
		taxId(csjp::move_cast(temp.taxId)),
		icon(csjp::move_cast(temp.icon)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Company() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= country;
		json["country"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= city;
		json["city"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= postalCode;
		json["postalCode"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= address;
		json["address"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= taxId;
		json["taxId"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= icon;
		json["icon"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Company& operator=(Company && temp)
	{
		name = csjp::move_cast(temp.name);
		country = csjp::move_cast(temp.country);
		city = csjp::move_cast(temp.city);
		postalCode = csjp::move_cast(temp.postalCode);
		address = csjp::move_cast(temp.address);
		taxId = csjp::move_cast(temp.taxId);
		icon = csjp::move_cast(temp.icon);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Company& operator=(const Company & orig)
	{
		Company copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Company & other) const
	{
		if(
				name != other.name ||
				country != other.country ||
				city != other.city ||
				postalCode != other.postalCode ||
				address != other.address ||
				taxId != other.taxId ||
				icon != other.icon ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Company & other) const
	{
		return name < other.name;	}

	bool isMore(const Company & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Company fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Company & orig) const;

};
inline bool operator==(const Company & a, const Company & b) { return a.isEqual(b); }
inline bool operator!=(const Company & a, const Company & b) { return !a.isEqual(b); }
inline bool operator<(const Company & a, const Company & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Company & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Company & b) { return b.isMore(a); }
inline bool operator<(const Company & a, const Text & b) { return a.isLess(b); }

class CompanySet : public csjp::SorterOwnerContainer<Company>
{
	public:
	csjp::PodArray<Company::Fields> ordering;
	bool ascending;

	const CompanySet & operator=(CompanySet && temp)
	{
		csjp::SorterOwnerContainer<Company>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Company & a, const Company & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Company::Name :
				ret = a.name < b.name;
				break;
			case Company::Country :
				ret = a.country < b.country;
				break;
			case Company::City :
				ret = a.city < b.city;
				break;
			case Company::PostalCode :
				ret = a.postalCode < b.postalCode;
				break;
			case Company::Address :
				ret = a.address < b.address;
				break;
			case Company::TaxId :
				ret = a.taxId < b.taxId;
				break;
			case Company::Icon :
				ret = a.icon < b.icon;
				break;
			case Company::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Company::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	CompanySet() :
		csjp::SorterOwnerContainer<Company>(),
		ascending(true)
	{
		ordering.add(Company::Name);
		ordering.add(Company::Country);
		ordering.add(Company::City);
		ordering.add(Company::PostalCode);
		ordering.add(Company::Address);
		ordering.add(Company::TaxId);
		ordering.add(Company::Icon);
		ordering.add(Company::LastModified);
		ordering.add(Company::Deleted);
	}
	CompanySet(const CompanySet & ts) :
		csjp::SorterOwnerContainer<Company>(ts),
		ascending(true)
	{
		ordering.add(Company::Name);
		ordering.add(Company::Country);
		ordering.add(Company::City);
		ordering.add(Company::PostalCode);
		ordering.add(Company::Address);
		ordering.add(Company::TaxId);
		ordering.add(Company::Icon);
		ordering.add(Company::LastModified);
		ordering.add(Company::Deleted);
	}
	~CompanySet() {}


	
	static CompanySet fromDb(SqlConnection & sql);

};

class Brand{
	public:

	Text name;
	Text company;
	Text icon;	// base64 repr of an image
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Company,
		Icon,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Brand() {}

	explicit Brand(const Text & name) :
		name(name)
	{}

	explicit Brand(const Brand & orig) :
		name(orig.name),
		company(orig.company),
		icon(orig.icon),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Brand(Brand && temp) :
		name(csjp::move_cast(temp.name)),
		company(csjp::move_cast(temp.company)),
		icon(csjp::move_cast(temp.icon)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Brand() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= company;
		json["company"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= icon;
		json["icon"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Brand& operator=(Brand && temp)
	{
		name = csjp::move_cast(temp.name);
		company = csjp::move_cast(temp.company);
		icon = csjp::move_cast(temp.icon);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Brand& operator=(const Brand & orig)
	{
		Brand copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Brand & other) const
	{
		if(
				name != other.name ||
				company != other.company ||
				icon != other.icon ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Brand & other) const
	{
		return name < other.name;	}

	bool isMore(const Brand & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Brand fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Brand & orig) const;

};
inline bool operator==(const Brand & a, const Brand & b) { return a.isEqual(b); }
inline bool operator!=(const Brand & a, const Brand & b) { return !a.isEqual(b); }
inline bool operator<(const Brand & a, const Brand & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Brand & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Brand & b) { return b.isMore(a); }
inline bool operator<(const Brand & a, const Text & b) { return a.isLess(b); }

class BrandSet : public csjp::SorterOwnerContainer<Brand>
{
	public:
	csjp::PodArray<Brand::Fields> ordering;
	bool ascending;

	const BrandSet & operator=(BrandSet && temp)
	{
		csjp::SorterOwnerContainer<Brand>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Brand & a, const Brand & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Brand::Name :
				ret = a.name < b.name;
				break;
			case Brand::Company :
				ret = a.company < b.company;
				break;
			case Brand::Icon :
				ret = a.icon < b.icon;
				break;
			case Brand::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Brand::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	BrandSet() :
		csjp::SorterOwnerContainer<Brand>(),
		ascending(true)
	{
		ordering.add(Brand::Name);
		ordering.add(Brand::Company);
		ordering.add(Brand::Icon);
		ordering.add(Brand::LastModified);
		ordering.add(Brand::Deleted);
	}
	BrandSet(const BrandSet & ts) :
		csjp::SorterOwnerContainer<Brand>(ts),
		ascending(true)
	{
		ordering.add(Brand::Name);
		ordering.add(Brand::Company);
		ordering.add(Brand::Icon);
		ordering.add(Brand::LastModified);
		ordering.add(Brand::Deleted);
	}
	~BrandSet() {}


	
	static BrandSet fromDb(SqlConnection & sql);

};

class Inventory{
	public:

	Text name;
	Text comment;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Comment,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Inventory() {}

	explicit Inventory(const Text & name) :
		name(name)
	{}

	explicit Inventory(const Inventory & orig) :
		name(orig.name),
		comment(orig.comment),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Inventory(Inventory && temp) :
		name(csjp::move_cast(temp.name)),
		comment(csjp::move_cast(temp.comment)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Inventory() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= comment;
		json["comment"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Inventory& operator=(Inventory && temp)
	{
		name = csjp::move_cast(temp.name);
		comment = csjp::move_cast(temp.comment);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Inventory& operator=(const Inventory & orig)
	{
		Inventory copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Inventory & other) const
	{
		if(
				name != other.name ||
				comment != other.comment ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Inventory & other) const
	{
		return name < other.name;	}

	bool isMore(const Inventory & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Inventory fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Inventory & orig) const;

};
inline bool operator==(const Inventory & a, const Inventory & b) { return a.isEqual(b); }
inline bool operator!=(const Inventory & a, const Inventory & b) { return !a.isEqual(b); }
inline bool operator<(const Inventory & a, const Inventory & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Inventory & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Inventory & b) { return b.isMore(a); }
inline bool operator<(const Inventory & a, const Text & b) { return a.isLess(b); }

class InventorySet : public csjp::SorterOwnerContainer<Inventory>
{
	public:
	csjp::PodArray<Inventory::Fields> ordering;
	bool ascending;

	const InventorySet & operator=(InventorySet && temp)
	{
		csjp::SorterOwnerContainer<Inventory>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Inventory & a, const Inventory & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Inventory::Name :
				ret = a.name < b.name;
				break;
			case Inventory::Comment :
				ret = a.comment < b.comment;
				break;
			case Inventory::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Inventory::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	InventorySet() :
		csjp::SorterOwnerContainer<Inventory>(),
		ascending(true)
	{
		ordering.add(Inventory::Name);
		ordering.add(Inventory::Comment);
		ordering.add(Inventory::LastModified);
		ordering.add(Inventory::Deleted);
	}
	InventorySet(const InventorySet & ts) :
		csjp::SorterOwnerContainer<Inventory>(ts),
		ascending(true)
	{
		ordering.add(Inventory::Name);
		ordering.add(Inventory::Comment);
		ordering.add(Inventory::LastModified);
		ordering.add(Inventory::Deleted);
	}
	~InventorySet() {}


	
	static InventorySet fromDb(SqlConnection & sql);

};

class Partner{
	public:

	Text name;	// Kertvárosi tesco
	Text country;	// Magyarország
	Text city;	// Pécs
	Text postalCode;	// 7631
	Text address;	// Kincses út 1.
	Text company;	// Tesco Global Áruházak Zrt.
	Text storeName;	// 41052 számú bolt
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Country,
		City,
		PostalCode,
		Address,
		Company,
		StoreName,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Partner() {}

	explicit Partner(const Text & name) :
		name(name)
	{}

	explicit Partner(const Partner & orig) :
		name(orig.name),
		country(orig.country),
		city(orig.city),
		postalCode(orig.postalCode),
		address(orig.address),
		company(orig.company),
		storeName(orig.storeName),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Partner(Partner && temp) :
		name(csjp::move_cast(temp.name)),
		country(csjp::move_cast(temp.country)),
		city(csjp::move_cast(temp.city)),
		postalCode(csjp::move_cast(temp.postalCode)),
		address(csjp::move_cast(temp.address)),
		company(csjp::move_cast(temp.company)),
		storeName(csjp::move_cast(temp.storeName)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Partner() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= country;
		json["country"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= city;
		json["city"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= postalCode;
		json["postalCode"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= address;
		json["address"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= company;
		json["company"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= storeName;
		json["storeName"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Partner& operator=(Partner && temp)
	{
		name = csjp::move_cast(temp.name);
		country = csjp::move_cast(temp.country);
		city = csjp::move_cast(temp.city);
		postalCode = csjp::move_cast(temp.postalCode);
		address = csjp::move_cast(temp.address);
		company = csjp::move_cast(temp.company);
		storeName = csjp::move_cast(temp.storeName);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Partner& operator=(const Partner & orig)
	{
		Partner copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Partner & other) const
	{
		if(
				name != other.name ||
				country != other.country ||
				city != other.city ||
				postalCode != other.postalCode ||
				address != other.address ||
				company != other.company ||
				storeName != other.storeName ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Partner & other) const
	{
		return name < other.name;	}

	bool isMore(const Partner & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Partner fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Partner & orig) const;

};
inline bool operator==(const Partner & a, const Partner & b) { return a.isEqual(b); }
inline bool operator!=(const Partner & a, const Partner & b) { return !a.isEqual(b); }
inline bool operator<(const Partner & a, const Partner & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Partner & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Partner & b) { return b.isMore(a); }
inline bool operator<(const Partner & a, const Text & b) { return a.isLess(b); }

class PartnerSet : public csjp::SorterOwnerContainer<Partner>
{
	public:
	csjp::PodArray<Partner::Fields> ordering;
	bool ascending;

	const PartnerSet & operator=(PartnerSet && temp)
	{
		csjp::SorterOwnerContainer<Partner>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Partner & a, const Partner & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Partner::Name :
				ret = a.name < b.name;
				break;
			case Partner::Country :
				ret = a.country < b.country;
				break;
			case Partner::City :
				ret = a.city < b.city;
				break;
			case Partner::PostalCode :
				ret = a.postalCode < b.postalCode;
				break;
			case Partner::Address :
				ret = a.address < b.address;
				break;
			case Partner::Company :
				ret = a.company < b.company;
				break;
			case Partner::StoreName :
				ret = a.storeName < b.storeName;
				break;
			case Partner::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Partner::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	PartnerSet() :
		csjp::SorterOwnerContainer<Partner>(),
		ascending(true)
	{
		ordering.add(Partner::Name);
		ordering.add(Partner::Country);
		ordering.add(Partner::City);
		ordering.add(Partner::PostalCode);
		ordering.add(Partner::Address);
		ordering.add(Partner::Company);
		ordering.add(Partner::StoreName);
		ordering.add(Partner::LastModified);
		ordering.add(Partner::Deleted);
	}
	PartnerSet(const PartnerSet & ts) :
		csjp::SorterOwnerContainer<Partner>(ts),
		ascending(true)
	{
		ordering.add(Partner::Name);
		ordering.add(Partner::Country);
		ordering.add(Partner::City);
		ordering.add(Partner::PostalCode);
		ordering.add(Partner::Address);
		ordering.add(Partner::Company);
		ordering.add(Partner::StoreName);
		ordering.add(Partner::LastModified);
		ordering.add(Partner::Deleted);
	}
	~PartnerSet() {}


	
	static PartnerSet fromDb(SqlConnection & sql);

};

class Account{
	public:

	Text name;
	Text currency;
	Text bankOffice;
	Text iban;
	Text swiftCode;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		Currency,
		BankOffice,
		Iban,
		SwiftCode,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Account() {}

	explicit Account(const Text & name) :
		name(name)
	{}

	explicit Account(const Account & orig) :
		name(orig.name),
		currency(orig.currency),
		bankOffice(orig.bankOffice),
		iban(orig.iban),
		swiftCode(orig.swiftCode),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Account(Account && temp) :
		name(csjp::move_cast(temp.name)),
		currency(csjp::move_cast(temp.currency)),
		bankOffice(csjp::move_cast(temp.bankOffice)),
		iban(csjp::move_cast(temp.iban)),
		swiftCode(csjp::move_cast(temp.swiftCode)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Account() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= currency;
		json["currency"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= bankOffice;
		json["bankOffice"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= iban;
		json["iban"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= swiftCode;
		json["swiftCode"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Account& operator=(Account && temp)
	{
		name = csjp::move_cast(temp.name);
		currency = csjp::move_cast(temp.currency);
		bankOffice = csjp::move_cast(temp.bankOffice);
		iban = csjp::move_cast(temp.iban);
		swiftCode = csjp::move_cast(temp.swiftCode);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Account& operator=(const Account & orig)
	{
		Account copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Account & other) const
	{
		if(
				name != other.name ||
				currency != other.currency ||
				bankOffice != other.bankOffice ||
				iban != other.iban ||
				swiftCode != other.swiftCode ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Account & other) const
	{
		return name < other.name;	}

	bool isMore(const Account & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static Account fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Account & orig) const;

};
inline bool operator==(const Account & a, const Account & b) { return a.isEqual(b); }
inline bool operator!=(const Account & a, const Account & b) { return !a.isEqual(b); }
inline bool operator<(const Account & a, const Account & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Account & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const Account & b) { return b.isMore(a); }
inline bool operator<(const Account & a, const Text & b) { return a.isLess(b); }

class AccountSet : public csjp::SorterOwnerContainer<Account>
{
	public:
	csjp::PodArray<Account::Fields> ordering;
	bool ascending;

	const AccountSet & operator=(AccountSet && temp)
	{
		csjp::SorterOwnerContainer<Account>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Account & a, const Account & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Account::Name :
				ret = a.name < b.name;
				break;
			case Account::Currency :
				ret = a.currency < b.currency;
				break;
			case Account::BankOffice :
				ret = a.bankOffice < b.bankOffice;
				break;
			case Account::Iban :
				ret = a.iban < b.iban;
				break;
			case Account::SwiftCode :
				ret = a.swiftCode < b.swiftCode;
				break;
			case Account::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Account::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	AccountSet() :
		csjp::SorterOwnerContainer<Account>(),
		ascending(true)
	{
		ordering.add(Account::Name);
		ordering.add(Account::Currency);
		ordering.add(Account::BankOffice);
		ordering.add(Account::Iban);
		ordering.add(Account::SwiftCode);
		ordering.add(Account::LastModified);
		ordering.add(Account::Deleted);
	}
	AccountSet(const AccountSet & ts) :
		csjp::SorterOwnerContainer<Account>(ts),
		ascending(true)
	{
		ordering.add(Account::Name);
		ordering.add(Account::Currency);
		ordering.add(Account::BankOffice);
		ordering.add(Account::Iban);
		ordering.add(Account::SwiftCode);
		ordering.add(Account::LastModified);
		ordering.add(Account::Deleted);
	}
	~AccountSet() {}


	
	static AccountSet fromDb(SqlConnection & sql);

};

class ItemQueryWithTag{
	public:

	Text query;
	Text tag;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Query,
		Tag,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	ItemQueryWithTag() {}

	explicit ItemQueryWithTag(const Text & query, const Text & tag) :
		query(query),
		tag(tag)
	{}

	explicit ItemQueryWithTag(const ItemQueryWithTag & orig) :
		query(orig.query),
		tag(orig.tag),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	ItemQueryWithTag(ItemQueryWithTag && temp) :
		query(csjp::move_cast(temp.query)),
		tag(csjp::move_cast(temp.tag)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~ItemQueryWithTag() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= query;
		json["query"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= tag;
		json["tag"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	ItemQueryWithTag& operator=(ItemQueryWithTag && temp)
	{
		query = csjp::move_cast(temp.query);
		tag = csjp::move_cast(temp.tag);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	ItemQueryWithTag& operator=(const ItemQueryWithTag & orig)
	{
		ItemQueryWithTag copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const ItemQueryWithTag & other) const
	{
		if(
				query != other.query ||
				tag != other.tag ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const ItemQueryWithTag & other) const
	{
		return query < other.query && tag < other.tag;	}

	bool isMore(const ItemQueryWithTag & other) const
	{
		return other.query < query &&other.tag < tag;	}

	bool isLess(const Text & otherquery,const Text & othertag) const
	{
		return query < otherquery &&tag < othertag;	}

	bool isMore(const Text & otherquery,const Text & othertag) const
	{
		return otherquery < query &&othertag < tag;	}


	
	static ItemQueryWithTag fromDb(SqlConnection & sql, const Text & _query, const Text & _tag);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const ItemQueryWithTag & orig) const;

};
inline bool operator==(const ItemQueryWithTag & a, const ItemQueryWithTag & b) { return a.isEqual(b); }
inline bool operator!=(const ItemQueryWithTag & a, const ItemQueryWithTag & b) { return !a.isEqual(b); }
inline bool operator<(const ItemQueryWithTag & a, const ItemQueryWithTag & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const ItemQueryWithTag & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class ItemQueryWithTagSet : public csjp::SorterOwnerContainer<ItemQueryWithTag>
{
	public:
	csjp::PodArray<ItemQueryWithTag::Fields> ordering;
	bool ascending;

	const ItemQueryWithTagSet & operator=(ItemQueryWithTagSet && temp)
	{
		csjp::SorterOwnerContainer<ItemQueryWithTag>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const ItemQueryWithTag & a, const ItemQueryWithTag & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case ItemQueryWithTag::Query :
				ret = a.query < b.query;
				break;
			case ItemQueryWithTag::Tag :
				ret = a.tag < b.tag;
				break;
			case ItemQueryWithTag::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case ItemQueryWithTag::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.query < b.query &&
					a.tag < b.tag;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemQueryWithTagSet() :
		csjp::SorterOwnerContainer<ItemQueryWithTag>(),
		ascending(true)
	{
		ordering.add(ItemQueryWithTag::Query);
		ordering.add(ItemQueryWithTag::Tag);
		ordering.add(ItemQueryWithTag::LastModified);
		ordering.add(ItemQueryWithTag::Deleted);
	}
	ItemQueryWithTagSet(const ItemQueryWithTagSet & ts) :
		csjp::SorterOwnerContainer<ItemQueryWithTag>(ts),
		ascending(true)
	{
		ordering.add(ItemQueryWithTag::Query);
		ordering.add(ItemQueryWithTag::Tag);
		ordering.add(ItemQueryWithTag::LastModified);
		ordering.add(ItemQueryWithTag::Deleted);
	}
	~ItemQueryWithTagSet() {}


	
	static ItemQueryWithTagSet fromDb(SqlConnection & sql, const Text & _query);

};

class ItemQueryWithoutTag{
	public:

	Text query;
	Text tag;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Query,
		Tag,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	ItemQueryWithoutTag() {}

	explicit ItemQueryWithoutTag(const Text & query, const Text & tag) :
		query(query),
		tag(tag)
	{}

	explicit ItemQueryWithoutTag(const ItemQueryWithoutTag & orig) :
		query(orig.query),
		tag(orig.tag),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	ItemQueryWithoutTag(ItemQueryWithoutTag && temp) :
		query(csjp::move_cast(temp.query)),
		tag(csjp::move_cast(temp.tag)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~ItemQueryWithoutTag() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= query;
		json["query"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= tag;
		json["tag"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	ItemQueryWithoutTag& operator=(ItemQueryWithoutTag && temp)
	{
		query = csjp::move_cast(temp.query);
		tag = csjp::move_cast(temp.tag);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	ItemQueryWithoutTag& operator=(const ItemQueryWithoutTag & orig)
	{
		ItemQueryWithoutTag copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const ItemQueryWithoutTag & other) const
	{
		if(
				query != other.query ||
				tag != other.tag ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const ItemQueryWithoutTag & other) const
	{
		return query < other.query && tag < other.tag;	}

	bool isMore(const ItemQueryWithoutTag & other) const
	{
		return other.query < query &&other.tag < tag;	}

	bool isLess(const Text & otherquery,const Text & othertag) const
	{
		return query < otherquery &&tag < othertag;	}

	bool isMore(const Text & otherquery,const Text & othertag) const
	{
		return otherquery < query &&othertag < tag;	}


	
	static ItemQueryWithoutTag fromDb(SqlConnection & sql, const Text & _query, const Text & _tag);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const ItemQueryWithoutTag & orig) const;

};
inline bool operator==(const ItemQueryWithoutTag & a, const ItemQueryWithoutTag & b) { return a.isEqual(b); }
inline bool operator!=(const ItemQueryWithoutTag & a, const ItemQueryWithoutTag & b) { return !a.isEqual(b); }
inline bool operator<(const ItemQueryWithoutTag & a, const ItemQueryWithoutTag & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const ItemQueryWithoutTag & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class ItemQueryWithoutTagSet : public csjp::SorterOwnerContainer<ItemQueryWithoutTag>
{
	public:
	csjp::PodArray<ItemQueryWithoutTag::Fields> ordering;
	bool ascending;

	const ItemQueryWithoutTagSet & operator=(ItemQueryWithoutTagSet && temp)
	{
		csjp::SorterOwnerContainer<ItemQueryWithoutTag>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const ItemQueryWithoutTag & a, const ItemQueryWithoutTag & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case ItemQueryWithoutTag::Query :
				ret = a.query < b.query;
				break;
			case ItemQueryWithoutTag::Tag :
				ret = a.tag < b.tag;
				break;
			case ItemQueryWithoutTag::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case ItemQueryWithoutTag::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.query < b.query &&
					a.tag < b.tag;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemQueryWithoutTagSet() :
		csjp::SorterOwnerContainer<ItemQueryWithoutTag>(),
		ascending(true)
	{
		ordering.add(ItemQueryWithoutTag::Query);
		ordering.add(ItemQueryWithoutTag::Tag);
		ordering.add(ItemQueryWithoutTag::LastModified);
		ordering.add(ItemQueryWithoutTag::Deleted);
	}
	ItemQueryWithoutTagSet(const ItemQueryWithoutTagSet & ts) :
		csjp::SorterOwnerContainer<ItemQueryWithoutTag>(ts),
		ascending(true)
	{
		ordering.add(ItemQueryWithoutTag::Query);
		ordering.add(ItemQueryWithoutTag::Tag);
		ordering.add(ItemQueryWithoutTag::LastModified);
		ordering.add(ItemQueryWithoutTag::Deleted);
	}
	~ItemQueryWithoutTagSet() {}


	
	static ItemQueryWithoutTagSet fromDb(SqlConnection & sql, const Text & _query);

};

class ItemQueryWare{
	public:

	Text query;
	Text ware;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Query,
		Ware,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	ItemQueryWare() {}

	explicit ItemQueryWare(const Text & query, const Text & ware) :
		query(query),
		ware(ware)
	{}

	explicit ItemQueryWare(const ItemQueryWare & orig) :
		query(orig.query),
		ware(orig.ware),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	ItemQueryWare(ItemQueryWare && temp) :
		query(csjp::move_cast(temp.query)),
		ware(csjp::move_cast(temp.ware)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~ItemQueryWare() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= query;
		json["query"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= ware;
		json["ware"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	ItemQueryWare& operator=(ItemQueryWare && temp)
	{
		query = csjp::move_cast(temp.query);
		ware = csjp::move_cast(temp.ware);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	ItemQueryWare& operator=(const ItemQueryWare & orig)
	{
		ItemQueryWare copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const ItemQueryWare & other) const
	{
		if(
				query != other.query ||
				ware != other.ware ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const ItemQueryWare & other) const
	{
		return query < other.query && ware < other.ware;	}

	bool isMore(const ItemQueryWare & other) const
	{
		return other.query < query &&other.ware < ware;	}

	bool isLess(const Text & otherquery,const Text & otherware) const
	{
		return query < otherquery &&ware < otherware;	}

	bool isMore(const Text & otherquery,const Text & otherware) const
	{
		return otherquery < query &&otherware < ware;	}


	
	static ItemQueryWare fromDb(SqlConnection & sql, const Text & _query, const Text & _ware);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const ItemQueryWare & orig) const;

};
inline bool operator==(const ItemQueryWare & a, const ItemQueryWare & b) { return a.isEqual(b); }
inline bool operator!=(const ItemQueryWare & a, const ItemQueryWare & b) { return !a.isEqual(b); }
inline bool operator<(const ItemQueryWare & a, const ItemQueryWare & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const ItemQueryWare & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class ItemQueryWareSet : public csjp::SorterOwnerContainer<ItemQueryWare>
{
	public:
	csjp::PodArray<ItemQueryWare::Fields> ordering;
	bool ascending;

	const ItemQueryWareSet & operator=(ItemQueryWareSet && temp)
	{
		csjp::SorterOwnerContainer<ItemQueryWare>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const ItemQueryWare & a, const ItemQueryWare & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case ItemQueryWare::Query :
				ret = a.query < b.query;
				break;
			case ItemQueryWare::Ware :
				ret = a.ware < b.ware;
				break;
			case ItemQueryWare::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case ItemQueryWare::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.query < b.query &&
					a.ware < b.ware;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemQueryWareSet() :
		csjp::SorterOwnerContainer<ItemQueryWare>(),
		ascending(true)
	{
		ordering.add(ItemQueryWare::Query);
		ordering.add(ItemQueryWare::Ware);
		ordering.add(ItemQueryWare::LastModified);
		ordering.add(ItemQueryWare::Deleted);
	}
	ItemQueryWareSet(const ItemQueryWareSet & ts) :
		csjp::SorterOwnerContainer<ItemQueryWare>(ts),
		ascending(true)
	{
		ordering.add(ItemQueryWare::Query);
		ordering.add(ItemQueryWare::Ware);
		ordering.add(ItemQueryWare::LastModified);
		ordering.add(ItemQueryWare::Deleted);
	}
	~ItemQueryWareSet() {}


	
	static ItemQueryWareSet fromDb(SqlConnection & sql, const Text & _query);

};

class ItemQueryPartner{
	public:

	Text query;
	Text partner;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Query,
		Partner,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	ItemQueryPartner() {}

	explicit ItemQueryPartner(const Text & query, const Text & partner) :
		query(query),
		partner(partner)
	{}

	explicit ItemQueryPartner(const ItemQueryPartner & orig) :
		query(orig.query),
		partner(orig.partner),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	ItemQueryPartner(ItemQueryPartner && temp) :
		query(csjp::move_cast(temp.query)),
		partner(csjp::move_cast(temp.partner)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~ItemQueryPartner() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= query;
		json["query"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= partner;
		json["partner"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	ItemQueryPartner& operator=(ItemQueryPartner && temp)
	{
		query = csjp::move_cast(temp.query);
		partner = csjp::move_cast(temp.partner);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	ItemQueryPartner& operator=(const ItemQueryPartner & orig)
	{
		ItemQueryPartner copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const ItemQueryPartner & other) const
	{
		if(
				query != other.query ||
				partner != other.partner ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const ItemQueryPartner & other) const
	{
		return query < other.query && partner < other.partner;	}

	bool isMore(const ItemQueryPartner & other) const
	{
		return other.query < query &&other.partner < partner;	}

	bool isLess(const Text & otherquery,const Text & otherpartner) const
	{
		return query < otherquery &&partner < otherpartner;	}

	bool isMore(const Text & otherquery,const Text & otherpartner) const
	{
		return otherquery < query &&otherpartner < partner;	}


	
	static ItemQueryPartner fromDb(SqlConnection & sql, const Text & _query, const Text & _partner);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const ItemQueryPartner & orig) const;

};
inline bool operator==(const ItemQueryPartner & a, const ItemQueryPartner & b) { return a.isEqual(b); }
inline bool operator!=(const ItemQueryPartner & a, const ItemQueryPartner & b) { return !a.isEqual(b); }
inline bool operator<(const ItemQueryPartner & a, const ItemQueryPartner & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const ItemQueryPartner & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class ItemQueryPartnerSet : public csjp::SorterOwnerContainer<ItemQueryPartner>
{
	public:
	csjp::PodArray<ItemQueryPartner::Fields> ordering;
	bool ascending;

	const ItemQueryPartnerSet & operator=(ItemQueryPartnerSet && temp)
	{
		csjp::SorterOwnerContainer<ItemQueryPartner>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const ItemQueryPartner & a, const ItemQueryPartner & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case ItemQueryPartner::Query :
				ret = a.query < b.query;
				break;
			case ItemQueryPartner::Partner :
				ret = a.partner < b.partner;
				break;
			case ItemQueryPartner::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case ItemQueryPartner::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.query < b.query &&
					a.partner < b.partner;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemQueryPartnerSet() :
		csjp::SorterOwnerContainer<ItemQueryPartner>(),
		ascending(true)
	{
		ordering.add(ItemQueryPartner::Query);
		ordering.add(ItemQueryPartner::Partner);
		ordering.add(ItemQueryPartner::LastModified);
		ordering.add(ItemQueryPartner::Deleted);
	}
	ItemQueryPartnerSet(const ItemQueryPartnerSet & ts) :
		csjp::SorterOwnerContainer<ItemQueryPartner>(ts),
		ascending(true)
	{
		ordering.add(ItemQueryPartner::Query);
		ordering.add(ItemQueryPartner::Partner);
		ordering.add(ItemQueryPartner::LastModified);
		ordering.add(ItemQueryPartner::Deleted);
	}
	~ItemQueryPartnerSet() {}


	
	static ItemQueryPartnerSet fromDb(SqlConnection & sql, const Text & _query);

};

class PaymentQueryPartner{
	public:

	Text query;
	Text partner;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Query,
		Partner,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	PaymentQueryPartner() {}

	explicit PaymentQueryPartner(const Text & query, const Text & partner) :
		query(query),
		partner(partner)
	{}

	explicit PaymentQueryPartner(const PaymentQueryPartner & orig) :
		query(orig.query),
		partner(orig.partner),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	PaymentQueryPartner(PaymentQueryPartner && temp) :
		query(csjp::move_cast(temp.query)),
		partner(csjp::move_cast(temp.partner)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~PaymentQueryPartner() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= query;
		json["query"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= partner;
		json["partner"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	PaymentQueryPartner& operator=(PaymentQueryPartner && temp)
	{
		query = csjp::move_cast(temp.query);
		partner = csjp::move_cast(temp.partner);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	PaymentQueryPartner& operator=(const PaymentQueryPartner & orig)
	{
		PaymentQueryPartner copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const PaymentQueryPartner & other) const
	{
		if(
				query != other.query ||
				partner != other.partner ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const PaymentQueryPartner & other) const
	{
		return query < other.query && partner < other.partner;	}

	bool isMore(const PaymentQueryPartner & other) const
	{
		return other.query < query &&other.partner < partner;	}

	bool isLess(const Text & otherquery,const Text & otherpartner) const
	{
		return query < otherquery &&partner < otherpartner;	}

	bool isMore(const Text & otherquery,const Text & otherpartner) const
	{
		return otherquery < query &&otherpartner < partner;	}


	
	static PaymentQueryPartner fromDb(SqlConnection & sql, const Text & _query, const Text & _partner);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const PaymentQueryPartner & orig) const;

};
inline bool operator==(const PaymentQueryPartner & a, const PaymentQueryPartner & b) { return a.isEqual(b); }
inline bool operator!=(const PaymentQueryPartner & a, const PaymentQueryPartner & b) { return !a.isEqual(b); }
inline bool operator<(const PaymentQueryPartner & a, const PaymentQueryPartner & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const PaymentQueryPartner & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }


class PaymentQueryPartnerSet : public csjp::SorterOwnerContainer<PaymentQueryPartner>
{
	public:
	csjp::PodArray<PaymentQueryPartner::Fields> ordering;
	bool ascending;

	const PaymentQueryPartnerSet & operator=(PaymentQueryPartnerSet && temp)
	{
		csjp::SorterOwnerContainer<PaymentQueryPartner>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const PaymentQueryPartner & a, const PaymentQueryPartner & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case PaymentQueryPartner::Query :
				ret = a.query < b.query;
				break;
			case PaymentQueryPartner::Partner :
				ret = a.partner < b.partner;
				break;
			case PaymentQueryPartner::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case PaymentQueryPartner::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.query < b.query &&
					a.partner < b.partner;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	PaymentQueryPartnerSet() :
		csjp::SorterOwnerContainer<PaymentQueryPartner>(),
		ascending(true)
	{
		ordering.add(PaymentQueryPartner::Query);
		ordering.add(PaymentQueryPartner::Partner);
		ordering.add(PaymentQueryPartner::LastModified);
		ordering.add(PaymentQueryPartner::Deleted);
	}
	PaymentQueryPartnerSet(const PaymentQueryPartnerSet & ts) :
		csjp::SorterOwnerContainer<PaymentQueryPartner>(ts),
		ascending(true)
	{
		ordering.add(PaymentQueryPartner::Query);
		ordering.add(PaymentQueryPartner::Partner);
		ordering.add(PaymentQueryPartner::LastModified);
		ordering.add(PaymentQueryPartner::Deleted);
	}
	~PaymentQueryPartnerSet() {}


	
	static PaymentQueryPartnerSet fromDb(SqlConnection & sql, const Text & _query);

};

class PaymentQuery{
	public:

	Text name;
	DateTime startDate;
	DateTime endDate;
	PaymentQueryPartnerSet partners;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		StartDate,
		EndDate,
		Partners,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	PaymentQuery() {}

	explicit PaymentQuery(const Text & name) :
		name(name)
	{}

	explicit PaymentQuery(const PaymentQuery & orig) :
		name(orig.name),
		startDate(orig.startDate),
		endDate(orig.endDate),
		partners(orig.partners),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	PaymentQuery(PaymentQuery && temp) :
		name(csjp::move_cast(temp.name)),
		startDate(csjp::move_cast(temp.startDate)),
		endDate(csjp::move_cast(temp.endDate)),
		partners(csjp::move_cast(temp.partners)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~PaymentQuery() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= startDate;
		json["startDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= endDate;
		json["endDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	PaymentQuery& operator=(PaymentQuery && temp)
	{
		name = csjp::move_cast(temp.name);
		startDate = csjp::move_cast(temp.startDate);
		endDate = csjp::move_cast(temp.endDate);
		partners = csjp::move_cast(temp.partners);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	PaymentQuery& operator=(const PaymentQuery & orig)
	{
		PaymentQuery copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const PaymentQuery & other) const
	{
		if(
				name != other.name ||
				startDate != other.startDate ||
				endDate != other.endDate ||
				partners != other.partners ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const PaymentQuery & other) const
	{
		return name < other.name;	}

	bool isMore(const PaymentQuery & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static PaymentQuery fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const PaymentQuery & orig) const;

};
inline bool operator==(const PaymentQuery & a, const PaymentQuery & b) { return a.isEqual(b); }
inline bool operator!=(const PaymentQuery & a, const PaymentQuery & b) { return !a.isEqual(b); }
inline bool operator<(const PaymentQuery & a, const PaymentQuery & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const PaymentQuery & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const PaymentQuery & b) { return b.isMore(a); }
inline bool operator<(const PaymentQuery & a, const Text & b) { return a.isLess(b); }

class PaymentQuerySet : public csjp::SorterOwnerContainer<PaymentQuery>
{
	public:
	csjp::PodArray<PaymentQuery::Fields> ordering;
	bool ascending;

	const PaymentQuerySet & operator=(PaymentQuerySet && temp)
	{
		csjp::SorterOwnerContainer<PaymentQuery>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const PaymentQuery & a, const PaymentQuery & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case PaymentQuery::Name :
				ret = a.name < b.name;
				break;
			case PaymentQuery::StartDate :
				ret = a.startDate < b.startDate;
				break;
			case PaymentQuery::EndDate :
				ret = a.endDate < b.endDate;
				break;
			case PaymentQuery::Partners :
				ret = a.partners < b.partners;
				break;
			case PaymentQuery::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case PaymentQuery::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	PaymentQuerySet() :
		csjp::SorterOwnerContainer<PaymentQuery>(),
		ascending(true)
	{
		ordering.add(PaymentQuery::Name);
		ordering.add(PaymentQuery::StartDate);
		ordering.add(PaymentQuery::EndDate);
		ordering.add(PaymentQuery::Partners);
		ordering.add(PaymentQuery::LastModified);
		ordering.add(PaymentQuery::Deleted);
	}
	PaymentQuerySet(const PaymentQuerySet & ts) :
		csjp::SorterOwnerContainer<PaymentQuery>(ts),
		ascending(true)
	{
		ordering.add(PaymentQuery::Name);
		ordering.add(PaymentQuery::StartDate);
		ordering.add(PaymentQuery::EndDate);
		ordering.add(PaymentQuery::Partners);
		ordering.add(PaymentQuery::LastModified);
		ordering.add(PaymentQuery::Deleted);
	}
	~PaymentQuerySet() {}


	
	static PaymentQuerySet fromDb(SqlConnection & sql);

};


inline bool operator<(const Text & a, const ItemQueryWithTag & b) { return b.tag.isMore(a); }
inline bool operator<(const ItemQueryWithTag & a, const Text & b) { return a.tag.isLess(b); }
inline bool operator<(const Text & a, const ItemQueryWithoutTag & b) { return b.tag.isMore(a); }
inline bool operator<(const ItemQueryWithoutTag & a, const Text & b) { return a.tag.isLess(b); }



class ItemQuery
{
	public:

	Text name;
	DateTime startDate;
	DateTime endDate;
	enum ItemQueryStockOptions stockOption;
	enum ItemQueryTagOptions tagOption;
	ItemQueryWithTagSet withTags;
	ItemQueryWithoutTagSet withoutTags;
	ItemQueryWareSet wares;
	ItemQueryPartnerSet partners;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		Name,
		StartDate,
		EndDate,
		StockOption,
		TagOption,
		WithTags,
		WithoutTags,
		Wares,
		Partners,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	ItemQuery() {}

	explicit ItemQuery(const Text & name) :
		name(name)
	{}

	explicit ItemQuery(const ItemQuery & orig) :
		name(orig.name),
		startDate(orig.startDate),
		endDate(orig.endDate),
		stockOption(orig.stockOption),
		tagOption(orig.tagOption),
		withTags(orig.withTags),
		withoutTags(orig.withoutTags),
		wares(orig.wares),
		partners(orig.partners),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	ItemQuery(ItemQuery && temp) :
		name(csjp::move_cast(temp.name)),
		startDate(csjp::move_cast(temp.startDate)),
		endDate(csjp::move_cast(temp.endDate)),
		stockOption(csjp::move_cast(temp.stockOption)),
		tagOption(csjp::move_cast(temp.tagOption)),
		withTags(csjp::move_cast(temp.withTags)),
		withoutTags(csjp::move_cast(temp.withoutTags)),
		wares(csjp::move_cast(temp.wares)),
		partners(csjp::move_cast(temp.partners)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~ItemQuery() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= startDate;
		json["startDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= endDate;
		json["endDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= stockOption;
		json["stockOption"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= tagOption;
		json["tagOption"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	ItemQuery& operator=(ItemQuery && temp)
	{
		name = csjp::move_cast(temp.name);
		startDate = csjp::move_cast(temp.startDate);
		endDate = csjp::move_cast(temp.endDate);
		stockOption = csjp::move_cast(temp.stockOption);
		tagOption = csjp::move_cast(temp.tagOption);
		withTags = csjp::move_cast(temp.withTags);
		withoutTags = csjp::move_cast(temp.withoutTags);
		wares = csjp::move_cast(temp.wares);
		partners = csjp::move_cast(temp.partners);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	ItemQuery& operator=(const ItemQuery & orig)
	{
		ItemQuery copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const ItemQuery & other) const
	{
		if(
				name != other.name ||
				startDate != other.startDate ||
				endDate != other.endDate ||
				stockOption != other.stockOption ||
				tagOption != other.tagOption ||
				withTags != other.withTags ||
				withoutTags != other.withoutTags ||
				wares != other.wares ||
				partners != other.partners ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const ItemQuery & other) const
	{
		return name < other.name;	}

	bool isMore(const ItemQuery & other) const
	{
		return other.name < name;	}

	bool isLess(const Text & othername) const
	{
		return name < othername;	}

	bool isMore(const Text & othername) const
	{
		return othername < name;	}


	
	static ItemQuery fromDb(SqlConnection & sql, const Text & _name);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const ItemQuery & orig) const;


	/* non-transactional */
	void setAsWithTags(const StringSet & names)
	{
		for(auto & tag : withTags)
			if(!names.has(tag.tag))
				const_cast<ItemQueryWithTag &>(tag).deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withTags.has(tag))
				withTags.add(new ItemQueryWithTag(name, tag));
		}
	}
	/* non-transactional */
	void setAsWithoutTags(const StringSet & names)
	{
		for(auto & tag : withoutTags)
			if(!names.has(tag.tag))
				const_cast<ItemQueryWithoutTag &>(tag).deleted = true;
		for(auto & name : names){
			Text tag(name);
			if(!withoutTags.has(tag))
				withoutTags.add(new ItemQueryWithoutTag(name, tag));
		}
	}
};
inline bool operator==(const ItemQuery & a, const ItemQuery & b) { return a.isEqual(b); }
inline bool operator!=(const ItemQuery & a, const ItemQuery & b) { return !a.isEqual(b); }
inline bool operator<(const ItemQuery & a, const ItemQuery & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const ItemQuery & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const Text & a, const ItemQuery & b) { return b.isMore(a); }
inline bool operator<(const ItemQuery & a, const Text & b) { return a.isLess(b); }

class ItemQuerySet : public csjp::SorterOwnerContainer<ItemQuery>
{
	public:
	csjp::PodArray<ItemQuery::Fields> ordering;
	bool ascending;

	const ItemQuerySet & operator=(ItemQuerySet && temp)
	{
		csjp::SorterOwnerContainer<ItemQuery>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const ItemQuery & a, const ItemQuery & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case ItemQuery::Name :
				ret = a.name < b.name;
				break;
			case ItemQuery::StartDate :
				ret = a.startDate < b.startDate;
				break;
			case ItemQuery::EndDate :
				ret = a.endDate < b.endDate;
				break;
			case ItemQuery::StockOption :
				ret = a.stockOption < b.stockOption;
				break;
			case ItemQuery::TagOption :
				ret = a.tagOption < b.tagOption;
				break;
			case ItemQuery::WithTags :
				ret = a.withTags < b.withTags;
				break;
			case ItemQuery::WithoutTags :
				ret = a.withoutTags < b.withoutTags;
				break;
			case ItemQuery::Wares :
				ret = a.wares < b.wares;
				break;
			case ItemQuery::Partners :
				ret = a.partners < b.partners;
				break;
			case ItemQuery::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case ItemQuery::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.name < b.name;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemQuerySet() :
		csjp::SorterOwnerContainer<ItemQuery>(),
		ascending(true)
	{
		ordering.add(ItemQuery::Name);
		ordering.add(ItemQuery::StartDate);
		ordering.add(ItemQuery::EndDate);
		ordering.add(ItemQuery::StockOption);
		ordering.add(ItemQuery::TagOption);
		ordering.add(ItemQuery::WithTags);
		ordering.add(ItemQuery::WithoutTags);
		ordering.add(ItemQuery::Wares);
		ordering.add(ItemQuery::Partners);
		ordering.add(ItemQuery::LastModified);
		ordering.add(ItemQuery::Deleted);
	}
	ItemQuerySet(const ItemQuerySet & ts) :
		csjp::SorterOwnerContainer<ItemQuery>(ts),
		ascending(true)
	{
		ordering.add(ItemQuery::Name);
		ordering.add(ItemQuery::StartDate);
		ordering.add(ItemQuery::EndDate);
		ordering.add(ItemQuery::StockOption);
		ordering.add(ItemQuery::TagOption);
		ordering.add(ItemQuery::WithTags);
		ordering.add(ItemQuery::WithoutTags);
		ordering.add(ItemQuery::Wares);
		ordering.add(ItemQuery::Partners);
		ordering.add(ItemQuery::LastModified);
		ordering.add(ItemQuery::Deleted);
	}
	~ItemQuerySet() {}


	
	static ItemQuerySet fromDb(SqlConnection & sql);

};



class Item{
	public:

	DateTime uploadDate;
	Text name;
	Text unit;
	Text type;
	Text brand;
	Double quantity; // amoutn to buy or not
	Double price; // price of gross piece/amount quantity
	Text currency;
	Text account;
	Text partner;
	Text inventory;
	Text comment;
	DateTime invChangeDate;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		UploadDate,
		Name,
		Unit,
		Type,
		Brand,
		Quantity,
		Price,
		Currency,
		Account,
		Partner,
		Inventory,
		Comment,
		InvChangeDate,
		LastModified,
		Deleted,
		NumOfFields
		, UnitPrice = NumOfFields + 0
		, QuantityWithUnit = NumOfFields + 1
		, NumOfAllFields
	};

public:
	Item() {}

	explicit Item(const DateTime & uploadDate) :
		uploadDate(uploadDate)
	{}

	explicit Item(const Item & orig) :
		uploadDate(orig.uploadDate),
		name(orig.name),
		unit(orig.unit),
		type(orig.type),
		brand(orig.brand),
		quantity(orig.quantity),
		price(orig.price),
		currency(orig.currency),
		account(orig.account),
		partner(orig.partner),
		inventory(orig.inventory),
		comment(orig.comment),
		invChangeDate(orig.invChangeDate),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Item(Item && temp) :
		uploadDate(csjp::move_cast(temp.uploadDate)),
		name(csjp::move_cast(temp.name)),
		unit(csjp::move_cast(temp.unit)),
		type(csjp::move_cast(temp.type)),
		brand(csjp::move_cast(temp.brand)),
		quantity(csjp::move_cast(temp.quantity)),
		price(csjp::move_cast(temp.price)),
		currency(csjp::move_cast(temp.currency)),
		account(csjp::move_cast(temp.account)),
		partner(csjp::move_cast(temp.partner)),
		inventory(csjp::move_cast(temp.inventory)),
		comment(csjp::move_cast(temp.comment)),
		invChangeDate(csjp::move_cast(temp.invChangeDate)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Item() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= uploadDate;
		json["uploadDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= name;
		json["name"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= unit;
		json["unit"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= type;
		json["type"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= brand;
		json["brand"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= quantity;
		json["quantity"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= price;
		json["price"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= currency;
		json["currency"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= account;
		json["account"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= partner;
		json["partner"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= inventory;
		json["inventory"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= comment;
		json["comment"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= invChangeDate;
		json["invChangeDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Item& operator=(Item && temp)
	{
		uploadDate = csjp::move_cast(temp.uploadDate);
		name = csjp::move_cast(temp.name);
		unit = csjp::move_cast(temp.unit);
		type = csjp::move_cast(temp.type);
		brand = csjp::move_cast(temp.brand);
		quantity = csjp::move_cast(temp.quantity);
		price = csjp::move_cast(temp.price);
		currency = csjp::move_cast(temp.currency);
		account = csjp::move_cast(temp.account);
		partner = csjp::move_cast(temp.partner);
		inventory = csjp::move_cast(temp.inventory);
		comment = csjp::move_cast(temp.comment);
		invChangeDate = csjp::move_cast(temp.invChangeDate);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Item& operator=(const Item & orig)
	{
		Item copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Item & other) const
	{
		if(
				uploadDate != other.uploadDate ||
				name != other.name ||
				unit != other.unit ||
				type != other.type ||
				brand != other.brand ||
				quantity != other.quantity ||
				price != other.price ||
				currency != other.currency ||
				account != other.account ||
				partner != other.partner ||
				inventory != other.inventory ||
				comment != other.comment ||
				invChangeDate != other.invChangeDate ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Item & other) const
	{
		return uploadDate < other.uploadDate;	}

	bool isMore(const Item & other) const
	{
		return other.uploadDate < uploadDate;	}

	bool isLess(const DateTime & otheruploadDate) const
	{
		return uploadDate < otheruploadDate;	}

	bool isMore(const DateTime & otheruploadDate) const
	{
		return otheruploadDate < uploadDate;	}


	
	static Item fromDb(SqlConnection & sql, const DateTime & _uploadDate);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Item & orig) const;

};
inline bool operator==(const Item & a, const Item & b) { return a.isEqual(b); }
inline bool operator!=(const Item & a, const Item & b) { return !a.isEqual(b); }
inline bool operator<(const Item & a, const Item & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Item & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const DateTime & a, const Item & b) { return b.isMore(a); }
inline bool operator<(const Item & a, const DateTime & b) { return a.isLess(b); }

class ItemSet : public csjp::SorterOwnerContainer<Item>
{
	public:
	csjp::PodArray<Item::Fields> ordering;
	bool ascending;

	const ItemSet & operator=(ItemSet && temp)
	{
		csjp::SorterOwnerContainer<Item>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Item & a, const Item & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Item::UploadDate :
				ret = a.uploadDate < b.uploadDate;
				break;
			case Item::Name :
				ret = a.name < b.name;
				break;
			case Item::Unit :
				ret = a.unit < b.unit;
				break;
			case Item::Type :
				ret = a.type < b.type;
				break;
			case Item::Brand :
				ret = a.brand < b.brand;
				break;
			case Item::Quantity :
				ret = a.quantity < b.quantity;
				break;
			case Item::Price :
				ret = a.price < b.price;
				break;
			case Item::Currency :
				ret = a.currency < b.currency;
				break;
			case Item::Account :
				ret = a.account < b.account;
				break;
			case Item::Partner :
				ret = a.partner < b.partner;
				break;
			case Item::Inventory :
				ret = a.inventory < b.inventory;
				break;
			case Item::Comment :
				ret = a.comment < b.comment;
				break;
			case Item::InvChangeDate :
				ret = a.invChangeDate < b.invChangeDate;
				break;
			case Item::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Item::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.uploadDate < b.uploadDate;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	ItemSet() :
		csjp::SorterOwnerContainer<Item>(),
		ascending(true)
	{
		ordering.add(Item::UploadDate);
		ordering.add(Item::Name);
		ordering.add(Item::Unit);
		ordering.add(Item::Type);
		ordering.add(Item::Brand);
		ordering.add(Item::Quantity);
		ordering.add(Item::Price);
		ordering.add(Item::Currency);
		ordering.add(Item::Account);
		ordering.add(Item::Partner);
		ordering.add(Item::Inventory);
		ordering.add(Item::Comment);
		ordering.add(Item::InvChangeDate);
		ordering.add(Item::LastModified);
		ordering.add(Item::Deleted);
	}
	ItemSet(const ItemSet & ts) :
		csjp::SorterOwnerContainer<Item>(ts),
		ascending(true)
	{
		ordering.add(Item::UploadDate);
		ordering.add(Item::Name);
		ordering.add(Item::Unit);
		ordering.add(Item::Type);
		ordering.add(Item::Brand);
		ordering.add(Item::Quantity);
		ordering.add(Item::Price);
		ordering.add(Item::Currency);
		ordering.add(Item::Account);
		ordering.add(Item::Partner);
		ordering.add(Item::Inventory);
		ordering.add(Item::Comment);
		ordering.add(Item::InvChangeDate);
		ordering.add(Item::LastModified);
		ordering.add(Item::Deleted);
	}
	~ItemSet() {}


	static ItemSet fromDb(SqlConnection & sql, const ItemQuery & q, ItemQueryStat & stat);
};

class Payment{
	public:

	DateTime uploadDate;
	Text account;
	Text partner;
	Double amount;
	DateTime subject;
	DateTime payDate;
	Text comment;
	DateTime lastModified;
	YNBool deleted;
	enum Fields {
		UploadDate,
		Account,
		Partner,
		Amount,
		Subject,
		PayDate,
		Comment,
		LastModified,
		Deleted,
		NumOfFields

		, NumOfAllFields
	};

public:
	Payment() {}

	explicit Payment(const DateTime & uploadDate) :
		uploadDate(uploadDate)
	{}

	explicit Payment(const Payment & orig) :
		uploadDate(orig.uploadDate),
		account(orig.account),
		partner(orig.partner),
		amount(orig.amount),
		subject(orig.subject),
		payDate(orig.payDate),
		comment(orig.comment),
		lastModified(orig.lastModified),
		deleted(orig.deleted)
	{}

	Payment(Payment && temp) :
		uploadDate(csjp::move_cast(temp.uploadDate)),
		account(csjp::move_cast(temp.account)),
		partner(csjp::move_cast(temp.partner)),
		amount(csjp::move_cast(temp.amount)),
		subject(csjp::move_cast(temp.subject)),
		payDate(csjp::move_cast(temp.payDate)),
		comment(csjp::move_cast(temp.comment)),
		lastModified(csjp::move_cast(temp.lastModified)),
		deleted(csjp::move_cast(temp.deleted))
	{}

	~Payment() {}

	csjp::Json toJson() const
	{
		csjp::Json json;
	{
		csjp::String s;
		s <<= uploadDate;
		json["uploadDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= account;
		json["account"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= partner;
		json["partner"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= amount;
		json["amount"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= subject;
		json["subject"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= payDate;
		json["payDate"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= comment;
		json["comment"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= lastModified;
		json["lastModified"] = move_cast(s);
	}
	{
		csjp::String s;
		s <<= deleted;
		json["deleted"] = move_cast(s);
	}
		return json;
	}

	Payment& operator=(Payment && temp)
	{
		uploadDate = csjp::move_cast(temp.uploadDate);
		account = csjp::move_cast(temp.account);
		partner = csjp::move_cast(temp.partner);
		amount = csjp::move_cast(temp.amount);
		subject = csjp::move_cast(temp.subject);
		payDate = csjp::move_cast(temp.payDate);
		comment = csjp::move_cast(temp.comment);
		lastModified = csjp::move_cast(temp.lastModified);
		deleted = csjp::move_cast(temp.deleted);
		return *this;
	}

	Payment& operator=(const Payment & orig)
	{
		Payment copy(orig);
		csjp::swap(copy, *this);
		return *this;
	}

	bool isEqual(const Payment & other) const
	{
		if(
				uploadDate != other.uploadDate ||
				account != other.account ||
				partner != other.partner ||
				amount != other.amount ||
				subject != other.subject ||
				payDate != other.payDate ||
				comment != other.comment ||
				lastModified != other.lastModified ||
				deleted != other.deleted
			)
			return false;
		return true;
	}

	bool isLess(const Payment & other) const
	{
		return uploadDate < other.uploadDate;	}

	bool isMore(const Payment & other) const
	{
		return other.uploadDate < uploadDate;	}

	bool isLess(const DateTime & otheruploadDate) const
	{
		return uploadDate < otheruploadDate;	}

	bool isMore(const DateTime & otheruploadDate) const
	{
		return otheruploadDate < uploadDate;	}


	
	static Payment fromDb(SqlConnection & sql, const DateTime & _uploadDate);
	void toDb(SqlConnection & sql) const;
	void toDb(SqlConnection & sql, const Payment & orig) const;

};
inline bool operator==(const Payment & a, const Payment & b) { return a.isEqual(b); }
inline bool operator!=(const Payment & a, const Payment & b) { return !a.isEqual(b); }
inline bool operator<(const Payment & a, const Payment & b) { return a.isLess(b); }

inline csjp::String & operator<<=(csjp::String & lhs, const Payment & rhs)
				{ lhs = rhs.toJson().toString(); return lhs; }

inline bool operator<(const DateTime & a, const Payment & b) { return b.isMore(a); }
inline bool operator<(const Payment & a, const DateTime & b) { return a.isLess(b); }

class PaymentSet : public csjp::SorterOwnerContainer<Payment>
{
	public:
	csjp::PodArray<Payment::Fields> ordering;
	bool ascending;

	const PaymentSet & operator=(PaymentSet && temp)
	{
		csjp::SorterOwnerContainer<Payment>::operator=(move_cast(temp));
		return *this;
	}

	virtual int compare(const Payment & a, const Payment & b) const
	{
		bool ret;

		switch(ordering[0]) {
			case Payment::UploadDate :
				ret = a.uploadDate < b.uploadDate;
				break;
			case Payment::Account :
				ret = a.account < b.account;
				break;
			case Payment::Partner :
				ret = a.partner < b.partner;
				break;
			case Payment::Amount :
				ret = a.amount < b.amount;
				break;
			case Payment::Subject :
				ret = a.subject < b.subject;
				break;
			case Payment::PayDate :
				ret = a.payDate < b.payDate;
				break;
			case Payment::Comment :
				ret = a.comment < b.comment;
				break;
			case Payment::LastModified :
				ret = a.lastModified < b.lastModified;
				break;
			case Payment::Deleted :
				ret = a.deleted < b.deleted;
				break;
			default:
				ret =
					a.uploadDate < b.uploadDate;
				break;
		}

		if(!ascending)
			ret = !ret;

		return ret ? -1 : 1;
	}

public:
	PaymentSet() :
		csjp::SorterOwnerContainer<Payment>(),
		ascending(true)
	{
		ordering.add(Payment::UploadDate);
		ordering.add(Payment::Account);
		ordering.add(Payment::Partner);
		ordering.add(Payment::Amount);
		ordering.add(Payment::Subject);
		ordering.add(Payment::PayDate);
		ordering.add(Payment::Comment);
		ordering.add(Payment::LastModified);
		ordering.add(Payment::Deleted);
	}
	PaymentSet(const PaymentSet & ts) :
		csjp::SorterOwnerContainer<Payment>(ts),
		ascending(true)
	{
		ordering.add(Payment::UploadDate);
		ordering.add(Payment::Account);
		ordering.add(Payment::Partner);
		ordering.add(Payment::Amount);
		ordering.add(Payment::Subject);
		ordering.add(Payment::PayDate);
		ordering.add(Payment::Comment);
		ordering.add(Payment::LastModified);
		ordering.add(Payment::Deleted);
	}
	~PaymentSet() {}


	static PaymentSet fromDb(SqlConnection & sql, const PaymentQuery & q, PaymentQueryStat & stat);
};







inline QStringList & operator<<=(QStringList & list, const WareTypeSet & wareTypes)
{
	list.clear();
	for(auto & wareType : wareTypes)
		list.append(wareType.type);
	return list;
}

inline QString & operator<<=(QString & str, const WareTypeSet & wareTypes)
{
	str.clear();
	for(auto & wareType : wareTypes){
		if(str.length())
			str.append(", ");
		str.append(wareType.type);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.add(new Text(wareTag.tag));
	return list;
}

inline QStringList & operator<<=(QStringList & list, const WareTagSet & wareTags)
{
	list.clear();
	for(auto & wareTag : wareTags)
		list.append(wareTag.tag);
	return list;
}

inline QString & operator<<=(QString & str, const WareTagSet & wareTags)
{
	str.clear();
	for(auto & wareTag : wareTags){
		if(str.length())
			str.append(", ");
		str.append(wareTag.tag);
	}
	return str;
}

inline QString & operator<<=(QString & str, const ItemQueryWithTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const ItemQueryWithTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const ItemQueryWithoutTagSet & queryTags)
{
	str.clear();
	for(auto & queryTag : queryTags){
		if(str.size())
			str.append(", ");
		str.append(queryTag.tag);
	}
	return str;
}

inline StringSet & operator<<=(StringSet & list, const ItemQueryWithoutTagSet & queryTags)
{
	list.clear();
	for(auto & queryTag : queryTags)
		list.add(new Text(queryTag.tag));
	return list;
}

inline QString & operator<<=(QString & str, const ItemQueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}

inline QString & operator<<=(QString & str, const PaymentQueryPartnerSet & partners)
{
	str.clear();
	for(auto & partner : partners){
		if(str.size())
			str.append(", ");
		str.append(partner.partner);
	}
	return str;
}

inline QString & operator<<=(QString & str, const ItemQueryWareSet & wares)
{
	str.clear();
	for(auto & ware : wares){
		if(str.size())
			str.append(", ");
		str.append(ware.ware);
	}
	return str;
}

inline bool operator==(const WareTypeSet & a, const QString & b)
{
	QStringList sl;
	sl = b.split(",", Qt::SkipEmptyParts);
	unsigned s = sl.size();
	if(s != a.size())
		return false;
	unsigned i;
	for(i=0; i<s; i++)
		if(!a.has(Text(sl.at(i).trimmed())))
			return false;
	return true;
}

inline bool operator==(const WareTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const WareTagSet & a, const StringSet & b){ return !(a == b); }

inline bool operator==(const ItemQueryWithTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const ItemQueryWithTagSet & a, const StringSet & b){ return !(a == b); }

inline bool operator==(const ItemQueryWithoutTagSet & a, const StringSet & b)
{
	if(a.size() != b.size())
		return false;
	unsigned s = a.size();
	for(unsigned i = 0; i < s; i++)
		if(a[i].tag != b[i])
			return false;
	return true;
}

inline bool operator!=(const ItemQueryWithoutTagSet & a, const StringSet & b){ return !(a == b); }

#endif

