# @file value-array.c

This file contains a growable array of values/pointers.
 
## @function make_value_array

Make a value array with the given initial capacity (which must be >
0). When the array runs out of capacity because of calls to add,
push, etc., then the backing array is automatically doubled in size
(this may change to a different fraction for "large arrays"
(greater than say 250 elements) in the future to save space).

If the initial_capacity is zero or if malloc() can't allocate
everything, then a fatal_error() occurs.
 
## @function value_array_add

Add an element to the end of an array. If more space is required
then the backing array is automatically resized. This resizing
means that a fatal_error() may occur if malloc() can not satisfy the
allocation request.
 
## @function value_array_delete_at

Deletes the element at the given position (and return it so that it
can potentially be freed by the caller).

If the position doesn't point to a valid element then
fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 
## @function value_array_get

Get the value stored at index `index`. If the index is outside of
the range of valid elements, then a fatal_error is signaled.
 
## @function value_array_insert_at

Insert an element into some existing position in the array (or at
the end if position == the current array length).

This operation is not efficient for large arrays as potentially the
entire array must be moved to new locations (there are
data-structures like trees that can make this more efficient though
such a data-structure isn't in this library yet).

If the position is > than the length length, then
fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.

If memory allocation is required and malloc() fails, then
fatal_error(ERROR_MEMORY_ALLOCATION) is called.
 
## @function value_array_pop

Returns the last element of the array (typically added via push).

If the array is currently empty, then
fatal_error(ERROR_ACCESS_OUT_OF_BOUNDS) is called.
 
## @function value_array_push

This is a synonym for value_array_add which serves to make it more
obvious that the array is actually being used like a stack.
 
