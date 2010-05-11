#!/usr/bin/env python

import sys
import os
import random
import string
import datetime
import cProfile as profile

from TST import TSTStr

def benchmark_insert():
	fh = open("actor_names.txt", "r")
	n=0
	tst = TSTStr()
	
	print "* Inserting actor names DB to tree..."

	start_t = datetime.datetime.now()
	for l in fh:
		tst.insert(l.strip())
		n+=1
	end_t = datetime.datetime.now()

	t = end_t-start_t
	print "* Inserted %i names to tree in %s" % (n, t)

	fh.seek(0)
	d = {}
	n=0
	print "* Inserting actor names DB to hashtable..."

	start_t = datetime.datetime.now()
	for l in fh:
		d[l.strip()] = None
		n+=1
	end_t = datetime.datetime.now()

	t = end_t-start_t

	print "* Inserted %i names to hash dictionary in %s" % (len(d.keys()), t)

	fh.close()

profile.run("benchmark_insert()")
