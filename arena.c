#line 2 "arena.c"

/**
 * @file arena.c
 *
 * (experimental)
 *
 * This is an optional memory allocator that can be used to replace
 * allocate.c. Similar concepts to an arena are memory pools and
 * especially "bump-allocation".
 *
 * Arena style allocation is especially well suited to long-lived
 * request based architectures like web-servers. For simple
 * command-line utilities, simply never calling free isn't the worst
 * option in the world on modern machines with 64bit address spaces
 * (and lots of RAM) though a little bit of arena parameter tuning may
 * be necessary to handle some cases more perfomantly.
 *
 * Gemini suggested Meson, APR (pools), MicroAllocator, and MPS as
 * possible arena libraries you could use instead (though wiring those
 * up correctly into c-armyknife-lib would make it harder to provide
 * as a single-file library though I would accept PULL requests if
 * anyone wants to give it a try - just make sure it is opt-in of
 * course and folks should always have this arena implementation as a
 * fall-back). Like most of c-armyknife-lib, the emphasis is therefore
 * on correctness and simplicity over performance/memory-efficiency.
 *
 * Since I didn't ask Gemini the right question, it didn't talk about
 * [Boehm garbage
 * collector](https://en.wikipedia.org/wiki/Boehm_garbage_collector). Initially
 * allocate.c was only meant to be a very thin shim on top of C's
 * malloc/free that would simply allow turning a knob to enable using
 * the Boehm collector (which I think would still be interesting to
 * "provide" at some point so I will probably do that), however
 * allocate.c evolved to have some interesting debugging options
 * (probablistic under-write/over-write detection) which may be kept
 * here.
 *
 * Arenas allows you to generally avoid *having* to make many
 * individual free calls as everything allocated in an arena can be
 * de-allocated with a single "fixed" cost arena_close() call which is
 * especially beneficial for amortizing the cost of small
 * allocations. While this does not in any way guarantee memory safety
 * (Rust's ownership/borrow model and other languages models like
 * garbage collection are definitely safer), the predictability of
 * arenas and the usage of the virtual memory system (when possible)
 * will generally allow certain use after free errors to be more
 * evident and thus found earlier.
 *
 * c-armyknife-lib's arenas fully support free however arenas are
 * generally configured to ignore free calls up to a certain size so
 * that the additional tracking over-head for small allocations is
 * avoided - a bit like having your cake and eating it too - at least
 * to an extent.
 *
 * If you do lots of allocations and frees of objects smaller in size
 * than 128bits, then certain C library implementations of malloc may
 * perform way better for certain use cases in which case allocate.c
 * may be the better option. Of course you may not know this initially
 * and once you go the arena route, it may be harder to put back into
 * place all of the free statements we allow you to avoid by using
 * arenas.
 */

#ifndef _ARENA_H_
#define _ARENA_H_

#ifdef C_ARMYKNIFE_LIB_USE_ARENAS

// A free list navigates it way backwards through time occupying the
// space formally used by live objects and obviously that space may be
// reused again at some point or there would be no need for
// free-lists!
//
// Free lists are shared across segments and segments are generally
// going to be allocated by the OS and may be subject to address space
// randomization such that an older segment is actually virtually
// addressed at a higher memory location than a newer segment at least
// some of the time. AFAIK, only a rare breed of compacting memory
// collector would preserve the property that a lower address means
// allocated earlier in time so never count on that property even
// though you might be tempted to do that.
//
// Since free lists are generally only going to be used for large
// objects, we might want to spend a little more time looking through
// them to ameliorate fragmentation though the first implementation is
// going to be first fit within a certain size bucket which will at
// least bound our wasted space. Keeping the free list sorted might
// improve best fit but let's not worry about that right now.
//
// Our buckets are initially going to be just log_2 though I think we
// can do something like log_1.5 or even lower if necessary.
//
// If you query an LLM to write malloc/free it will at least comment
// on how you should try to combine adjacent free items into bigger
// blocks to prevent memory fragmentation. With arenas, the main way
// to defragment memory is closing arenas so we aren't going to even
// think about combining adjacent free items, it's just wasted code
// that will probably be wrong.
typedef struct arena_free_list_S {
  uint64_t allocatable_size;
  struct arena_free_list_S* next;
} arena_free_list_t;

