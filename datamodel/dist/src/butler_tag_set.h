/* vim:set noet ai sw=8 ts=8 sts=8: tw=80 cino="W2s,c1s,C1,+2s,i2s,t0,g0,l1,:0" */
/* =>s,e0,n0,f0,{0,}0,^0,:s,=s,l0,b0,gs,hs,ps,ts,is,+s,c3,C0,/0,(2s,us,U0,w0,
 * W0,m0,j0,)20,*30,#0 */
/** 
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2009 Csaszar, Peter
 */

#ifndef BUTLER_TAG_SET_H
#define BUTLER_TAG_SET_H

#include <QObject>
#include <QString>
#include <QList>
#include <QMap>
#include <QVariant>

#include <ButlerTag>

namespace Butler {

	/* Methods accepting pointers in their parameters will take the
	 * ownership of the pointed object. */
	class TagSet : public QObject
	{
		private:
			Q_OBJECT;
		public:
			TagSet();
			~TagSet();

			explicit TagSet(const TagSet &tagSet);
			TagSet& operator=(const TagSet&);

			void append(Tag *t);
			void move(int from, int to);
			void remove(int i);
			void clear();

			const Tag& at(int i) const;

			bool empty() const;
			int size() const;

			void sort();
			void swap(int i, int j);

			static bool isEqual(const TagSet &a, const TagSet &b);

		private:
			void equal(const TagSet &ts);

			QList<Tag*> data;
			QMap<const QString*, const Tag*> nameToPtr;
	};

	bool operator==(const TagSet &a, const TagSet &b);
	bool operator!=(const TagSet &a, const TagSet &b);

}

#endif

