#ifndef html_h
#define html_h 1

#include "text.h"
#include "multibyte.h"

extern int pre;

int get_attr();
int get_new_attr(CHAR *name, CHAR *content);

extern CHAR attr_name[DEF_STR_LEN];
extern CHAR attr_ctnt[DEF_STR_LEN];

void html();
void check_for_center();
void start_p();
void start_div(int a);
void end_div();
void image(CHAR *, int);
CHAR friss_kommentar();

void find_encoding();
void find_xml_encoding();

void href();
void href_output();
void href_link_inline_output();

void start_nooutput();
void end_nooutput();
#endif
