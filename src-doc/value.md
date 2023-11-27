## @enum non_fatal_error_code_t

These are user recoverable errors and when a non-recoverable error
is returned, the state of the system should be left in a
recoverable state.
 
# @file value.c

A major part of the armyknife library is giving basic "collection"
capabilities to C.

In an ideal world, C would allow structures to be parameterized so
that a field holding a "key" or a "value" could change size (based
on its static parameters).

Since that capability doesn't currently exist, instead we take an
approach where keys and values are always the same size (64
bits). While this doesn't allow storing *anything* as a key or
value, it does allow storing a *pointer to anything* as a key or
value. This is actually very similar to how Java collections work
except we can store primitive values like integers and doubles
without boxing.

When "searching" a collection for a key, we want to be able to
return "not found" (and potentially other "non-fatal" error
conditions). For this reason we also have the value_result_t
structure, which is a pair of a value_t and a
non_fatal_error_code_t. (It's actually slightly more complicated
because `tcc` treats anonymous unions a little differently than
`gcc` and `clang` so we work around that by repeating the fields of
a value to make value_result_t a bit more convenient to work with).

value_result_t is sometimes used by non collection based functions,
such as parsing an integer, so that other non-fatal errors can be
communicated back to the caller.

The contract with returning a non-fatal errors is that the state of
the system is in a good state to continue processing (or they get
to decide if the error should be fatal). Users will thus expect
that when a non-fatal error is returned that no global state
modification has ocurred.

value_t's are typically only passed into functions while
optional_value_result_t's are then typically returned from
functions.

When a value_result_t is returned you must always check for an
error code before using the value component of the result. `is_ok`
and `is_not_ok` make this slightly easier.

value_t's and value_result_t's carry no type information that can
be queried at runtime and by their nature C compilers are going to
do a very incomplete job of statically type checking these. For
example you can easily put a double into a collection and
successfully get back a very suspicious pointer and the compiler
will not warn you about this. So collections aren't as safe as in
other languages at either compile or run-time. (Java's collections
(when generic types are *not* used), are not "safe" at compile time
but are still dynamically safe.)

On the positive side, this also means you haven't paid a price to
prevent these errors at runtime and so in theory your code can run
faster.

If C had a richer type-system, namely generic types,, I think we
could catch all all potential type errors at compile time and even
allow storing "values" larger than 64bits "inline".

The most common things to use as keys are strings, integers, and
pointers (while common values are strings, integers, pointers and
booleans). We make these convient but not quite type safe though
you can make things a little safer by using typedef and inline
functions.
 
## @function is_not_ok

Return true if the given value_result_t contains an error, such as
NF_ERROR_NOT_FOUND.
 
## @function is_ok

Return true if the given value_result_t contains a legal value
instead of an error condition.
 
## @typedef value_result_t

A pair of a value_t and a non-fatal error code. When the error code
is set, the value_t should never be looked at (most likely will be
"zero" or a "nullptr" but you shouldn't trust that).
 
## @typedef value_t

An un-typed union of integers, doubles, and pointers.
 
