/*
 # TSTpy.h
 # Author: Adam Ever-Hadani <adamhadani@videosurf.com>
 # Copyright (c) 2008 Videosurf Inc.
 #
 # libTST
 # Implement Tries / Ternary Search Trees data structure and algorithm.
 #
 # This header defines a class specialization used for the Python bindings.
 #
*/

#ifndef _TSTPY_H_INCLUDE
#define _TSTPY_H_INCLUDE

#include <stdio.h>
#include <vector>

#include "Python.h"
#include "PythonReference.h"
#include "common.h"

#include "TST.h"


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class TSTStr : public TST<char>
{
public:

    TSTStr(int init_size=32, BYTE flags=NODE_STORE_STRING):
        TST<char>(init_size, flags)
    {}

    virtual ~TSTStr() {}

    PythonReference __getitem__(const char*& s)
    {
        return search(s) ?
            PythonReference(Py_True):
            PythonReference(Py_False);
    }

    PythonReference __contains__(const char*& s)
    {

        return search(s) ?
            PythonReference(Py_True):
            PythonReference(Py_False);
	}

	PythonReference near_search(const char*& s, int d)
	{
        int nRes = nn_search(s, d);

        if (!nRes)
            return PythonReference(Py_None);

        PyObject *pyList = PyList_New(m_vecNearMatches.size());

        assert(pyList);

        // Create python list from result set
        std::vector<char *>::iterator it;
        UINT nPos=0;
        for (it=m_vecNearMatches.begin(); it != m_vecNearMatches.end(); ++it)
        {
                //PyObject *pyStr = PyString_FromString(*it);
                PyObject *pyStr = Py_BuildValue("s", *it);

                assert(pyStr);
                //PythonReference pRef(pyStr);

                PyList_SetItem(pyList, nPos++, pyStr);

                //Py_DECREF(pyStr);
        }

        return PythonReference(pyList);
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif // _TSTPY_H_INCLUDE
