#!/usr/bin/env python

try:
    from setuptools import setup, find_packages, Extension
except ImportError:
    from ez_setup import use_setuptools
    use_setuptools()
    from setuptools import setup, find_packages, Extension

import os
import sys
import os.path

extra_compile_args = []
if sys.platform == "win32" and sys.version >= '2.4':
    extra_compile_args = '/Zp1 /Og /Ob2 /Oi /Ot /GS'.split()

METADATA = {
    'name':         "pytrees",
    'version':      "1.0",
    'author':       "Adam Ever-Hadani",
    'author_email': "adamhadani@gmail.com",
    'description':  "Tree data structures and algorithms in C++ / Python"
}

setup(
    py_modules = ["TST"],
    ext_modules = [
        Extension(
            "_TST",
            sources = ["TST.i"],
            include_dirs = ['include'],
            extra_compile_args="-Xlinker -export-dynamic -Wl,-O3 -Wl,-Bsymbolic-functions".split(),
            swig_opts = ["-c++"]
        )
    ],

    packages = find_packages(),  
    zip_safe = False,

    **METADATA
)
