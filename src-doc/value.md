# @file value.c

A major part of the armyknife library is giving basic "collection"
capabilities to C which is why we have values but it also turns out
the library seems more consistent if a few non collection based
routines also use a similar interface to communicate "failures"
with the caller hence optional_value_result_t's are used as the
result of string functions. A mental model is that
optional_value_result_t can only result from "query" kinds of
operations that can return before changing the global state (OR
they can restore the state to the same condition, unlikely for many
failures which is why we have fatal-error.c which currrently just
terminates the program because that is the safest thing to do in
most circumstance. If you attempt to look past the end of an array,
that is a fatal error though "trying" to parse an integer, that is
an optional_value_result_t.)

value_t's are typically only passed into functions while
optional_value_result_t's are then typically returned from
functions. When a optional_value_result_t is returned you must
therefore explictly check the error conditions and while you should
always do this, I don't know how to make the C compiler agree that
you at least kind of pretended to look at the error code which is
something that standard C could do but wouldn't do without
pressure, aka, common use ).

value_t's and optional_value_result_t's carry no type information
that can be queried at runtime and by their nature, most C
compilers are going to do a very incomplete job of statically type
checking these. For example you can easily put a double into a
collection and successfully get back a very suspicious pointer and
the compiler will not warn you about this. On the positive side,
this also means you haven't paid a price to prevent these errors at
runtime and so in theory your code can run faster. If C had a
richer type-system, I think we could catch all such errors at
compile time (and this would not make code any slower than this
approach since by definition, static compilation errors will not
change how correct code runs).

Both value_t's and optional_value_result_t's have a fixed sized. On
a native 64bit architecture, a value_t can be passed in a single
register and optional_value_result_t can be returned in two
registers and while I have no comparitive benchmarks to prove this
strategy is in fact efficient, one has to wonder why this was put
into most 64bit C calling conventions (aka ABIs) if no one was
actually trying to make this efficient.

value_t's are meant only to used used when putting associations
into a collection where you have now given up on having static type
checking in standard C because C lacks generic types. You gain
functionality without embracing inheritance or a feature not
supported by standard C but you lose quite a bit of type safety.

While a value_t can't hold *everything* it can hold a pointer to
almost anything so it's a bit like Java's notion of a primitive
value plus Java's notion of a pointer mixed into a very unsafe
union. If this seems unsafe, which of course it is, I have some
news for you if you think C is ever safe.

The most common things to key over are strings, integers, and
pointers (while common values are strings, integers, pointers and
booleans). We make these convient but not quite type safe though
you can make things safer using typedef and inline functions for
the pointer case if your are willing to take that step.

Sometimes other techniques like unit testing work quite well at
finding these errors as well as higher level bugs so we recommend
that and even provide a very simple framework to do that.
 
## @function is_not_ok

Return true if the given value_result_t contains an error, such as
NF_ERROR_NOT_FOUND.
 
## @function is_ok

Return true if the given value_result_t contains a legal value
instead of an error condition.
 
## Non fatal errors.

These may only occur as part of query interface.
 
