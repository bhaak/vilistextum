#ifndef util_h
#define util_h 1

#include "multibyte.h"

#define wcstoi(tmp)  wcstol(tmp, (wchar_t **)NULL, 10)

int get_align();
void push_align(int a);
void pop_align();

int uppercase(int c);
void uppercase_str(CHAR *s);

void set_char(CHAR *s, CHAR c);

int x2dec(CHAR *s, int base);

void print_error(char *error, CHAR *text);

#endif
