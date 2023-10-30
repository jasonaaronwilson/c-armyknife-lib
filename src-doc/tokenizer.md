# @file tokenizer.c

This contains a routine to split a string into a series of tokens
seperated via delimiters.
 
## @function tokenize

Tokenize a string.

Delimiters terminate the current token and are thrown away. The
delimiters string is treated as a sequence of delimiter characters,
it does not mean a delimiter can be multiple characters.
 
