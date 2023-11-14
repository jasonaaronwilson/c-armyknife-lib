# C Armyknife Library

A small C library with a focus on command line utilities.

## Features

* fatal errors (with backtraces which are OK if you use the -rdynamic
  compiler option though not close to Java quality yet)
* "immediate" exit on any memory allocation failure plus zeroing
  malloc to reduce undefined behavior
  * with an LRU based over-write/under-write checker perfect for small
    programs like tests
* basic "immutable" string operations for example, string_append,
  string_hash, and even string_left_pad
* growable arrays of primitive values (that can be trivally used as a
  stack)
* three similar interfaces for "maps" of string -> primitive values.
  * string_alist for small maps
  * string_ht for large maps
  * string_tree for sorted maps
* command line parsing helper that handles GNU style "long" flags
  (short flags will obviously be supported eventually).
* a cross-platform deterministic random, and a few other small but
  useful routines.
* a [companion documentation
  projecct](https://github.com/jasonaaronwilson/c-javadoc-extractor)
  so you can write your Javadoc style documentation in markdown and
  extract them to simple markdown files.
* a test runner (bash script) suitable for smaller projects

## Links

* [Source Code Documentation](src-doc/README.md)

## Limitations

Primitive "values" must fit into the same size as uint64_t which
probably means storing a pointer for most user defined data. While the
syntax is mostly convenient when working with "values", the compiler
is NOT able to do meaningful type checking. I'm considering several
ways of generating code (which is what the C preprocessors does after
all) and it would be nice if all of that was also written in C.

### Memory Deallocation

While some thought was put into "malloc" to make programs have less
undefined behavior, tests and sample programs are not valgrind clean
yet.

For substantial tools, you probably want to use
https://en.wikipedia.org/wiki/Boehm_garbage_collector though simply
not freeing anything is another option for simple tools that don't
generate much intermediate garbage.

## Compatibility

Unit tests run with gcc, clang, and tcc on x86-64 Linux.

```
gcc --version
gcc (Debian 10.2.1-6) 10.2.1 20210110

clang --version
Debian clang version 11.0.1-2
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

tcc --version
tcc version 0.9.27 (x86_64 Linux)
```

## Status

This library is under active development as of 2023-11-07. The
[TODO](TODO.md) file in this directory has a small road-map to help get
to version 1.0 so you can see a bit about what I'm thinking.

`make test` coverage will increase over time (especially once I figure
out how to actually measure coverage...).

I'm experimenting on the development side by having no header files
though as a user you don't have to care about this: the result of
"make" is a "single header file library" which is one of the few sane
ways to distribute C software right now unless your library is a
standard part of common linux distributions.

