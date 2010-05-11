from distutils.core import setup, Extension
import sys

extra_compile_args = []
if sys.platform == "win32" and sys.version >= '2.4':
	extra_compile_args = '/Zp1 /Og /Ob2 /Oi /Ot /GS'.split()

setup(
    name = "TST",
    version = "1.0",
    author = "Adam Ever-Hadani",
    author_email = "adamhadani@gmail.com",
    description = "Tries / Ternary Search Trees in C++ / Python",
    py_modules = ["TST"],
    ext_modules = [
        Extension(
            "_TST",
            sources=["TST_wrap.cxx"],
            include_dirs=['include'],
			extra_compile_args="-Xlinker -export-dynamic -Wl,-O3 -Wl,-Bsymbolic-functions".split()
			#extra_compile_args="-Xlinker -export-dynamic -shared".split(" ")
            #define_macros=[('SCANNER', None),],
        )
    ]
)
