/*
 * Copyright (C) 1998-2002 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
 *
 *  history
 *  14.10.2001: creation of this file
 *
 *
 */

#include <../config.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <locale.h>
#include <stdlib.h>

#ifdef MULTIBYTE
#include <iconv.h>
#endif

#include "main.h"
#include "text.h"
#include "multibyte.h"

char *default_charset = "iso-8859-1";

char iconv_charset[DEF_STR_LEN];

int usr=0;

#ifdef MULTIBYTE
iconv_t conv;
#endif

/* ------------------------------------------------ */

#ifdef MULTIBYTE
void init_multibyte()
{
	char *ret;
	ret = setlocale(LC_CTYPE, "en_US.utf-8"); 
  if (ret==NULL) { fprintf(stderr, "setlocale failed with: en_US.utf8\n\n"); }
  /*else { fprintf(stderr, "%s\n", ret); } */
}
#endif

/* ------------------------------------------------ */

#ifdef MULTIBYTE
int convert_character(int num, CHAR *outstring)
{
	char in[33], out[33];
	size_t result=(size_t)(-1);
	int i;
	int converted; /* has the entity been successfully converted */
	
#ifdef HAVE_ICONV_H
	char *inp, *outp;
	size_t insize = 1, outsize = 32;

	/* no conversion needed */
	if (option_output_utf8) {
		outstring[0] = num;
		outstring[1] = L'\0';
		return 1;
	}

#ifdef proc_debug
	printf("convert_character start\n");
#endif

	/*#define iconv_debug 1 */
	
	for (i=0; i<33; i++) { in[i]=0x00; out[i]=0x00; }
	inp  = in;
	outp = out;
	insize = wctomb(inp, num);

/* 	printf("iconv_charset: -%s-\n", iconv_charset); */
/* 	printf("insize %d\n", insize); */
	if ((conv = iconv_open(iconv_charset, "utf-8"))==(iconv_t)(-1)) 
		{	printf("iconv_open failed in convert_character: wrong character set?\n"); perror(iconv_charset); exit(1); } 
	
	result = iconv(conv, &inp, &insize, &outp, &outsize);
	iconv_close(conv);
	
#else
	for (i=0; i<33; i++) { in[i]=0x00; out[i]=0x00; }
#endif
	if (result==(size_t)(-1)) 
	{	
#ifdef iconv_debug
		printf("convert_character: not converted: &#%d;", num);
#endif
		/*swprintf (outstring, 32, L"&#%d;\0", num); */
		/*fprintf(stdout, "%ls\n", outstring); */
		converted = 0;
		/* if the entity is 160 (nbsp), use ' ' instead */
		if (num==160) {
			converted = 1;
			outstring[0] = L' ';
			outstring[1] = L'\0';
		}
	} else {
#ifdef iconv_debug
		printf("convert_character: Input '%s'; Output '%s'\n", in, out);
#endif
		converted = 1;
		outstring[0] = num;
		outstring[1] = L'\0';
#ifdef iconv_debug
		printf("convert_character: Result %d; outstring: %ls: out %s\nstrlen(out) %d; \n", result, outstring, out, strlen(out));
#endif		
	}
#ifdef iconv_debug
	printf("convert_character: fertig %d -%ls-\n", converted, outstring);
#endif


#ifdef proc_debug
	printf("convert_character end\n\n");
#endif
	return(converted);
} /* end convert_character */
#endif

/* ------------------------------------------------ */

char* get_iconv_charset()
{
	return(iconv_charset);
}

/* ------------------------------------------------ */

void set_iconv_charset(char *charset) {
	/* set charset for iconv conversion */
	strcpy(iconv_charset, charset);
	if (transliteration) { strcat(iconv_charset, "//TRANSLIT");}
	/* printf("iconv_charset %s\n", iconv_charset); */
}

/* ------------------------------------------------ */

#if 0
void set_locale(char *charset) 
{
  /* if charset == "", take the value from the environment variables */
  char *ret; 
  /*ret = setlocale(LC_CTYPE, ""); */
	char country_charset[DEF_STR_LEN]; 
	if (usr==0) 
	{ 
#ifdef proc_debug 
	printf("set_locale start\n"); 
#endif 

	/* set charset for iconv conversion */
	strcpy(iconv_charset, charset); 
	if (transliteration) { strcat(iconv_charset, "//TRANSLIT");} 
	/*printf("iconv_charset %s\n", iconv_charset); */

	strcpy(country_charset, default_country); 
	strcat(country_charset, charset); 

  ret = setlocale(LC_CTYPE, country_charset);  
	/*printf("Locale: =%s=\n", country_charset); */
	strcpy(this_complete_locale, country_charset); 
	strcpy(this_locale, charset); 
  if (ret==NULL) { fprintf(stderr, "setlocale failed with: %s\n\n", country_charset); } 
  else { } // fprintf(stderr, "%s\n", ret); }  */
#ifdef proc_debug 
	printf("set_locale end\n"); 
#endif
	} 
} /* end set_locale */
#endif

/* ------------------------------------------------ */

/*void use_locale()  { } printf("using this locale %s\n", this_locale); set_locale(this_locale); } */
void use_default_charset() { set_iconv_charset(default_charset); }

/* ------------------------------------------------ */

void set_usr_locale(char *user_locale)
{
	set_iconv_charset(user_locale);
 	usr=1;
}

/* ------------------------------------------------ */
#ifdef MULTIBYTE
void strip_wchar(CHAR *locale, char *stripped_locale)
{
	CHAR *in  = locale;
	char *out = stripped_locale;
	int len;
	int i;
#ifdef proc_debug
	printf("strip_wchar start\n");
#endif

	/*printf("locale: -%d-%d-\n", locale[0], locale[1]); */

	len = STRLEN(locale);
	/* copy stripped string to out */
	for (i=0; i<len; i++) { out[i] = wctob(in[i]); }
	out[i] = 0x00;
	/*printf("out %s\n", stripped_locale); */
#ifdef proc_debug
	printf("strip_wchar end\n");
#endif
} /* end strip_wchar */
#endif
/* ------------------------------------------------ */

void print_locale() { 
	/* printf("%s\n", this_locale); */
}

/* ------------------------------------------------ */
