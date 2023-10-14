# Armyknife C Library

The armyknife C library is a "single source file" library that adds a
touch of convenience and consistency above libc alone.

It provides:

* consistent "_t" type naming convention (even for "built-in" types
  like bool which becomes boolean_t)
* fatal errors with backtrace (better with -rdynamic compiler option)
  * though not an exensible set of errors (yet... give me some time)
* safer "zeroing" malloc to reduce UB (undefined behavior)
* a buffer abstraction that plays a major role in io (and will grow
  more important over time).
* basic string operations (maybe even left pad functionality someday)
* easy arrays of primitive values that can grow
* three similar interfaces for "maps" of string -> primitive values.
  * string_alist for small maps
  * string_ht for large maps
  * string_tree for sorted maps

## Philosophy

Meant for "quick and dirty" coding which we all love. Code clarity and
*convenience* is more important than absolute performance.

## Limitations

Primitive values must fit into the same size as uint64_t which
probably means storing a pointer rather than inlined data. I actually
attempted inlined data (see "the attic") but it became confusing even
to me while I was down in the code so I've opted to scale that goal
back. Many languages such as Java achieve high performance despite
essentially having the same restriction so this itself isn't a reason
to dismiss this library (memory allocation, overly opinionated error
handling, etc. are practical concerns tough).

While the syntax is convenient when working with "values", the
compiler is NOT able to do meaningful type checking. I would not
attempt to put different kinds of things into the same collection for
example -- there is no tagging and values are stored in a "raw"
fashion.

While some thought was put into "malloc" to make programs have less
undefined behavior, in general assume nothing is free'd properly by
the library. Go lang has garbage collection, Rust has it's borrow
model, and Swift has ARC. Our model here is quick and dirty -- modern
machines have lots of memory so as long as you aren't tackling the
biggest problems that take hours or days to run, just don't worry
about it at first.

For any substantial tool, you probably want to use
https://en.wikipedia.org/wiki/Boehm_garbage_collector though I plan to
add some support for user defined allocators especially "arenas".

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
as of 2023-10-13.

`make test` coverage will increase over time (especially once I figure
out how to measure coverage).

I'm experimenting on the development side by having no header files
though as a user, the result is a "single header file library" which
is one of the few sane ways to distribute C software right now. Even
if you have no interest in this library, you may be interested on how
I put it together.

## Test Infrastructure

I'm using a test infrastructure that has been taken down to the bare
nuts and bolts: any executable program that exits with "0" is a
passing test (and anything else is a failure).

Like single header file libraries, I believe this is a useful
abstraction for small projects and you should cargo-cult the heck out
of it. In about 15 minutes I went from only testing with gcc to
testing with gcc, clang, and tcc.
