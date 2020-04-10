#ifndef text_h
#define text_h

#ifdef AMIGA
#define DEF_STR_LEN 8192
#else
#define DEF_STR_LEN 32768
#endif

#include "multibyte.h"

extern int LEFT;
extern int CENTER;
extern int RIGHT;

extern CHAR ch;

extern int paragraph;
extern int div_test;
extern int nooutput;

extern int breite;
extern int hr_breite;

void status();

extern int tab;
extern int spaces;

void print_zeile();
int is_zeile_empty();
void clear_line();

void push_align(int a);
void pop_align();

void wort_plus_string(CHAR *s);
void wort_plus_string_escape(CHAR *s, int do_escape);
void wort_plus_string_nocount(CHAR *s);
void wort_plus_ch(int c);
void wort_ende();

void line_break();

void paragraphen_ende();
void neuer_paragraph();

void hr();

#endif
