/*
 * Copyright (c) 1998-2011 Patric Mueller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 *  history
 *  14.10.2001: creation of this file
 *
 */

#include <../config.h>

#define _POSIX_C_SOURCE 2 /* for popen, pclose */
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

static char *default_charset = "iso-8859-1";

static char iconv_charset[DEF_STR_LEN];
static char iconv_output_charset[DEF_STR_LEN] = "";

static int usr=0;

#ifdef MULTIBYTE
iconv_t conv;
char internal_locale[256];
#endif

/* ------------------------------------------------ */

#ifdef MULTIBYTE
static int suffix(const char * str, const char * suffix)
{
	if ( strlen(str) < strlen(suffix) ) return 0;
	if ( ! strcmp(suffix, str + ( strlen(str) - strlen(suffix) ) ) ) return 1;
	return 0;
}

static int utf_8_locale(const char * locale)
{
	if (!locale) return 0;
	return suffix(locale,".utf8") || suffix(locale, ".UTF-8");
}

void init_multibyte()
{
	char *locale_found;
	if ((locale_found = setlocale(LC_CTYPE, INTERNAL_LOCALE))) {
		strcpy(internal_locale, INTERNAL_LOCALE);
	} else {
		/* read all available locales */
		FILE *fp = popen("locale -a", "r");
		if (fp) {
			while (!feof(fp) && !locale_found) {
				char buf[256];
				if (fgets(buf, sizeof(buf), fp) != NULL) {
					/* remove newline */
					buf[strlen(buf)-1] = '\0';
					/* check for a working UTF-8 locale */
					if (utf_8_locale(buf) &&
					    (locale_found = setlocale(LC_CTYPE, buf))) {
						strcpy(internal_locale, buf);
					}
				}
			}
		}

		if (locale_found == NULL) {
			fprintf(stderr, "Could not find a UTF-8 locale! Please enable en_US.UTF-8!\n");
			exit(EXIT_FAILURE);
		}
		pclose(fp);
	}
}
#endif

/* ------------------------------------------------ */

#ifdef MULTIBYTE
char* get_iconv_charset()
{
	return(iconv_charset);
}

/* ------------------------------------------------ */

char* get_iconv_output_charset()
{
	/* check if there's a different output character set */
	if (strlen(iconv_output_charset) > 0) {
		return(iconv_output_charset);
	} else {
		return(iconv_charset);
	}
}

/* ------------------------------------------------ */

char* get_iconv_output_charset();
int convert_character(int num, CHAR *outstring)
{
	char in[33], out[33];
	size_t result=(size_t)(-1);
	int i;
	int converted; /* has the entity been successfully converted */
	
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

/* 	printf("iconv_output_charset: -%s-\n", get_iconv_output_charset()); */
/* 	printf("insize %d\n", insize); */
	if ((conv = iconv_open(get_iconv_output_charset(), "utf-8"))==(iconv_t)(-1)) 
		{	printf("iconv_open failed in convert_character: wrong character set?\n"); perror(get_iconv_output_charset()); exit(1); } 
	
	result = iconv(conv, &inp, &insize, &outp, &outsize);
	iconv_close(conv);
	
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
			outstring[0] = L' '; outstring[1] = L'\0';
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

void set_iconv_charset(char *charset) {
	/* set charset for iconv conversion */
	strcpy(iconv_charset, charset);
	if (transliteration) { strcat(iconv_charset, "//TRANSLIT");}
	/* printf("iconv_charset %s\n", iconv_charset); DEBUG */
}

/* ------------------------------------------------ */

void set_iconv_output_charset(char *charset) {
	/* set charset for iconv conversion */
	strcpy(iconv_output_charset, charset);
	if (transliteration) { strcat(iconv_charset, "//TRANSLIT");}
	/* printf("iconv_output_charset %s\n", iconv_output_charset); DEBUG */
}

/* ------------------------------------------------ */

void use_default_charset() {
	set_iconv_charset(default_charset);
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
