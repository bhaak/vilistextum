/*
 * Copyright (C) 1998-2004 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 * 
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
 *
 */

#include <string.h>
#include <stdio.h>

#include "debug.h"

#include "text.h"
#include "main.h"
#include "util.h"
#include "multibyte.h"

/* ------------------------------------------------ */

int microsoft_entities(CHAR *s)
{
  if (!convert_characters) { return(0); }
  /* Euro */
  else if CMP("&#128;", s)    { CPYSL(s, "EUR"); }
  else if CMP("&euro;", s)    { CPYSL(s, "EUR"); }
  else if CMP("&#8364;", s)   { CPYSL(s, "EUR"); }
  
  /* Single Low-9 Quotation Mark */
  else if CMP("&#130;", s)    { set_char(s, ','); }
  else if CMP("&sbquo;", s)   { set_char(s, ','); }
  else if CMP("&#8218;", s)   { set_char(s, ','); }

  else if CMP("&#131;", s)    { set_char(s, 'f'); } /* Latin Small Letter F With Hook */
  else if CMP("&fnof;", s)    { set_char(s, 'f'); } /* Latin Small Letter F With Hook */
  else if CMP("&#402;", s)    { set_char(s, 'f'); } /* Latin Small Letter F With Hook */

  /* Double Low-9 Quotation Mark */
  else if CMP("&#132;", s)    { CPYSL(s, "\""); }
  else if CMP("&bdquo;", s)   { CPYSL(s, "\""); }
  else if CMP("&#8222;", s)   { CPYSL(s, "\""); }

  else if CMP("&#133;", s)    { CPYSL(s, "... "); } /* Horizontal Ellipsis */
  else if CMP("&hellip;", s)  { CPYSL(s, "... "); } /* Horizontal Ellipsis */
  else if CMP("&#8230;", s)   { CPYSL(s, "... "); } /* Horizontal Ellipsis */

  /* Dagger */
  else if CMP("&#134;", s)    { CPYSL(s, "/-"); }   
  else if CMP("&dagger;", s)  { CPYSL(s, "/-"); }
  else if CMP("&#8224;", s)   { CPYSL(s, "/-"); }

  /* Double Dagger */
  else if CMP("&#135;", s)    { CPYSL(s, "/="); }
  else if CMP("&Dagger;", s)  { CPYSL(s, "/="); }
  else if CMP("&#8225;", s)   { CPYSL(s, "/="); }
  
  /* Modifier Letter Circumflex Accent */
  else if CMP("&#136;", s)    { set_char(s, '^'); }
  else if CMP("&circ;", s)    { set_char(s, '^'); }
  else if CMP("&#710;", s)    { set_char(s, '^'); }

  /* Per Mille Sign */
  else if CMP("&#137;", s)    { CPYSL(s, "0/00"); }
  else if CMP("&permil;", s)  { CPYSL(s, "0/00"); }
  else if CMP("&#8240;", s)   { CPYSL(s, "0/00"); } 

  /* Latin Capital Letter S With Caron */
  else if CMP("&#138;", s)    { set_char(s, 'S'); }
  else if CMP("&Scaron;", s)  { set_char(s, 'S'); }
  else if CMP("&#352;", s)    { set_char(s, 'S'); }

  /* Single Left-Pointing Angle Quotation Mark */
  else if CMP("&#139;", s)    { set_char(s, '<'); }
  else if CMP("&lsaquo;", s)  { set_char(s, '<'); }
  else if CMP("&#8249;", s)   { set_char(s, '<'); }

  /* Latin Capital Ligature OE */
  else if CMP("&#140;", s)    { CPYSL(s, "OE"); }  
  else if CMP("&OElig;", s)   { CPYSL(s, "OE"); }   
  else if CMP("&#338;", s)    { CPYSL(s, "OE"); }  

  /* Z\/ */
  else if CMP("&#142;", s)    { set_char(s, 'Z'); }
  else if CMP("&#381;", s)    { set_char(s, 'Z'); }

  /* Left Single Quotation Mark */
  else if CMP("&#145;", s)    { set_char(s, '`'); }
  else if CMP("&lsquo;", s)   { set_char(s, '`'); }
  else if CMP("&#8216;", s)   { set_char(s, '`'); }

  /* Right Single Quotation Mark */
  else if CMP("&#146;", s)    { set_char(s, '\''); }
  else if CMP("&rsquo;", s)   { set_char(s, '\''); }
  else if CMP("&#8217;", s)   { set_char(s, '\''); }

  /* Left Double Quotation Mark */
  else if CMP("&#147;", s)    { set_char(s, '"'); }
  else if CMP("&ldquo;", s)   { set_char(s, '"'); }
  else if CMP("&#8220;", s)   { set_char(s, '"'); }

  /* Right Double Quotation Mark */
  else if CMP("&#148;", s)    { set_char(s, '"'); }
  else if CMP("&rdquo;", s)   { set_char(s, '"'); }
  else if CMP("&#8221;", s)   { set_char(s, '"'); }

  /* Bullet */
  else if CMP("&#149;", s)    { set_char(s, '*'); }
  else if CMP("&bull;", s)    { set_char(s, '*'); }
  else if CMP("&#8226;", s)   { set_char(s, '*'); }
  
  /* En Dash */
  else if CMP("&#150;", s)    { set_char(s, '-'); }
  else if CMP("&ndash;", s)   { set_char(s, '-'); }
  else if CMP("&#8211;", s)   { set_char(s, '-'); }

  /* Em Dash */
  else if CMP("&#151;", s)    { CPYSL(s, "--"); }
  else if CMP("&mdash;", s)   { CPYSL(s, "--"); }
  else if CMP("&#8212;", s)   { CPYSL(s, "--"); }
  
  /* Small Tilde */
  else if CMP("&#152;", s)    { set_char(s, '~'); }
  else if CMP("&tilde;", s)   { set_char(s, '~'); }
  else if CMP("&#732;", s)    { set_char(s, '~'); } 

  /* Trade Mark Sign */
  else if CMP("&#153;", s)    { CPYSL(s, "[tm]"); }
  else if CMP("&trade;", s)   { CPYSL(s, "[tm]"); }
  else if CMP("&#8482;", s)   { CPYSL(s, "[tm]"); }

  /* Latin Small Letter S With Caron */
  else if CMP("&#154;", s)    { set_char(s, 's'); }
  else if CMP("&scaron;", s)  { set_char(s, 's'); }
  else if CMP("&#353;", s)    { set_char(s, 's'); }

  /* Single Right-Pointing Angle Quotation Mark */
  else if CMP("&#155;", s)    { set_char(s, '>'); }
  else if CMP("&rsaquo;", s)  { set_char(s, '>'); }
  else if CMP("&#8250;", s)   { set_char(s, '>'); }

  /* Latin Small Ligature OE */
  else if CMP("&#156;", s)    { CPYSL(s, "oe"); }
  else if CMP("&oelig;", s)   { CPYSL(s, "oe"); }
  else if CMP("&#339;", s)    { CPYSL(s, "oe"); }

  /* z\/ */
  else if CMP("&#158;", s)    { set_char(s, 'z'); }
  else if CMP("&#382;", s)    { set_char(s, 'z'); }

  /* Latin Capital Letter Y With Diaeresis  */
  else if CMP("&#159;", s)    { set_char(s, 'Y'); }
  else if CMP("&Yuml;", s)    { set_char(s, 'Y'); }
  else if CMP("&#376;", s)    { set_char(s, 'Y'); }
	
  else { return(0); }

  return(1); /* Microsoft entity found */
} /* end microsoft_entities */

