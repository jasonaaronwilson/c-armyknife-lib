# @file buffer.c

A growable array of bytes. These are useful for many purposes such
as building large strings.
 
## @function buffer_append_byte

Append a single byte to the byte array.
 
## @function buffer_append_bytes

Append multiple bytes to the byte array.
 
## @function buffer_append_string

Append all of the bytes from a C string (except the ending NUL
char).
 
## @function buffer_c_substring

Extract a newly allocated string that contain the bytes from start
to end (appending a zero byte to make sure it's a legal C string).
 
## @function buffer_get

Get a single byte from a byte array.
 
## @function buffer_increase_capacity

As an optimization, the capacity of a buffer can be increased.
 
## @function buffer_length

Return the number of bytes that have been added to this byte array.
 
## @function buffer_to_c_string

Extract a newly allocated string that contain all of the bytes in the byte
buffer as a NUL (zero byte) terminated C string.
 
## @function make_buffer

Make an empty byte array with the given initial capacity.
 
