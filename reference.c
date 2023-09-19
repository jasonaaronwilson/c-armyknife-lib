/**
 * C does not have parameterized types which makes generic containers
 * more difficult to work with.
 *
 * Therefore we we resort to dynamic (aka runtime) type checking of
 * the generic parameters.
 * type_t conveniently are interned so we can check a type with a
 * single comparison of a type_t pointer. Additionally, we pass these
 * structs by value which the C compiler can probably return in
 * registers.
 *
 * Containers which store elements of the same type only need to store
 * the type once so this doesn't impact the overall space occupied by 
 * the collection.
 */

struct {
  type_t* underlying_type;
  void* pointer;
} reference_t;
