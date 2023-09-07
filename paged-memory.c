#include "paged-memory.h"

paged_memory* allocate_page(paged_memory *memory, uint64_t address) {
  return 0;
}

// Must support unaligned accesses and present memory as
// little-endian.

uint8_t load8(paged_memory *memory, uint64_t address) {
  return 0;
}

void store8(paged_memory *memory, uint64_t address, uint8_t value) {
}

uint16_t load16(paged_memory *memory, uint64_t address) {
  return 0;
}

void store16(paged_memory *memory, uint64_t address, uint16_t value) {
}

uint32_t load32(paged_memory *memory, uint64_t address) {
  return 0;
}

void store32(paged_memory *memory, uint64_t address, uint32_t value) {
}

uint64_t load64(paged_memory *memory, uint64_t address) {
  return 0;
}

void store64(paged_memory *memory, uint64_t address, uint64_t value) {
}