// This may need to be bigger if we use log_1.5 etc.
#define MAX_FREE_LIST_BUCKETS 64

// The first M buckets are going to always be NULL based on
// minimum_free_size below. We are not going to complicate the code to
// worry about this fact.
typedef struct arena_free_lists_S {
  arena_free_list_t*[MAX_FREE_LIST_BUCKETS] buckets;
} arena_free_lists_t;

/**
 * @struct arena_t
 *
 * An arena_t represents both the abstract notion of an arena as well
 * as the head of a list of arena segments. Our hope is that arenas
 * are initialized in such as way that you don't end up with
 * additional segments very often so we will collect enough data to
 * help you figure that out when you need to.
 * 
 * The general idea is going to be to put a arena_t* into a
 * thread-local variable initialized from main. For now we will just
 * use a simple "global" variable (the_current_arena) instead since
 * c-armyknife-lib's only client, me, has yet to use
 * multi-threading. Using a thread-local means we can effectively use
 * a "fluid-let" *like* (this is C) to control (via dynamic scoping)
 * what the current arena is which means we don't have to explicitly
 * pass-around the arena just to do an allocation in some remote leaf
 * function. While explicit data-dependency to this level has some
 * advantages, it's the kind of non-sense that will turn people off
 * and everyone will just write Python or something.
 */
typedef struct arena_S {
  // This is essentially a bump pointer. Although I saw at least one
  // argument for growing arena's backwards, the current
  // implementation will grow upwards for now. The general allocation
  // interface doesn't specify the direction of growth but we are
  // keeping it simple and predictable by growing from lower to 
  //
  // When current > end, this means that this arena (segment) and all
  // of it's sibling segments have been closed (deallocated). It's
  // definitely a problem to try to allocate into such an arena, look
  // at previous segments, look at objects between start and end,
  // etc. Normally we would use "nullptr" to indicate this condition
  // because it's kind of more natural in the C world but setting to
  // end means we can use a single comparision rather than two
  // comparisions before using this as a bump pointer in the common
  // case of allocating a small object that will fit into the current
  // segment.
  void* current;

  // Allocations below the minimum_free_size are essentially untracked
  // and can't be freed until the arena itself is closed while
  // allocations above this size utilize free lists and thus might be
  // reused (for other large allocations). Setting this to
  // "(uint64_t)(-1)" essentially turns this implementation into a
  // pure "bump-allocator" arena/memory-pool. Setting this to 0 is a
  // stress-test that should mimic allocate.c (the malloc based
  // allactor) more closely though not exactly.
  uint64_t minimum_free_size;

  // This is the address of the start of the arena. Pretend I just
  // wrote uint64_t here.
  void* start;

  // This is the first address that is greater than the memory
  // allocated to the arena. Pretend I just wrote uint64_t here.
  void* end;

  // When an allocation exceeds minimum_free_size, we are obligated to
  // look here first (looking at it below this size should actually be
  // harmless...) Free lists will potentially span multiple "segments"
  // where segments must at least have the same parent.
  arena_free_lists_t* free_lists;

  // This is the (rounded up to at least a 4K page size) number of
  // bytes for each "segment" of an arena. When utilizing malloc
  // instead of mmap(), you may want to keep this value low. This is
  // stored explicitly since in order to support objects larger than
  // the segment size, `end - start` is not necessarily the right
  // default when allocating a new segment.
  uint64_t arena_segment_size;

  // A list of the older "segments" that are logically part of this
  // logical arena. All will share the same parent and closing this
  // arena will also close all previous_segments.
  struct arena_S* previous_segments;

  // This is probably unnecessary. Apparently meson is not hierachial
  // while APR pools are hierarchical. As long as we require that
  // children are properly closed (which in light of "exceptions" in a
  // language without exception handling, may not be trivial to
  // achieve...), then this mostly serves a debugging/profiling role
  // which we will need lots of to make this work.
  struct arena_S* parent;

  // TODO(jawilson): any OS specific stuff to support un-memory
  // mapping this particular segment.
  
} arena_t;

