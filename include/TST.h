/*
 # TST.h
 # Author: Adam Ever-Hadani <adamhadani@videosurf.com>
 # Copyright (c) 2008 Videosurf Inc.
 #
 # libTST
 # Implement Tries / Ternary Search Trees data structure and algorithm.
 #
*/

#ifndef TST_H_INCLUDED
#define TST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "common.h"
#include "allocator.h"

// Flag set means entire string
// copy will be allocated by terminal
// node pointing to this string
#define NODE_STORE_STRING 0x01

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Class Prototypes

template<typename Char_t>
class TSTNode;


template<typename Char_t,
         typename Allocator_t = BoostVecHeapPoolAlloc<TSTNode<Char_t> > >
class TST;


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TST Class decleration
template<typename Char_t, typename Allocator_t>
class TST {
public:
    typedef TSTNode<Char_t> *node_ptr;

    TST(int init_size = 32, BYTE flags = 0x00);
    virtual ~TST();

    void insert(const Char_t *s);
    TSTNode<Char_t> *insert(node_ptr p, const Char_t *s);

    bool search(const Char_t *s);

    int nn_search(const Char_t *s, int d);
    void nn_search(node_ptr p, const Char_t *s, int d);

    std::vector<char*> m_vecNearMatches;

private:
    node_ptr       m_pRoot;
    Allocator_t    m_memStorage;
    BYTE           m_Flags;

};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TST Class method definitions

template<typename Char_t, typename Allocator_t>
TST<Char_t, Allocator_t>::TST(int init_size, BYTE flags) :
    m_pRoot(NULL),
    m_memStorage(init_size),
    m_Flags(flags)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Char_t, typename Allocator_t>
TST<Char_t, Allocator_t>::~TST()
{
    // Invalidate pointers
    m_pRoot = NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Char_t, typename Allocator_t>
void TST<Char_t, Allocator_t>::insert(const Char_t *s)
{
    if (s == NULL || *s == 0)
        return;

    int d;

    const char *s_start = s;
    node_ptr pp, *p;
    p = &m_pRoot;

    while ( (pp = *p) ) {
        if ((d = *s - pp->m_splitchar) == 0)
        {
            if (*s++ == 0)
                return;

            p = &(pp->m_pEqNode);
        }
        else if (d < 0)
            p = &(pp->m_pLowNode);
        else
            p = &(pp->m_pHighNode);
    }

    for (;;) {
        *p = m_memStorage.allocate();
        pp = *p;
        pp->m_Flags = m_Flags;
        pp->m_splitchar = *s;

        if (*s++ == 0) {
            // Reached terminal node for word, store pointer to original string
            // Note that it is the user's responsibility to make sure this string
            // is not deleted if NODE_STORE_STRING is false.
            if (m_Flags & NODE_STORE_STRING)
            {
                char *new_str = new char[strlen(s_start)+1];
                strcpy(new_str, s_start);
                pp->m_pEqNode = (node_ptr)new_str;
            }
            else
                pp->m_pEqNode = (node_ptr)s_start;

            return;
        }

        p = &(pp->m_pEqNode);
    }

/*
    //assert(s != NULL && *s != 0);
    if (s == NULL || *s == 0)
        return;

    m_pRoot = insert(m_pRoot, s);
*/
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 @DEPRECATED
 Iterative version used instead above.
*/
template<typename Char_t, typename Allocator_t>
TSTNode<Char_t> * TST<Char_t, Allocator_t>::insert(node_ptr p, const Char_t *s)
{
    if (p == NULL)
    {
        p = m_memStorage.allocate();

        assert(p != NULL);

        p->m_Flags = m_Flags;

        p->m_splitchar = *s;
        p->m_pLowNode = p->m_pHighNode = p->m_pEqNode = NULL;
    }

    if (*s < p->m_splitchar)
        p->m_pLowNode = insert(p->m_pLowNode, s);
    else if (*s == p->m_splitchar)
    {
        if (*s != 0)
            p->m_pEqNode = insert(p->m_pEqNode, ++s);
    }
    else
    {
        p->m_pHighNode = insert(p->m_pHighNode, s);
    }

    return p;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Char_t, typename Allocator_t>
bool TST<Char_t, Allocator_t>::search(const Char_t *s)
{
    if (s == NULL || *s == 0)
        return FALSE;

    int d, sc;
    TSTNode<Char_t> *p = m_pRoot;
    sc = *s;

    while (p) {
        if ((d = sc - p->m_splitchar) == 0)
        {
            if (sc == 0)
            {
                return TRUE;
            }

            sc = *(++s);
            p = p->m_pEqNode;
        }
        else if (d < 0)
            p = p->m_pLowNode;
        else
            p = p->m_pHighNode;
    }

    return FALSE;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Char_t, typename Allocator_t>
int TST<Char_t, Allocator_t>::nn_search(const Char_t *s, int d)
{
    // Reset near matches container
    m_vecNearMatches.clear();

    nn_search(m_pRoot, s, d);

    return static_cast<int>( m_vecNearMatches.size() );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename Char_t, typename Allocator_t>
void TST<Char_t, Allocator_t>::nn_search(node_ptr p, const Char_t *s, int d)
{
    if (!p || d < 0) return;

    if (d > 0 || *s < p->m_splitchar)
        nn_search(p->m_pLowNode, s, d);

    if (p->m_splitchar == 0) {
        if ((int) strlen(s) <= d)
        {
            // Found match, store in our matches collection
            m_vecNearMatches.push_back( (char *)p->m_pEqNode );
        }
    }
    else
       nn_search(p->m_pEqNode, *s ? s+1:s,
                (*s == p->m_splitchar) ? d:d-1);

    if (d > 0 || *s > p->m_splitchar)
        nn_search(p->m_pHighNode, s, d);
}

// TSTNode Class decleration

template<typename Char_t>
class TSTNode {
public:
    friend class TST<Char_t>;

    TSTNode(BYTE flags = 0x00) :
        m_splitchar(),
        m_Flags(flags),
        m_pLowNode(NULL),
        m_pHighNode(NULL),
        m_pEqNode(NULL)
    {
    }

    ~TSTNode()
    {

        // If is a terminal node and storing a string,
        // Free string allocation.
        if (m_Flags & NODE_STORE_STRING)
        {
            if (m_splitchar == 0 &&
                m_pEqNode != NULL)
            {
                //delete (char *)m_pEqNode;
            }
        }

        // Invalidate pointers
        m_pLowNode =
            m_pHighNode =
            m_pEqNode =
            NULL;

    }

private:
    // 'Splitting' character stored
    Char_t  m_splitchar;

    // One byte used for any additional flags
    BYTE m_Flags;

    // Pointers to child nodes
    TSTNode<Char_t> *m_pLowNode;
    TSTNode<Char_t> *m_pHighNode;
    TSTNode<Char_t> *m_pEqNode;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// TSTNode Class method definitions

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // TST_H_INCLUDED
