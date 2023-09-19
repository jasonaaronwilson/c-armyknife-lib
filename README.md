# Armyknife C Library

This is a "single source file" style library that adds enough to
functionality to C to write real programs:

* fatal errors
* safer malloc and free
* dynamically sized arrays (and specifically byte-arrays to make IO
  easier)
* more string operations (maybe even left pad functionality someday)
* a dynamic type (like "any" in Go) - can be used to make
  optionals and error signaling easier while providing a basis for
  generic containers
* hashtables
* readability improvements like boolean_t (in the eye of the beholder
  perhaps)

## Status

Collections support doesn't exist yet.  I'm think still thinking about
how to do dynamic-type checking for collections to make them easy and
safe to use. Once I have hashtables, dynamic arrays will be trivial to
implement.






