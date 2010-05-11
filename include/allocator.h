/*
 # allocator.h
 # Author: Adam Ever-Hadani <adamhadani@videosurf.com>
 # Copyright (c) 2008 Videosurf Inc.
 #
 # libTST
 # Implement Tries / Ternary Search Trees data structure and algorithm.
 #
 # This header defines the memory storage objects that can be used by a TST.
 #
*/

#ifndef ALLOCATOR_H_INCLUDED
#define ALLOCATOR_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <assert.h>
#include <vector>
#include <list>

#include "boost/pool/object_pool.hpp"
#include "boost/pool/pool_alloc.hpp"

#include "common.h"

using std::vector;
using std::list;
//using std::slist;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Class>
 class HeapPoolAlloc {
public:
    HeapPoolAlloc(UINT init_size) {}
    virtual ~HeapPoolAlloc() {}

    virtual void reserve(size_t size) =0;
    virtual void purge_memory() =0;

    virtual Class *allocate() =0;
    virtual void deallocate(Class *p) =0;
 };

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Class>
 class BoostHeapPoolAlloc : public HeapPoolAlloc<Class> {
public:
    typedef Class *tptr;

    BoostHeapPoolAlloc(UINT init_size) :
        HeapPoolAlloc<Class>(init_size),
        m_memPool(init_size)
    {
    }

    virtual ~BoostHeapPoolAlloc()
    {
        purge_memory();
    }

    void reserve(size_t size)
    {
        // Reserve is not supported after initialization.
    }

    void purge_memory()
    {
        // Purging should be done by deleting this object
        // De-allocation is taken care of by boost::object_pool
    }

    // Memory management
    tptr allocate()
    {
        tptr p = m_memPool.construct();

        return p;
    }

    void deallocate(tptr p)
    {
        m_memPool.free(p);
    }

protected:
    boost::object_pool<Class> m_memPool;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Class>
 class BoostVecHeapPoolAlloc : public HeapPoolAlloc<Class> {
public:
    typedef Class *tptr;

    BoostVecHeapPoolAlloc(UINT init_size) :
        HeapPoolAlloc<Class>(init_size),
        m_memPool(init_size),
        m_nPos(0)
    {

    }

    virtual ~BoostVecHeapPoolAlloc()
    {
        purge_memory();
    }

    void reserve(size_t size)
    {
        m_memPool.reserve(size);
    }

    void purge_memory()
    {
        // Purging should be done by deleting this object
        // De-allocation is taken care of by boost::object_pool
    }

    // Memory management
    tptr allocate()
    {
        size_t szPool = m_memPool.size();

        if ( m_nPos+1 == szPool )
        {
                m_memPool.resize(szPool*2);
        }


        return &m_memPool[m_nPos++];
    }

    void deallocate(tptr p)
    {
        // TODO: Implement de-allocator
    }

protected:
    std::vector<Class, boost::fast_pool_allocator<Class> > m_memPool;
    UINT m_nPos;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Class>
 class BoostListHeapPoolAlloc : public HeapPoolAlloc<Class> {
public:
    typedef Class *tptr;

    BoostListHeapPoolAlloc(UINT init_size) :
        HeapPoolAlloc<Class>(init_size),
        m_memPool(init_size),
        m_nPos(0)
    {

    }

    virtual ~BoostListHeapPoolAlloc()
    {
        purge_memory();
    }

    void reserve(size_t size)
    {
        // No reserves in node-based storage (list)
    }

    void purge_memory()
    {
        // Purging should be done by deleting this object
        // De-allocation is taken care of by boost::object_pool
    }

    // Memory management
    tptr allocate()
    {
        m_memPool.push_front( Class() );

        return &m_memPool.front();
    }

    void deallocate(tptr p)
    {
        // TODO: Implement de-allocator
    }

protected:
    std::list<Class, boost::fast_pool_allocator<Class> > m_memPool;
    UINT m_nPos;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // ALLOCATOR_H_INCLUDED
