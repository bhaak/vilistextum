#ifndef main_h
#define main_h

#include "multibyte.h"

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

int palm;
int	convert_tags;
int errorlevel;
int convert_characters;
int shrink_lines;
int remove_empty_alt;
int option_links;
int option_links_inline;
int option_title;
int sevenbit;
int transliteration;

int option_no_image;
int option_no_alt;
int option_output_utf8;

int option_latex;

CHAR *default_image;

#endif
