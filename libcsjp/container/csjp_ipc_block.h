/*
 * Author: Csaszar, Peter <csjpeter@gmail.com>
 * Copyright (C) 2020-2020 Csaszar, Peter
 */

#ifndef CSJP_IPC_BLOCK_H
#define CSJP_IPC_BLOCK_H

#include <string.h>
#include <csjp_object.h>
#include <csjp_string.h>

namespace csjp {

/**
 * To be used to pass on objects from one thread to another without
 * locking.
 *
 * The size can only be defined during construction time.
 *
 * One thread should only push objects,
 * the other should only pop objects.
 *
 * This container is a FIFO / LILO one. Thus the object
 * was pushed first will be the one oped first.
 *
 * Stores pointers of objects. As long as stored owns them and
 * on destroy deletes any stored object.
 */

template <typename DataType>
class IpcBlock
{
public:
	explicit IpcBlock(const IpcBlock<DataType> & orig) = delete;

	const IpcBlock & operator=(const IpcBlock<DataType> & orig) = delete;

	IpcBlock(IpcBlock<DataType> && temp) = delete;

	const IpcBlock & operator=(IpcBlock<DataType> && temp) = delete;

public:
	explicit IpcBlock(size_t s) :
		cap(0),
		counter(0),
		beginPos(NULL),
		endPos(NULL),
		pushIter(NULL),
		popIter(NULL),
		capacity(cap),
		count(counter)
	{
		DataType **dst = (DataType **)realloc(beginPos, sizeof(DataType*) * (s + 1));
		if(!dst)
			throw OutOfMemory("No enough memory for IpcBlock allocation with "
					"% number of elements.", s);

		cap = s;
		beginPos = dst;
		endPos = beginPos + cap;
		pushIter = beginPos;
		popIter = beginPos;
	}

	/**
	 * Runtime:		linear, O(n)	<br/>
	 */
	virtual ~IpcBlock()
	{
		for(DataType ** i = beginPos; i < endPos; i++)
			delete *i;
		if(beginPos)
			free(beginPos);
	}

private:
	size_t cap; // capacity
	size_t counter; // counter of currently held objects
	DataType **beginPos;
	DataType **endPos;
	DataType **pushIter;
	DataType **popIter;
	/*
	 * Invariants:
	 */
public:
	const size_t & capacity;
	const size_t & count;

public:
	/**
	 * Runtime:		linear, O(n)	<br/>
	 */
	void push(Object<DataType> & t)
	{
		if (*pushIter != 0)
		{
			throw Exception("IPC buffer overflow.");
		}

		*pushIter = t.release();
		counter++;
		pushIter++;
		if (pushIter == endPos)
			pushIter = beginIter;
	}

	void push(DataType * && dt)
	{
		Object<DataType> t(dt);
		dt = 0;
		push(t)
	}

	void push(DataType && dt)
	{
		Object<DataType> t(new DataType(dt));
		push(t)
	}

	/**
	 * Runtime:		linear, O(n)	<br/>
	 */
	Object<DataType> pop()
	{
		Object<DataType> o(*popIter);
		if (*popIter == 0)
			return o;

		*popIter = 0;
		counter--;
		popIter++;
		if (popIter == endPos)
			popIter = beginIter;
		return o;
	}
};

}

#endif
