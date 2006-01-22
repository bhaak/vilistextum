/*
 * Copyright (c) 1998-2006 Patric Müller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 * 
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <string.h>
#include <stdio.h>

#include "debug.h"

#include "main.h"
#include "util.h"
#include "multibyte.h"

/* ------------------------------------------------ */

int unicode_entity(CHAR *s)
{
	int number;
  if (!convert_characters) { return(0); }

	number = extract_entity_number(s);

	switch (number) {
	case 8208: /* 0x2010 HYPHEN */
	case 8209: /* 0x2011 NON-BREAKING HYPHEN */
	 set_char(s, '-'); break;
  default: 
		return(0); 
	}

  return(1); /* found a transcription for entity */
} /* end unicode_entity */

/* ------------------------------------------------ */