/* ------------------------------------------------ */

void microsoft_character(int c)
{
  /*fprintf(stderr, "3 sevenbit %d\n", sevenbit); */
  /*if (sevenbit)  */
  /*{  */
	/*	wort_plus_ch(c); */
	/*	return; */
  /*} */

  switch (c)
  {
	/* Microsoft... */
  case 0x80: /* MICROSOFT EURO */
	WORT_PLUS_STRING("EUR"); break;
	
  case 0x82: /* SINGLE LOW-9 QUOTATION MARK */
	wort_plus_ch(','); break;
  case 0x83: /* Latin Small Letter F With Hook */
	wort_plus_ch('f'); break;
  case 0x84: /* Double Low-9 Quotation Mark */
	WORT_PLUS_STRING("\""); break;
  case 0x85: /* HORIZONTAL ELLIPSIS */
	WORT_PLUS_STRING("..."); break;
  case 0x86: /* Dagger */
	WORT_PLUS_STRING("/-"); break;
  case 0x87: /* Double Dagger */
	WORT_PLUS_STRING("/="); break;
  case 0x88: /* Modifier Letter Circumflex Accent */
	wort_plus_ch('^'); break;
  case 0x89: /* Per Mille Sign */
	WORT_PLUS_STRING("O/oo"); break;
  case 0x8a: /* Latin Capital Letter S With Caron */
	wort_plus_ch('S'); break;
  case 0x8b: /*  Single Left-Pointing Angle Quotation Mark */
	wort_plus_ch('<'); break;
  case 0x8c: /* Latin Capital Ligature OE */
	WORT_PLUS_STRING("OE"); break;
	
  case 0x8e: /* Z\/ */
	wort_plus_ch('Z'); break;
	
  case 0x91: /* LEFT SINGLE QUOTATION MARK */
	wort_plus_ch('`'); break;
  case 0x92: /* RIGHT SINGLE QUOTATION MARK */
	wort_plus_ch('\''); break;
  case 0x93: /* LEFT DOUBLE QUOTATION MARK */
	wort_plus_ch('\"'); break;
  case 0x94: /* RIGHT DOUBLE QUOTATION MARK */
	wort_plus_ch('\"'); break;
  case 0x95: /* BULLET */
	wort_plus_ch('*'); break;
  case 0x96: /* EN DASH */
	wort_plus_ch('-'); break;
  case 0x97: /* EM DASH */
	WORT_PLUS_STRING("--"); break;
  case 0x98: /* SMALL TILDE */
	wort_plus_ch('~'); break;
  case 0x99: /* TRADE MARK SIGN */
	WORT_PLUS_STRING("[tm]"); break;
  case 0x9a: /* LATIN SMALL LETTER S WITH CARON */
	wort_plus_ch('s'); break;
  case 0x9b: /* SINGLE RIGHT-POINTING ANGLE QUOTATION MARK */
	wort_plus_ch('>'); break;
  case 0x9c: /* LATIN SMALL LIGATURE OE */
	WORT_PLUS_STRING("oe"); break;
	
  case 0x9e: /* z\/ */
	wort_plus_ch('z'); break;
  case 0x9f: /* LATIN CAPITAL LETTER Y WITH DIAERESIS */
	wort_plus_ch('Y'); break;
  }
} /* end microsoft_character */
