#ifndef main_h
#define main_h

#include "multibyte.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

extern int palm;
extern int convert_tags;
extern int errorlevel;
extern int convert_characters;
extern int shrink_lines;
extern int remove_empty_alt;
extern int option_links;
extern int option_links_inline;
extern int option_title;
extern int sevenbit;
extern int transliteration;

extern int option_no_image;
extern int option_no_alt;
extern int option_output_utf8;

extern int option_latex;

extern CHAR *default_image;

#endif
