#ifndef unicode_entities_h
#define unicode_entities_h       1

#include "multibyte.h"

int unicode_entity(CHAR *s);
int ligature_entity(CHAR *s);
int fallback_entity(CHAR *s);
CHAR* fallback_character(CHAR s);

#endif
