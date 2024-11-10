# C Armyknife Library

A small fully open sourced C extension library.

## Features

* an extremely easy to use command line parser

* LOG as easy as printf.

```
    log_info("Splitting %s off from %s", token_to_string(node->name),
              token_to_string(struct_node->name));
```

* a fail fast fatal error mechanism.

* our allocator always zeros on allocation

* enhanced or at least more readable C utf-8 string operations:
  string_append, string_hash, encoding/decoding UTF-8 strings to code
  points, string_left_pad, ...

* a byte buffer abstraction that makes it really easy to accumulate
  bytes in memory, treat or extract sub-parts as UTF-8 strings, etc.

* growable arrays of "primitive values" (so anything that fits in
  64bits including pointers...)

* three similar interfaces for "maps" that provide time/space
  tradeoffs
  * string_alist for very small maps
  * string_ht for medium to very large maps
  * string_tree for sorted maps

* cross-platform *deterministic* random. (Useful for unit tests for
  one thing.)

* a test suite

* a [companion documentation
  project](https://github.com/jasonaaronwilson/c-javadoc-extractor)
  so you can write your Javadoc style documentation in markdown and
  extract them to simple markdown files.

* a non-Posix sub_process facility which I hope already works on MacOS
  and can be ported to Windows.

## Use Cases

This is the library I'm using to "self-host" a C like compiler,
omni-c, my retro-futuristic version of the C language. You don't have
to buy into that vision to use c-armyknife-lib.

## Links

* [Source Code Documentation](src-doc/README.md)

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

This library is under active development as of 2024-11-10. The
[TODO](TODO.md) file in this directory has a small road-map to help
get to version 1.0 so you can see a bit about what I'm thinking.

`make test` coverage will increase over time (especially once I figure
out how to actually measure coverage...).

I'm experimenting on the development side by having no header files
though as a user you don't have to care about this: the result of
"make" is a "single header file library" which is one of the few sane
ways to distribute C software right now unless your library is a
standard part of common linux distributions.

