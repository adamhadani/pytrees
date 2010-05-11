#!/usr/bin/env python
"""
test.py

Unit-test code for the TST library.
"""

import sys

import unittest
import os
import random
import string
import datetime

from TST import TSTStr
import ahocorasick

############################################################################################################

random.seed(0)

############################################################################################################

class TestMemory(unittest.TestCase):
	def setUp(self):
		self.tst = TSTStr()
		self.s = "Brad Pitt"
		self.rc_none = sys.getrefcount(None)
		self.rc_s = sys.getrefcount(self.s)

	def testSetup(self):
		self.assertEqual(sys.getrefcount(None), self.rc_none)

	def testInsert(self):
		self.tst.insert(self.s)
		self.assertEqual(self.tst.search(self.s), True)
		self.assertEqual(sys.getrefcount(self.s), self.rc_s)

############################################################################################################

class TestBenchmark(unittest.TestCase):
	def setUp(self):
		print "Setting up test..."
		
		self.tst = TSTStr(10000000)
		self.aho = ahocorasick.KeywordTree()
		self.d = {}
		self.actor_names = []
		
		print "* Reading string dictionary from file..."
		
		fh = open("actor_names.txt", "r")
		for l in fh: 
			if len(l.strip()) > 1:
				self.actor_names.append( l.strip().lower() )
		fh.close()
		
		self.actor_names = self.actor_names[:10000000]
		self.rev_actor_names = [i[::-1] for i in self.actor_names]
		
		
	def runTest(self):
		actor_names = self.actor_names
		rev_actor_names = self.rev_actor_names
		
		tst = self.tst
		aho = self.aho
		d = self.d
		
		print "Benchmarking INSERT operation (%i items)" % len(actor_names)

		indice = range(len(actor_names))
		random.shuffle(indice)
								
		print "* Inserting actor names DB to tree..."
		
		start_t = datetime.datetime.now()

		for l in indice:
			tst.insert(actor_names[l])
			#tst[actor_names[l]] = None
			
		end_t = datetime.datetime.now()

		t_insert_tree = end_t-start_t
		print "* Inserted %i names to tree in %s" % (len(self.actor_names), t_insert_tree)

		print "* Inserting actor names DB to hashtable..."

		start_t = datetime.datetime.now()

		for l in indice:
			d[actor_names[l]] = None

		end_t = datetime.datetime.now()

		t_insert_hash = end_t-start_t

		print "* Inserted %i names to hash dictionary in %s" % (len(d.keys()), t_insert_hash)

		print "Benchmarking SEARCH NO-MATCH operation (%i items)" % len(actor_names)
		
		start_t = datetime.datetime.now()
		
		for l in rev_actor_names:			
			ret = tst.search(l)
			#self.assert_(ret is False, "Found non-existant key: %s" % l)
			
		end_t = datetime.datetime.now()

		t_search_nomatch_tree = end_t-start_t

		print "* Searched %i names in tree in %s" % (len(actor_names), t_search_nomatch_tree)

		print "* Benchmarking search on hash dictionary..."
		print "Hash contains %i entries" % len(d.keys())
		
		start_t = datetime.datetime.now()
		
		for l in rev_actor_names:
			ret = d.has_key(l)
			#self.assert_(ret is False, "ret was not False for: %s" % l)
			
		end_t = datetime.datetime.now()

		t_search_nomatch_hash = end_t-start_t

		print "* Searched %i names in hash dictionary in %s" % (len(actor_names), t_search_nomatch_hash)
		
		
############################################################################################################
		
if __name__ == "__main__":
	print "* Running unittest suite for libTST..."

	suite = unittest.TestSuite((
		unittest.makeSuite(TestMemory),
		unittest.makeSuite(TestBenchmark)
	))

	unittest.TextTestRunner().run(suite)