#include <stdint.h>

extern uint8_t* arena_checked_malloc(char* file, int line, uint64_t amount);
extern uint8_t* arena_checked_malloc_copy_of(char* file, int line, uint8_t* source, uint64_t amount);
extern void arena_checked_free(char* file, int line, void* pointer);

// TODO(jawilson):
//
// arena_open();
// arena_close();
// get_current_arena(); ?
// statistics, logging, and debugging ?

/**
 * @macro malloc_bytes
 *
 * This is essentially the same as malloc but the memory is always
 * zeroed before return it to the user. We use a macro here to call
 * checked_malloc so that the file and line number can be passed.
 */
#define malloc_bytes(amount) (arena_checked_malloc(__FILE__, __LINE__, amount))

/**
 * @macro free_bytes
 *
 * This is essentially the same as free.
 */
#define free_bytes(ptr) (arena_checked_free(__FILE__, __LINE__, ptr))

/**
 * @macro malloc_struct
 *
 * This provides a convenient way to allocate a zero-filled space big
 * enough to hold the given structure with sizeof automatically used
 * and the result automatically casted to a pointer to the given type.
 */
#define malloc_struct(struct_name)                                             \
  ((struct_name*) (arena_checked_malloc(__FILE__, __LINE__, sizeof(struct_name))))

/**
 * @macro malloc_copy_of
 *
 * This provides a convenient way to allocate a copy of a given
 * "source". Generally you would only use it with a pointer to a
 * structure though in theory it could be used on other things.
 *
 * See also: string_duplicate which automatically calls strlen, etc.
 */
#define malloc_copy_of(source, number_of_bytes)                                \
  (arena_checked_malloc_copy_of(__FILE__, __LINE__, source, number_of_bytes))

// TODO(jawilson): malloc_copy_of_struct

#endif /* C_ARMYKNIFE_LIB_USE_ARENAS */

#endif /* _ALLOCATE_H_ */

// ======================================================================

#ifdef C_ARMYKNIFE_LIB_USE_ARENAS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

boolean_t is_initialized = false;
boolean_t should_log_value = false;

uint64_t number_of_bytes_allocated = 0;
uint64_t number_of_malloc_calls = 0;
uint64_t number_of_free_calls = 0;

