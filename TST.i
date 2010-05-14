/* TST.i 
 # Python SWIG Interface definition file
 # for the TST (Ternary Search Tree) library.
 #
 */

%define DOCSTRING
"Python SWIG bindings for the TST (Ternary Search Tree)
data structures and algorithms library.
To instantiate, Use one of the templated subtypes (TSTStr)."
%enddef

%module(docstring=DOCSTRING) TST

%feature("autodoc", "1");

%include "./include/TST.h"

/* Instantiate template specialization
   for the char (str) case.
*/
%template(_TST_char) TST<char>;

%typemap(in) UINT {
        $1 = (int)PyInt_AsLong($input);
}

%typemap(out) PythonReference {
    $result = $1.lend();
}

%include "./include/TSTpy.h"

%{
#include "./include/TSTpy.h"
%}
