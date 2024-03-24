#line 2 "terminal.c"
#ifndef _TERMINAL_H_
#define _TERMINAL_H_

/**
 * @file terminal.c
 *
 * A simplistic terminal library that doesn't depend on ncurses,
 * etc. Realistically most terminals are now decendants of xterm
 * (itelf like vt100 or ANSI deep down) and tend to converge on how
 * new features work so we are going to just initialy code for Konsole
 * and go from there.
 *
 * Based on this document, I feel we can assume we have 24bit color
 * support and handle other terminals differently in the future
 * (perhaps by just not emitting colors):
 *
 * https://github.com/termstandard/colors
 */

__attribute__((warn_unused_result)) extern buffer_t*
    term_clear_screen(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    term_set_foreground_color(buffer_t* buffer, uint32_t color);

__attribute__((warn_unused_result)) extern buffer_t*
    term_set_background_color(buffer_t* buffer, uint32_t color);

__attribute__((warn_unused_result)) extern buffer_t*
    term_move_cursor_absolute(buffer_t* buffer, int x, int y);

__attribute__((warn_unused_result)) extern buffer_t*
    term_move_cursor_relative(buffer_t* buffer, int x, int y);

__attribute__((warn_unused_result)) extern buffer_t*
    term_bold(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t* term_dim(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    term_italic(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    term_underline(buffer_t* buffer);

__attribute__((warn_unused_result)) extern buffer_t*
    term_reset_formatting(buffer_t* buffer);

// TODO(jawilson): terminal queries like request cursor position

#endif /* _TERMINAL_H_ */

#define TERM_ESCAPE_START "\033["
#define TERM_ESCAPE_END "m"

/**
 * @function term_set_foreground_color
 *
 * Append a terminal escape sequence to a buffer that instructs the
 * terminal to change to foreground color to a 24bit RGB value.
 *
 * (Currently we don't automatically "downgrade" to 256 color or 16
 * color terminals).
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_set_foreground_color(buffer_t* buffer, uint32_t color) {
  uint8_t blue = color & 0xff;
  uint8_t green = (color >> 8) & 0xff;
  uint8_t red = (color >> 16) & 0xff;

  // Escape sequence for setting foreground color (ESC [ 38; 2; r; g; b m)
  return buffer_printf(buffer,
                       TERM_ESCAPE_START "38;2;%d;%d;%d" TERM_ESCAPE_END, red,
                       green, blue);
}

/**
 * @function term_set_background_color
 *
 * Append a terminal escape sequence to a buffer that instructs the
 * terminal to change to background color to a 24bit RGB value.
 *
 * (Currently we don't automatically "downgrade" to 256 color or 16
 * color terminals).
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_set_background_color(buffer_t* buffer, uint32_t color) {
  uint8_t blue = color & 0xff;
  uint8_t green = (color >> 8) & 0xff;
  uint8_t red = (color >> 16) & 0xff;

  // Escape sequence for setting background color (ESC [ 48; 2; r; g; b m)
  return buffer_printf(buffer,
                       TERM_ESCAPE_START "48;2;%d;%d;%d" TERM_ESCAPE_END, red,
                       green, blue);
}

/**
 * @function term_move_cursor_absolute
 *
 * Append a terminal escape sequence to a buffer that instructs the
 * terminal to move it's "cursor" to a zero based x, y absolute
 * coordinate.
 *
 * (Traditionally terminals operate on a 1 based coordinate system but
 * zero based indexes would perhaps be choosen now since most
 * programming languages are zero based).
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_move_cursor_absolute(buffer_t* buffer, int x, int y) {
  // Escape sequence for cursor movement (ESC [ y; x H)
  return buffer_printf(buffer, TERM_ESCAPE_START "%d;%dH", y + 1, x + 1);
}

/**
 * @function term_move_cursor_relative
 *
 * Append a terminal escape sequence to a buffer that instructs the
 * terminal to move it's "cursor" relative to it's current poition. 0
 * doesn't change the column or row, while negative will move either
 * left or to the "top" of the terminal, while positive numbers move
 * right or downwards towards the bottom of the terminal.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_move_cursor_relative(buffer_t* buffer, int x, int y) {
  // First handle the x position
  if (x > 0) {
    buffer = buffer_printf(buffer, TERM_ESCAPE_START "%dC", x);
  } else if (x < 0) {
    buffer = buffer_printf(buffer, TERM_ESCAPE_START "%dD", -x);
  }
  if (y > 0) {
    buffer = buffer_printf(buffer, TERM_ESCAPE_START "%dB", y);
  } else {
    buffer = buffer_printf(buffer, TERM_ESCAPE_START "%dA", -y);
  }
  return buffer;
}

/**
 * @function term_bold
 *
 * Append a terminal escape sequence to a buffer that turns on "bold"
 * text.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_bold(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "1m");
}

/**
 * @function term_dim
 *
 * Append a terminal escape sequence to a buffer that turns on "dim"
 * text.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_dim(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "2m");
}

/**
 * @function term_dim
 *
 * Append a terminal escape sequence to a buffer that turns on "italic"
 * text.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_italic(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "3m");
}

/**
 * @function term_underline
 *
 * Append a terminal escape sequence to a buffer that turns on
 * "underline" text.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_underline(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "4m");
}

/**
 * @function term_reset_formatting
 *
 * Append a terminal escape sequence to a buffer that resets the
 * formatting (and appears to cancel the foreground and background
 * color as well).
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_reset_formatting(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "0m");
}

/**
 * @function term_clear_screen
 *
 * Append a terminal escape sequence to a buffer that clears the
 * entire terminal.
 */
__attribute__((warn_unused_result)) extern buffer_t*
    term_clear_screen(buffer_t* buffer) {
  return buffer_printf(buffer, TERM_ESCAPE_START "2J");
}
