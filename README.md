# Armyknife C Library

This is a "single source file" style library that adds enough to
functionality to C to write real programs:

* fatal errors
* safer malloc and free
* dynamically sized byte-arrays
* more string operations (maybe even left pad functionality someday)
* a dynamic result type (aka any - can be used to make optional and errors)
* hashtables, dynamically sized arrays (and maybe a balanced tree
  collection)
* readability improvements like boolean_t

## Status

Collections support doesn't exist yet.  I'm think still thinking about
how to do dynamic-type checking for collections to make them easy and
safe to use. Once I have hashtables, dynamic arrays will be trivial to
implement.






