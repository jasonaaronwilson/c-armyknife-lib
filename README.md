# Armyknife C Library

This is a "single source file" library that adds a touch of
convenience and consistency above libc alone.

It provides very little actually:

* consistent _t type naming convention (even for "built-in" types like
  bool)
* fatal errors with backtrace
  * though not an exensible set of errors yet...
* safer zeroing malloc and free to reduce UB (undefined behavior)
* a buffer abstraction that plays a major role in io (and will grow
  more important over time I think).
* basic string operations (maybe even left pad functionality someday)
* easy to use string-->void* maps (hashtables and association lists
  including iterator macros).
* easy to grow arrays of pointers (poor mans "collect")
* an ambitious plan for more generic tuples, arrays, and hashtables
  that **seems to be to painful to use in practice** so I will
  probably change course.

## Status

The simpler stuff is largely unit tested to a trivial degree and I'm
eating my dogfood in at least one other program in active development
that relies on c-armyknife-lib.

More interesting is the whole way [header file
extraction](https://github.com/jasonaaronwilson/c-single-source-file)
is kind of working out not only on how to structure this library but
also for a static binary target though there are hitches.

## Test Infrastructure

I'm defining/using a test infrastructure that is essentially a single
bash script that produces output that seems readable to me.

`runtests.sh` simply invokes a bunch of executables and tells you
which ones returned OK (aka exit code zero) or NOT OK (any other exit
code). This may be a fit to your small to medium sized projects.

