# Armyknife C Library

The armyknife C library is a "single source file" library that adds a
touch of convenience and consistency above libc alone.

It provides:

* consistent "_t" type naming convention (even for "built-in" types
  like bool which becomes boolean_t)
* fatal errors with backtrace (best with -rdynamic compiler option)
  * though not an exensible set of errors (yet... give me some time)
* safer "zeroing" malloc to reduce UB (undefined behavior)
* a buffer abstraction that plays a major role in io (and will grow
  more important over time) and string construction.
* basic "immutable" string operations for example 64bit string_hash,
  string_append, and string_left_pad.
* easy growable arrays of primitive values (also acts like a stack)
* three similar interfaces for "maps" of string -> primitive values.
  * string_alist for small maps
  * string_ht for large maps
  * string_tree for sorted maps
* deterministic random, and a few other small be useful routines
* simple command line parsing (GNU style long flags are put into
  hashtables and everything else collected as "file" arguments).

## Philosophy

Meant for "quick and dirty" coding which we all love. Code clarity,
correctness and *convenience* are more important than absolute
performance. Where necessary, structures are used to return multiple
values (for example all "find" methods return if the item was found in
additional to the item/value). C compilers are actually pretty good at
handling small structures and passing pointers into C routines as
"out" paremeters is often less convenient.

## Limitations

Primitive values must fit into the same size as uint64_t which
probably means storing a pointer rather than inlined data for user
defined data. [^1]

While the syntax is convenient when working with "values", the
compiler is NOT able to do meaningful type checking. I would not
attempt to put different kinds of things into the same collection for
example -- there is no tagging and values are stored in a "raw"
fashion.

### Memory Deallocation

While some thought was put into "malloc" to make programs have less
undefined behavior, in general assume nothing is free'd properly by
the library. Go has garbage collection, Rust has it's borrow model,
and Swift has ARC, etc.. Our model here is quick and dirty -- modern
machines have lots of memory so as long as you aren't tackling the
biggest problems that take hours or days to run, just don't worry
about it at first (and then worry about the biggest data-structures
first, for example files read into memory, etc.)

For any substantial tool, you probably want to use
https://en.wikipedia.org/wiki/Boehm_garbage_collector though I plan to
add some support for user defined allocators especially "arenas". We
will make it easier to employ the Boehm collector with a single ifdef
before including the header file.

## Compatibility

Unit tests are clean with gcc, clang, and tcc on x86-64.

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

I'm eating my own dogfood and this library is under active development
as of 2023-10-13. I think this library is about version 0.1 right now.

`make test` coverage will increase over time (especially once I figure
out how to measure coverage).

I'm experimenting on the development side by having no header files
though as a user you don't have to care about this: the result is a
"single header file library" which is one of the few sane ways to
distribute C software right now unless your library is a standard part
of a linux distribution. Even if you have no interest in this library,
you may be interested on how I put it together (including the simple
test infrastructure, see below).

## Test Infrastructure

I'm using a test infrastructure that has been taken down to the bare
nuts and bolts: any executable program that exits with "0" is a
passing test (and anything else is a failure). All tests are therefore
just shell scripts that build and run (via another shell script) plus
usually a single C source file (which obviously includes the same
single-header file that is exported to users). In fact, this second
script now builds and runs each test for gcc, clang, and tcc. This
extra step seems wasteful 

Like single header file libraries, I believe this is a useful
abstraction for small projects and feel free to cargo-cult the heck
out of it though I may try to split this off into its own small git
repository.

[^1]: I actually attempted inlined data (see "the attic") but it
became confusing even to me while I was writing the code (let alone in
six months) so I've opted to scale that goal back. Many languages such
as Java achieve high performance despite essentially having the same
restriction so this itself isn't a reason to dismiss this library
(memory allocation, overly opinionated error handling, etc. are
practical concerns tough).
