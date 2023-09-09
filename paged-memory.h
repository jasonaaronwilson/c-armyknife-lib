#ifndef _PAGED_MEMORY_
#define _PAGED_MEMORY_

#include <stdint.h>

// 64K pages are neither large nor small.
#define PAGE_SHIFT 16
#define PAGE_SIZE (1 << PAGE_SHIFT)
#define PAGE_MASK (((uint64_t) PAGE_SIZE) - 1)

// Dead simple but slow implementation sufficient for small
// programs. Only paged_memory.c may access the members of this
// strucuture.
typedef struct paged_memory_S {
  struct paged_memory_S* next;
  uint64_t page_number;
  uint8_t* data;
} paged_memory_t;

extern int is_address_mapped(paged_memory_t* memory, uint64_t address);
extern paged_memory_t* allocate_page(paged_memory_t* memory, uint64_t address);
extern uint8_t load8(paged_memory_t* memory, uint64_t address);
extern void store8(paged_memory_t* memory, uint64_t address, uint8_t value);
extern uint16_t load16(paged_memory_t* memory, uint64_t address);
extern void store16(paged_memory_t* memory, uint64_t address, uint16_t value);
extern uint32_t load32(paged_memory_t* memory, uint64_t address);
extern void store32(paged_memory_t* memory, uint64_t address, uint32_t value);
extern uint64_t load64(paged_memory_t* memory, uint64_t address);
extern void store64(paged_memory_t* memory, uint64_t address, uint64_t value);

#endif