static inline boolean_t should_log_memory_allocation() {
  if (is_initialized) {
    return should_log_value;
  }
  char* var = getenv("ARMYKNIFE_LOG_MEMORY_ALLOCATION");
  is_initialized = true;
  if (var != NULL && strcmp(var, "true") == 0) {
    should_log_value = true;
  }
  return should_log_value;
}

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
 *
 * The amount of padding to place in front of each allocation. We have
 * several ways to then check that no one overwrites this padding
 * though we won't catch every case.
 *
 * This should be a multiple of 8 or else the expected alignment
 * (which malloc doesn't make that explicit...) will be broken.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
#define ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING 8
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING
 *
 * The amount of padding to place at the back of each allocation. We
 * have several ways to then check that no one overwrites this padding
 * though we won't catch every case.
 *
 * Unlike start padding, this does not effect alignment and so values
 * as small as 1 make perfect sense though I still recommend 4 or 8
 * bytes especially on 64bit big-endian architectures.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING
#define ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING 8
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE
 *
 * This determine how big the lossy hashtable is. On every allocation
 * or deallocation the *entire* lossy hashtable is scanned to see if
 * the padding bytes have been perturbed which makes it possible to
 * find some memory overwrite errors earlier than waiting for the free
 * call (or potentially even if the memory isn't ever freed).
 *
 * It makes no sense to set this unless either
 * ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING or
 * ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING is non-zero.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE
#define ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE 16
#endif

/**
 * @debug_compiliation_option ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT
 *
 * This is just a fail-safe to catch extremely dumb allocation amounts
 * (at least as of 2023). If you know you will have a fixed size
 * amount of memory, you could set this lower and potentially get a
 * slightly nicer error message.
 */
#ifndef ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT
#define ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT (1L << 48)
#endif

#define START_PADDING_BYTE (170 & 0xff)
#define END_PADDING_BYTE ((~START_PADDING_BYTE) & 0xff)

struct memory_hashtable_bucket_S {
  // malloc will never allocated at address 0 so if this field is
  // zero, then this spot in the hashtable is occupied.
  uint64_t malloc_address;
  uint64_t malloc_size;
  char* allocation_filename;
  uint64_t allocation_line_number;
};

typedef struct memory_hashtable_bucket_S memory_hashtable_bucket_t;

memory_hashtable_bucket_t memory_ht[ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE];

void check_start_padding(uint8_t* address) {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING; i++) {
    if (address[i] != START_PADDING_BYTE) {
      fatal_error(ERROR_MEMORY_START_PADDING_ERROR);
    }
  }
}

void check_end_padding(uint8_t* address, char* filename, uint64_t line) {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING; i++) {
    if (address[i] != END_PADDING_BYTE) {
      fprintf(stderr,
              "FATAL: someone clobbered past an allocation %lu. (allocated "
              "here: %s:%lu)\n",
              ((uint64_t) address), filename, line);
      fatal_error(ERROR_MEMORY_END_PADDING_ERROR);
    }
  }
}

void check_memory_hashtable_padding() {
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE; i++) {
    if (memory_ht[i].malloc_address != 0) {
      uint64_t malloc_start_address = memory_ht[i].malloc_address;
      uint64_t malloc_size = memory_ht[i].malloc_size;
      check_start_padding((uint8_t*) malloc_start_address);
      check_end_padding((uint8_t*) (malloc_start_address
                                    + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
                                    + malloc_size),
                        memory_ht[i].allocation_filename,
                        memory_ht[i].allocation_line_number);
    }
  }
}

// I got this from a blog post by Daniel Lemire (who was actually
// pushing a different scheme...) A terrible hash function will sink
// our scheme but anything that isn't terrible just gets us closer to
// some ideal.
uint64_t mumurhash64_mix(uint64_t h) {
  h *= h >> 33;
  h *= 0xff51afd7ed558ccdL;
  h *= h >> 33;
  h *= 0xc4ceb9fe1a85ec53L;
  h *= h >> 33;
  return h;
}

// Start tracking padding for a given allocated address. This includes
// setting the padding to particular values and of course putting the
// address into the tracking table.
void track_padding(char* file, int line, uint8_t* address, uint64_t amount) {
  // First set the padding to predicatable values
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING; i++) {
    address[i] = START_PADDING_BYTE;
  }
  uint8_t* end_padding_address
      = address + amount + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;
  for (int i = 0; i < ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING; i++) {
    end_padding_address[i] = END_PADDING_BYTE;
  }

  if (ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE > 0) {
    // Now replace whatever entry we might already have there. This is
    // why we have more LRU semantics. We could use another signal to
    // probalistically delay updating the hashtable when the bucket is
    // already occupied but I think LRU might work well most of the
    // time. (Mostly a hunch I will admit.).
    int bucket = mumurhash64_mix((uint64_t) address)
                 % ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE;
    memory_ht[bucket].malloc_address = (uint64_t) address;
    memory_ht[bucket].malloc_size = amount;
    memory_ht[bucket].allocation_filename = file;
    memory_ht[bucket].allocation_line_number = line;
  }
}

