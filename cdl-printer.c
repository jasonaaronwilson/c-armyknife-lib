#line 2 "cdl-printer.c"

/**
 * @file cdl-printer.c
 *
 * CDL is like TOML where the "inline" format is used for all tables,
 * commas are eliminated, and there is no support for dates.
 *
 * CDL can represent:
 *
 * 1. comments ==> # extends to the end of the line
 * 1. booleans ===> true and false
 * 1. symbols ==> foo, bar element_name, etc. (covers C identifiers)
 * 1. strings ==> "example\n"
 * 1. numbers ==> 123.5, 120, etc.
 * 1. arrays ==> [ hello world 123 54.9]
 * 1. tables ==> { x = 100 y = 50 }
 *
 * keys in the table must be symbols or strings (possibly numbers in
 * the future?)
 *
 * This library only provides a printer. For reading, we'll delay that
 * until Omni C can use it's reflection API to automatically write
 * readers for us.
 *
 * CDL stands for "clear data language" or "C data language"
 */

#ifndef _CDL_PRINTER_H_
#define _CDL_PRINTER_H_

typedef struct {
  buffer_t* buffer;
  char* key_token;
  int indention_level;
} cdl_printer_t;

cdl_printer_t* make_cdl_printer(buffer_t* buffer);

void cdl_boolean(cdl_printer_t* printer, boolean_t bolean);
void cdl_string(cdl_printer_t* printer, char* string);
void cdl_int64(cdl_printer_t* printer, int64_t number);
void cdl_uint64(cdl_printer_t* printer, uint64_t number);
void cdl_double(cdl_printer_t* printer, double number);

void cdl_start_array(cdl_printer_t* printer);
void cdl_end_array(cdl_printer_t* printer);

void cdl_start_table(cdl_printer_t* printer);
void cdl_key(cdl_printer_t* printer, char* key);
void cdl_end_table(cdl_printer_t* printer);

#endif /* _CDL_PRINTER_H_ */

cdl_printer_t* make_cdl_printer(buffer_t* buffer) {
  cdl_printer_t* result = malloc_struct(cdl_printer_t);
  result->buffer = buffer;
  return result;
}

void cdl_indent(cdl_printer_t* printer) {
  buffer_append_repeated_byte(printer->buffer, ' ',
                              4 * printer->indention_level);
}

boolean_t is_symbol(char* string) {
  for (int i = 0; string[i] != 0; i++) {
    if (i == 0) {
      if (!isalpha(string[i])) {
        return false;
      }
    } else {
      if (!isalnum(string[i])) {
        return false;
      }
    }
  }
  return true;
}

void cdl_output_token(cdl_printer_t* printer, char* string) {
  cdl_indent(printer);
  if (printer->key_token != NULL) {
    buffer_printf(printer->buffer, "%s = %s\n", printer->key_token, string);
    printer->key_token = NULL;
  } else {
    buffer_printf(printer->buffer, "%s\n", string);
  }
}

void cdl_boolean(cdl_printer_t* printer, boolean_t boolean) {
  cdl_output_token(printer, boolean ? "true" : "false");
}

void cdl_string(cdl_printer_t* printer, char* string) {
  if (!is_symbol(string)) {
    cdl_output_token(printer, string_printf("\"%s\"", string));
  } else {
    cdl_output_token(printer, string);
  }
}

void cdl_int64(cdl_printer_t* printer, int64_t number) {
  cdl_output_token(printer, string_printf("%ld", number));
}

void cdl_uint64(cdl_printer_t* printer, uint64_t number) {
  cdl_output_token(printer, uint64_to_string(number));
}

void cdl_double(cdl_printer_t* printer, double number) {
  cdl_output_token(printer, string_printf("%lf", number));
}

void cdl_start_array(cdl_printer_t* printer) {
  cdl_output_token(printer, "[");
  printer->indention_level += 1;
}

void cdl_end_array(cdl_printer_t* printer) {
  printer->indention_level -= 1;
  cdl_output_token(printer, "]");
}

void cdl_start_table(cdl_printer_t* printer) {
  cdl_output_token(printer, "{");
  printer->indention_level += 1;
}

void cdl_key(cdl_printer_t* printer, char* key) { printer->key_token = key; }

void cdl_end_table(cdl_printer_t* printer) {
  printer->indention_level -= 1;
  cdl_output_token(printer, "}");
}
