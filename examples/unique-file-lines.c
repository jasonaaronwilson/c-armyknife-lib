/**
 * @file unique-file-lines.c
 *
 * This program reads one or more files specified on the command line
 * and "cats" them together but with duplicate lines removed (in some
 * ways very similar to the histogram program we will eventually
 * provide).
 *
 * This is similar to using cat + sort + uniq on Unix except:
 *
 * 1. The input remains in the orginal order

 * 2. While I'm not sure how much memory *sort* uses on your flavor of
 * unix, uniq only has to keep two lines in memory at once, since it
 * only compares adjacent lines, so it's unfair to only compare us to
 * "uniq" without also comparing us to "sort". For this program,
 * "unique-file-lines", you can assume we use at least as much memory
 * as would be needed to keep all of the input files in memory.
 *
 * Besides potentially being useful, this tool stresses a bunch of
 * parts of c-arymknife-lib and since this program can easily be
 * written in other languages, it provides some potential for
 * benchmarking.
 *
 * While not currently implemented, probabilistic approaches could be
 * much more space efficient (and potentially more performant). For
 * example, with a decent quality 64bit hash, every "seen" line could
 * be represented in about 8 bytes and could use a better "set"
 * abstraction than what our library provides (since we assume you can
 * use a hashmap mapped to true/false which wastes space).
 *
 * The problem is that with some small probability, a non duplicate
 * line will be determined to be a duplicate. An even greater space
 * savings may be possible with Bloom filters, which will also
 * incorrectly classify some non duplicate lines as duplicates.
 */

string_hashtable_t* initial_hashtable() {
  // Currently hashtables don't grow so at least make sure we reduce
  // "scanning" by 1000% or more with a large initial hashtable that
  // doesn't even seem that large TBH.
  string_hashtable_t seen = make_string_hashtable(64 * 1024);
  return seen;
}
