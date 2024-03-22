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
 
