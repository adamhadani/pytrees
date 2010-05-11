/*
main.cpp

Ternary search trees implementation.

*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "common.h"
#include "TST.h"
#include "TSTpy.h"

using std::string;
using std::vector;

typedef struct tnode *Tptr;
typedef struct tnode {
   char splitchar;
   Tptr lokid, eqkid, hikid;
} Tnode;

int rsearch(Tptr p, const char *s)
{
    if (!p) return 0;

    if ( *s < p->splitchar)
        return rsearch(p->lokid, s);
    else if ( *s > p->splitchar)
        return rsearch(p->hikid, s);
    else {
        if (*s == 0) return 1;
        return rsearch(p->eqkid, ++s);
    }
}

int search(Tptr root, const char *s)
{
    Tptr p;
    p = root;

    while (p) {
        if ( *s < p->splitchar )
            p = p->lokid;
        else if ( *s == p->splitchar )
        {
            if ( *s++ == 0 ) return 1;
            p = p->eqkid;
        }
        else
            p = p->hikid;

    }

    return 0;
}

Tptr insert(Tptr p, const char *s)
{
    if (p == NULL)
    {
        p = (Tptr)malloc(sizeof(Tnode));
        p->splitchar = *s;
        p->lokid = p->hikid = p->eqkid = NULL;
    }

    if (*s < p->splitchar)
        p->lokid = insert(p->lokid, s);
    else if (*s == p->splitchar)
    {
        if (*s != 0)
            p->eqkid = insert(p->eqkid, ++s);
    }
    else
    {
        p->hikid = insert(p->hikid, s);
    }

    return p;
}

char **read_dict(char* fname)
{
    char **buf = NULL;


    return buf;
}

void test_from_dict(const char* fname)
{
    TSTStr tree(32);

    vector<string> strings;
    vector<string>::iterator it;
    string line;
    std::ifstream myfile (fname);

    strings.push_back( "Brad Peat" );
    strings.push_back( "Elad Pitt" );

    if (myfile.is_open())
    {
        printf("Reading strings from file...\n");

        /*
        int n=0;
        while (! myfile.eof() )
        {
            getline(myfile, line);
            strings.push_back( line );
            n++;
        }*/

        printf("Inserting from collection to tree...\n");
        for (it = strings.begin(); it != strings.end(); ++it)
        {
            tree.insert( (*it).c_str() );

        }
        printf("Done.\n");
        myfile.close();
    }

    const char *s1 = "Brad Pitt";

    //BOOL ret = tree.search(s1);
    int ret = tree.nn_search(s1, 2);

    std::vector<char *>::iterator it2;
    for (it2=tree.m_vecNearMatches.begin();it2 != tree.m_vecNearMatches.end(); ++it2)
    {
        printf("Match: %s\n", *it2);
    }

    printf("%s in tree: %i\n", s1, ret);

}

int main(int argc, char* argv[])
{
    test_from_dict("actor_names.txt");

    return 0;
}
