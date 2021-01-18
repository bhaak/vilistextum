#ifndef html_h
#define html_h 1

#include "text.h"
#include "multibyte.h"

extern int pre;

int get_attr(void);
int get_new_attr(CHAR *name, CHAR *content);

extern CHAR attr_name[DEF_STR_LEN];
extern CHAR attr_ctnt[DEF_STR_LEN];

void html(void);
void check_for_center(void);
void start_p(void);
void start_div(int a);
void end_div(void);
void image(CHAR *, int);
CHAR friss_kommentar(void);

void find_encoding(void);
void find_xml_encoding(void);

void href(void);
void href_output(void);
void href_link_inline_output(void);

void start_nooutput(void);
void end_nooutput(void);
#endif
