#!/usr/bin/env python

import TST

t = TST.TSTStr()

t.insert("Elad Pitt")
t.insert("Brad Pita")

for i in xrange(100000):
	res = t.near_search("Brad Pitt", 2)

for i in xrange(1000):
	print "res: ", res
