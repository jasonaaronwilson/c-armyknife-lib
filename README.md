# C Armyknife Library

A small fully open sourced C library. My initial focus is writing
small text based command line utilities using only essential POSIX
functionality.

## Features

* an extremely easy to use command line parser that even allows git
  style subcommands if you want that. "--help" is essentially free
  though you can easily make it much better with descriptions of flags
  (and commands).

* a logging mechanism that's literally as easy to use as "printf"

```
    log_info("Splitting %s off from %s", token_to_string(node->name),
              token_to_string(struct_node->name));
```

   (It's near zero cost when logging at that level isn't
   enabled...)
  
* a fatal error mechanism (potentially with backtraces though not
  close to Java quality yet; also easily catch segmentation faults
  turning them into fatal errors; and make it trivial to attach a
  debugger on error by delaying exit if requested)

  (Either Ubuntu or Mint locked this down...)

* a somewhat safer take on memory allocation

   * always zero on init if you use my safe allocator

   * an optional debug LRU based over-write/under-write checker
     perfect for small programs like tests that caught errors that
     other tools didn't for me.

* basic "immutable" string operations for example, string_append; and
  also, string_hash, and decoding UTF-8 strings (and even
  string_left_pad)

* growable arrays of primitive values (that can be trivally used as a
  stack though don't expect such an array to ever shrink)

* three similar interfaces for "maps" that provide time/space
  tradeoffs
  * string_alist for small maps
  * string_ht for large maps
  * string_tree for (large) sorted maps

* cross-platform *deterministic* random. Useful for unit tests.

* a [companion documentation
  projecct](https://github.com/jasonaaronwilson/c-javadoc-extractor)
  so you can write your Javadoc style documentation in markdown and
  extract them to simple markdown files.

* a quasi corporate quality test suite (but not thread safe, that's
    not yet a property I'm going after yet)

  * My test runner makes it seems like there are only 18 tests. If you
    search for "void test_" in this directory you'll get more like
    74.

    A metric that is interesting is "points of test failure" so if you
    look for test_assert, you'll find more than than 225.

    Since I'm testing collections there are loops that do work and
    then a loop that does test_asserts and so the true number of test
    asserts is much higher than than the static number. (I'm going to
    totally waste effort mesuaring this once I figure out how to
    side-channel it...)

## Links

* [Source Code Documentation](src-doc/README.md)

## Limitations

Primitive "values" must fit into the same size as uint64_t which
probably means storing a pointer for most user defined data. While the
syntax is mostly convenient when working with "values", the compiler
is NOT able to do meaningful type checking. I'm considering several
ways of generating code (which is what the C preprocessors does after
all) and it would be nice if all of that was also written in C.

(We never want to force you to change code, Omni C 

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

