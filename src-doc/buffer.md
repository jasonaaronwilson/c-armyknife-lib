# @file buffer.c

A growable array of bytes.

Buffers are useful for constructing UTF-8 strings (for example by
using buffer_printf) though they can hold any binary data including
interior NUL bytes. Buffers automatically grow as data is appended
(or inserted into) them reducing large classes of errors.
 
## @struct buffer_t

Buffers are non-thread-safe memory regions that generally "grow"
over time. Whenever a buffer grows beyound it's capacity, it is
moved to accomodates its new capacity (even if there may be other
pointers to it's insides, so don't do that unless you have decided
the buffer will never grow again).

While buffers may seem scary, used properly from a single thread,
they are actually quite predictable.
 
## @function buffer_append_byte

Append a single byte to the byte array.
 
## @function buffer_append_bytes

Append multiple bytes to the byte array.
 
## @function buffer_append_code_point()

Append a single code-point according to UTF-8 encoding (so 1 to 4
bytes). While you can put anything you want into a buffer_t (not
just valid UTF-8 sequences), if you then try to make a C string
from the buffer then it might end up with a NUL ('\0') byte in the
middle of it if you add code_point == 0 somewhere besides the end
of the string.

@see utf8_decode(const uint8_t* utf8_bytes).
 
## @function buffer_append_repeated_byte

Append 'count' copies of byte to the passed in buffer. This can be
used for things like indentation or horizontal rules (composed from
say '-', '=', or '*').
 
## @function buffer_append_string

Append all of the bytes from a C string (except the ending NUL
char).
 
## @function buffer_c_substring

Extract a newly allocated string that contains the bytes from start
to end (appending a zero byte to make sure it's a legal C string).
 
## @function buffer_get

Get a single byte from a byte array.
 
## @function buffer_increase_capacity

As an optimization, the capacity of a buffer can be increased.
 
## @function buffer_length

Clear a buffer without resizing it.
 
## @function buffer_length

Return the number of bytes that have been added to this byte array.
 
## @function buffer_printf

Format like printf but append the result to the passed in buffer
(returning a new buffer in case the buffer exceeded it's capacity).
 
## @function buffer_to_c_string

Extract a newly allocated string that contain all of the bytes in the byte
buffer as a NUL (zero byte) terminated C string.
 
## @function make_buffer

Make an empty byte array with the given initial capacity.
 
