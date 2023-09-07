#include <stdlib.h>

#include "paged-memory.h"

/**
 * This is an abstraction for holding the virtualized main memory
 * (i.e., a "proccess address space" like a POSIX process might wish
 * to employ).
 *
 * The interface must support:
 * 1) at least somewhat sparse address spaces
 * 2) little-endian layout (even when running on big-endian hardware)
 * 3) non-aligned accesses
 *
 * In the future, the interface might support more features such as:
 * 1) deallocation of a page
 * 2) remapping (or duplicately mapping) a page to a new address
 * 3) read-only pages
 * 4) O(1) snap-shots (useful for time-traveling debugging)
 * 5) simulated memory mapping of a file for read-only access
 *
 * For simplicity, this implementation just uses a linked-list of page
 * mappings (ordered by the most recently allocated). This is
 * obviously quite slow compared to a hash-table, a HAMT, etc., though
 * for small programs with one or two pages which is probably all I
 * will have to test with for quite a while, it will be quite fast
 * anyways and only beat by say using memory mapping in the host (plus
 * say a single addition to determine the host address).
 *
 * This version is also slower than necessary in that it doesn't
 * attempt to use aligned loads even when the address is naturally
 * aligned and the current machine is little-endian.
 */

uint8_t *page_data_of(paged_memory *memory, uint64_t page_number) {
  while (memory) {
    if (memory->page_number == page_number) {
      return memory->data;
    }
  }
  return 0;
}

/**
 * Determine if a mapping exists for a given address.
 */
int is_address_mapped(paged_memory *memory, uint64_t address) {
  return page_data_of(memory, address >> PAGE_SHIFT) != 0;
}

/**
 * Ensure that a mapping exists for the given address. Note that a new
 * paged_memory* object may be returned (even in cases where it is
 * already mapped).
 */
paged_memory *allocate_page(paged_memory *memory, uint64_t address) {
  if (!is_address_mapped(memory, address)) {
    paged_memory *result = (paged_memory *)(malloc(sizeof(paged_memory)));
    // TODO(jawilson): check result isn't null
    result->next = memory;
    result->page_number = address >> PAGE_SHIFT;
    result->data = (uint8_t *)(malloc(PAGE_SIZE));
    // TODO(jawilson): check data isn't null and also memset to 0
    return result;
  }
  return memory;
}

uint8_t load8(paged_memory *memory, uint64_t address) { return 0; }

void store8(paged_memory *memory, uint64_t address, uint8_t value) {}

uint16_t load16(paged_memory *memory, uint64_t address) { return 0; }

void store16(paged_memory *memory, uint64_t address, uint16_t value) {}

uint32_t load32(paged_memory *memory, uint64_t address) { return 0; }

void store32(paged_memory *memory, uint64_t address, uint32_t value) {}

uint64_t load64(paged_memory *memory, uint64_t address) { return 0; }

void store64(paged_memory *memory, uint64_t address, uint64_t value) {}
