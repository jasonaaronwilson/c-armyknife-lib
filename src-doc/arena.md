# @file arena.c

(experimental)

This is an optional memory allocator that can be used to replace
allocate.c. Similar concepts to an arena are memory pools and
especially "bump-allocation".

Arena style allocation is especially well suited to long-lived
request based architectures like web-servers. For simple
command-line utilities, simply never calling free isn't the worst
option in the world on modern machines with 64bit address spaces
(and lots of RAM) though a little bit of arena parameter tuning may
be necessary to handle some cases more perfomantly.

Gemini suggested Meson, APR (pools), MicroAllocator, and MPS as
possible arena libraries you could use instead (though wiring those
up correctly into c-armyknife-lib would make it harder to provide
as a single-file library though I would accept PULL requests if
anyone wants to give it a try - just make sure it is opt-in of
course and folks should always have this arena implementation as a
fall-back). Like most of c-armyknife-lib, the emphasis is therefore
on correctness and simplicity over performance/memory-efficiency.

Since I didn't ask Gemini the right question, it didn't talk about
[Boehm garbage
collector](https://en.wikipedia.org/wiki/Boehm_garbage_collector). Initially
allocate.c was only meant to be a very thin shim on top of C's
malloc/free that would simply allow turning a knob to enable using
the Boehm collector (which I think would still be interesting to
"provide" at some point so I will probably do that), however
allocate.c evolved to have some interesting debugging options
(probablistic under-write/over-write detection) which may be kept
here.

Arenas allows you to generally avoid *having* to make many
individual free calls as everything allocated in an arena can be
de-allocated with a single "fixed" cost arena_close() call which is
especially beneficial for amortizing the cost of small
allocations. While this does not in any way guarantee memory safety
(Rust's ownership/borrow model and other languages models like
garbage collection are definitely safer), the predictability of
arenas and the usage of the virtual memory system (when possible)
will generally allow certain use after free errors to be more
evident and thus found earlier.

c-armyknife-lib's arenas fully support free however arenas are
generally configured to ignore free calls up to a certain size so
that the additional tracking over-head for small allocations is
avoided - a bit like having your cake and eating it too - at least
to an extent.

If you do lots of allocations and frees of objects smaller in size
than 128bits, then certain C library implementations of malloc may
perform way better for certain use cases in which case allocate.c
may be the better option. Of course you may not know this initially
and once you go the arena route, it may be harder to put back into
place all of the free statements we allow you to avoid by using
arenas.
 
## @struct arena_t

An arena_t represents both the abstract notion of an arena as well
as the head of a list of arena segments. Our hope is that arenas
are initialized in such as way that you don't end up with
additional segments very often so we will collect enough data to
help you figure that out when you need to.

The general idea is going to be to put a arena_t* into a
thread-local variable initialized from main. For now we will just
use a simple "global" variable (the_current_arena) instead since
c-armyknife-lib's only client, me, has yet to use
multi-threading. Using a thread-local means we can effectively use
a "fluid-let" *like* (this is C) to control (via dynamic scoping)
what the current arena is which means we don't have to explicitly
pass-around the arena just to do an allocation in some remote leaf
function. While explicit data-dependency to this level has some
advantages, it's the kind of non-sense that will turn people off
and everyone will just write Python or something.
 
## @macro free_bytes

This is essentially the same as free.
 
## @macro malloc_bytes

This is essentially the same as malloc but the memory is always
zeroed before return it to the user. We use a macro here to call
checked_malloc so that the file and line number can be passed.
 
## @macro malloc_copy_of

This provides a convenient way to allocate a copy of a given
"source". Generally you would only use it with a pointer to a
structure though in theory it could be used on other things.

See also: string_duplicate which automatically calls strlen, etc.
 
## @macro malloc_struct

This provides a convenient way to allocate a zero-filled space big
enough to hold the given structure with sizeof automatically used
and the result automatically casted to a pointer to the given type.
 
## @function arena_checked_malloc

Allocate the given amount bytes or cause a fatal error. The memory
is also zeroed.

If possible, use the macros malloc_bytes or malloc_struct instead
for an easier to use interface. Those macros simply call
checked_malloc.
 
## @function checked_free

Allocate amount bytes or cause a fatal error. The memory is also
zeroed.

If possible, use the macros malloc_bytes or malloc_struct instead
for an easier to use interface. Those macros simply call
checked_malloc.
 
## @function checked_malloc_copy_of

Allocate amount bytes and initialize it with a copy of that many
bytes from source.
 
## @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING

The amount of padding to place at the back of each allocation. We
have several ways to then check that no one overwrites this padding
though we won't catch every case.

Unlike start padding, this does not effect alignment and so values
as small as 1 make perfect sense though I still recommend 4 or 8
bytes especially on 64bit big-endian architectures.
 
## @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE

This determine how big the lossy hashtable is. On every allocation
or deallocation the *entire* lossy hashtable is scanned to see if
the padding bytes have been perturbed which makes it possible to
find some memory overwrite errors earlier than waiting for the free
call (or potentially even if the memory isn't ever freed).

It makes no sense to set this unless either
ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING or
ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING is non-zero.
 
## @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT

This is just a fail-safe to catch extremely dumb allocation amounts
(at least as of 2023). If you know you will have a fixed size
amount of memory, you could set this lower and potentially get a
slightly nicer error message.
 
## @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING

The amount of padding to place in front of each allocation. We have
several ways to then check that no one overwrites this padding
though we won't catch every case.

This should be a multiple of 8 or else the expected alignment
(which malloc doesn't make that explicit...) will be broken.
 
