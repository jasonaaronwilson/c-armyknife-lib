#line 2 "random.c"

/**
 * An implementation of "xorshiro128**", a pseudo-random number
 * generator.
 *
 * This is not a high quality source of entropy and is intended for
 * use in tests or other places where determinism is important
 * (including across platforms and C library implementations).
 *
 * See: https://prng.di.unimi.it/xoroshiro128starstar.c
 */

#ifndef _RANDOM_H_
#define _RANDOM_H_

struct random_state_S {
  uint64_t a;
  uint64_t b;
};

typedef struct random_state_S random_state_t;

extern random_state_t random_state_for_test(void);
extern uint64_t random_next(random_state_t* state);

#endif /* _RANDOM_H_ */

/**
 * Return a consistent initial random state for tests.
 */
random_state_t random_state_for_test(void) {
  return (random_state_t){.a = 0x1E1D43C2CA44B1F5, .b = 0x4FDD267452CEDBAC};
}

static inline uint64_t rotl(uint64_t x, int k) {
  return (x << k) | (x >> (64 - k));
}

uint64_t random_next(random_state_t* state) {
  uint64_t s0 = state->a;
  uint64_t s1 = state->b;
  uint64_t result = rotl(s0 * 5, 7) * 9;
  s1 ^= s0;
  state->a = rotl(s0, 24) ^ s1 ^ (s1 << 16); // a, b
  state->b = rotl(s1, 37);                   // c

  return result;
}
