inih                        {#inih}
====

[inih](http://code.google.com/p/inih/) is a simple .INI file 
parser written in C, released under the New BSD
license (see LICENSE.txt). Go to the project home page for more info:

[code.google.com/p/inih](http://code.google.com/p/inih/)

struct_ini_t was asdded to store all sections and name-value pairs in a single
structure. It does not respect the rule that no aitown specifics must be avoided
in external, hosted projects. This will be eventually resolved by removing
the dependencies (specifically, the AITOWN_EXPORT macro) and adding library
specific content.
