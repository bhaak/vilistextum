#ifndef fileio_h
#define fileio_h 1

#include "multibyte.h"

void quit();

void open_files(char *input, char *output);
void output_string(CHAR *str);
void convert_string(char *str, CHAR *converted_string);

int get_current_char();
int read_char();
/*void goback_char(int p); */
void putback_char(CHAR c);


void files_reopen(); 

#endif