void untrack_padding(uint8_t* malloc_address) {
  check_start_padding(malloc_address);
  // Unfortunately, since we don't know the size of the allocation, we
  // can't actually check the end padding! When there is enough start
  // padding (say at least 64bits), then we could potentially store
  // say 48bits worth of an allocation amount in it.
  //
  // On the other hand, we do check the end padding if it is still
  // tracked in the lossy memory hashtable.

  if (ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE > 0) {
    // Now finally zero-out the memory hashtable.
    int bucket = mumurhash64_mix((uint64_t) malloc_address)
                 % ARMYKNIFE_MEMORY_ALLOCATION_HASHTABLE_SIZE;
    memory_ht[bucket].malloc_address = 0;
    memory_ht[bucket].malloc_size = 0;
    memory_ht[bucket].allocation_filename = 0;
    memory_ht[bucket].allocation_line_number = 0;
  }
}

/**
 * @function arena_checked_malloc
 *
 * Allocate the given amount bytes or cause a fatal error. The memory
 * is also zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
uint8_t* arena_checked_malloc(char* file, int line, uint64_t amount) {

  if (amount == 0 || amount > ARMYKNIFE_MEMORY_ALLOCATION_MAXIMUM_AMOUNT) {
    fatal_error(ERROR_BAD_ALLOCATION_SIZE);
  }

  if (should_log_memory_allocation()) {
    fprintf(stderr,
            "ALLOCATE %s:%d -- n_bytes=%lu already_allocated=%lu n_calls=%lu\n",
            file, line, amount, number_of_bytes_allocated,
            number_of_malloc_calls);
  }

  check_memory_hashtable_padding();

  uint64_t amount_with_padding = ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING
                                 + amount
                                 + ARMYKNIFE_MEMORY_ALLOCATION_END_PADDING;
  uint8_t* result = malloc(amount_with_padding);
  if (result == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_ALLOCATION);
  }

  if (should_log_memory_allocation()) {
    fprintf(stderr, "ALLOCATE %s:%d -- %lu -- ptr=%lu\n", file, line, amount,
            (unsigned long) result);
  }

  memset(result, 0, amount_with_padding);
  track_padding(file, line, result, amount);

  number_of_bytes_allocated += amount;
  number_of_malloc_calls++;

  return result + ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;
}

/**
 * @function checked_malloc_copy_of
 *
 * Allocate amount bytes and initialize it with a copy of that many
 * bytes from source.
 */
uint8_t* arena_checked_malloc_copy_of(char* file, int line, uint8_t* source,
				      uint64_t amount) {
  uint8_t* result = checked_malloc(file, line, amount);
  memcpy(result, source, amount);
  return result;
}

/**
 * @function checked_free
 *
 * Allocate amount bytes or cause a fatal error. The memory is also
 * zeroed.
 *
 * If possible, use the macros malloc_bytes or malloc_struct instead
 * for an easier to use interface. Those macros simply call
 * checked_malloc.
 */
void arena_checked_free(char* file, int line, void* pointer) {
  if (should_log_memory_allocation()) {
    fprintf(stderr, "DEALLOCATE %s:%d -- %lu\n", file, line,
            (uint64_t) pointer);
  }
  if (pointer == NULL) {
    fatal_error_impl(file, line, ERROR_MEMORY_FREE_NULL);
  }

  // Check all of the padding we know about
  check_memory_hashtable_padding();

  uint8_t* malloc_pointer
      = ((uint8_t*) pointer) - ARMYKNIFE_MEMORY_ALLOCATION_START_PADDING;

  // Check this entries padding (in case it got lossed from the global
  // hashtable), and also remove it from the hashtable if it was
  // found.
  untrack_padding(malloc_pointer);
  number_of_free_calls++;
  free(malloc_pointer);
}

#endif /* C_ARMYKNIFE_LIB_USE_ARENAS */
