# @file terminal.c

A simplistic terminal library that doesn't depend on ncurses,
etc. Realistically most terminals are now decendants of xterm
(itelf like vt100 or ANSI deep down) and tend to converge on how
new features work so we are going to just initialy code for Konsole
and go from there.

Based on this document, I feel we can assume we have 24bit color
support and handle other terminals differently in the future
(perhaps by just not emitting colors):

https://github.com/termstandard/colors
 
## @function term_clear_screen

Append a terminal escape sequence to a buffer that clears the
entire terminal.
 
## @function term_move_cursor_absolute

Append a terminal escape sequence to a buffer that instructs the
terminal to move it's "cursor" to a zero based x, y absolute
coordinate.

(Traditionally terminals operate on a 1 based coordinate system but
zero based indexes would perhaps be choosen now since most
programming languages are zero based).
 
## @function term_set_background_color

Append a terminal escape sequence to a buffer that instructs the
terminal to change to background color to a 24bit RGB value.

(Currently we don't automatically "downgrade" to 256 color or 16
color terminals).
 
## @function term_set_foreground_color

Append a terminal escape sequence to a buffer that instructs the
terminal to change to foreground color to a 24bit RGB value.

(Currently we don't automatically "downgrade" to 256 color or 16
color terminals).
 
