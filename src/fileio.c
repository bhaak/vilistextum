/*
 * Copyright (C) 1998-2004 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 *  history
 *  18.04.2001 : incorporated stdin/stdout patch from Luke Ravitch
 */

#include "multibyte.h"
#include "charset.h"

#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "text.h"
#include "html.h"
#include "charset.h"
#include "multibyte.h"
#include "main.h"

#ifdef MULTIBYTE
  #include <iconv.h>
  #include <locale.h>
  #include <localcharset.h>
#endif

FILE *in, *out;

CHAR curr_ch;

char infile[DEF_STR_LEN], outfile[DEF_STR_LEN];

/* ------------------------------------------------ */

void open_files(char *input, char *output)
{
	/* cp file names for later use with files_reopen */
	strcpy(infile, input);
	strcpy(outfile, output);

  if ((in = (strcmp(input, "-") ? fopen(input, "r") : stdin))==0)  
  {
    fprintf(stderr, "Couldn't open %s!\n",input);
    exit(20);
  }

  if ((out = (strcmp(output, "-") ? fopen(output, "w") : stdout))==0)
  {
    fprintf(stderr, "Couldn't open %s!\n",output);
    exit(20);
  }
} /* end open_files */

/* ------------------------------------------------ */

#ifdef MULTIBYTE
void convert_string(char *str, CHAR *converted_string)
{
	size_t result=(size_t)(-1);
	/*wchar_t outstring[33];  */
	/*wchar_t ret; */
	iconv_t conv;
	char output[DEF_STR_LEN];
	char *inp, *outp;
	int fehlernr=0;
	size_t insize, outsize;

	/* set locale based on environment variables */
	setlocale(LC_CTYPE, "");

	insize = strlen(str);
	outsize = DEF_STR_LEN;

	inp = str;
	outp = output;

	if ((conv = iconv_open("utf-8", "char"))==(iconv_t)(-1))
		{	printf("iconv_open failed in convert_string: Can't convert from %s to UTF-8?\n", locale_charset()); exit(1); }

	result = iconv(conv, &inp, &insize, &outp, &outsize);
	fehlernr = errno;

	if (fehlernr==E2BIG) { fprintf(stderr, "errno==E2BIG\n"); }
	else if (fehlernr==EILSEQ) { 
		fprintf(stderr, "Can't convert '%s' as character set %s\n", str, locale_charset());
		fprintf(stderr, "Check your language settings with locale(1)\n");
	}
	else if (fehlernr==EINVAL) { fprintf(stderr, "errno==EINVAL\n"); }
		
	output[strlen(output)] = '\0';

	setlocale(LC_CTYPE, INTERNAL_LOCALE);
	mbstowcs(converted_string, output, strlen(output));

	iconv_close(conv);

} /* end convert_string */
#endif

/* ------------------------------------------------ */

void output_string(CHAR *str)
{
#ifdef MULTIBYTE
	if (option_output_utf8) {
		/* internal locale is utf-8, no conversion needed */
		fprintf(out,"%ls\n", str);
	} else {
		size_t result=(size_t)(-1);

		iconv_t conv;
		char input[DEF_STR_LEN], output[DEF_STR_LEN];
		char *inp, *outp;
		int fehlernr=0;
		size_t insize, outsize;

		result = wcstombs(input, str, DEF_STR_LEN);

		if (result==-1) { printf("Couldn't convert string\n"); } /* this should not happen */
		insize = strlen(input);
		outsize = DEF_STR_LEN;

		inp = input;
		outp = output;

		if ((conv = iconv_open(get_iconv_charset(), "utf-8"))==(iconv_t)(-1))
			{	printf("iconv_open failed in read_char: wrong character set?\n"); perror(get_iconv_charset()); exit(1); }

		/* printf("%s %s\n", get_iconv_charset(), inp); */
		result = iconv(conv, &inp, &insize, &outp, &outsize);
		fehlernr = errno;

		if (fehlernr==E2BIG) { fprintf(stderr, "errno==E2BIG\n"); }
		else if (fehlernr==EILSEQ) { fprintf(stderr, "errno==EILSEQ in output_string\n"); }
		else if (fehlernr==EINVAL) { fprintf(stderr, "errno==EINVAL\n"); }
		
		output[DEF_STR_LEN-outsize] = '\0';
		fwrite(output, sizeof(output[0]), strlen(output), out); fputc('\n', out);

		iconv_close(conv);
	}
#else
  fprintf(out,"%s\n", str);
#endif
} /* end output_string */

/* ------------------------------------------------ */

void quit()
{
  if (!is_zeile_empty()) { wort_ende(); print_zeile(); }

  href_output();

  fclose(in);
  fclose(out);

  exit(0);
} /* end quit */

/* ------------------------------------------------ */

int read_char() 
{
	int c = ' ';
#ifdef MULTIBYTE
	int fehlernr=0; /* tmp variable for errno */
	static int i=0;
	int j=0,k; 
	size_t result=(size_t)(-1);
	wchar_t outstring[33]; 
	iconv_t conv;
	char input[33], output[33];
	CHAR tmpstr[33];
	char *inp, *outp;
	size_t insize = 1, outsize = 32;

	inp = input;
	outp = output;

	if ((conv = iconv_open("utf-8", get_iconv_charset()))==(iconv_t)(-1)) 
		{	printf("iconv_open failed in read_char: wrong character set?\n"); perror(get_iconv_charset()); exit(1); }

	j=0;
	do { 	 
		c=fgetc(in);		
		
		input[j] = c;
		
		errno=0;
		insize = j+1;
		result = iconv(conv, &inp, &insize, &outp, &outsize);
		fehlernr = errno;

		if (fehlernr==E2BIG) { printf("errno==E2BIG\n"); }
		/* character is invalid  */
		else if (fehlernr==EILSEQ) { 
			fprintf(stderr, "errno==EILSEQ; invalid byte sequence in read_char for %s: ", get_iconv_charset()); 
			for (k=0; k<j;k++) {
				fprintf(stderr, "%d ",(unsigned char)input[k]);
			}
			printf("\n");
			fehlernr=0; j=0;
			c = '?';
		}
		/* incomplete but still valid byte sequence */
		else if (fehlernr==EINVAL) { /* printf("errno==EINVAL\n"); */ }
		/* valid character found */
		else if (fehlernr==0) {
			/* printf("\n1: c=%d ; %d\n",c,c); */
				result = mbstowcs(outstring, output, strlen(output));
				if (convert_character(outstring[0], tmpstr)) {
					c = outstring[0];
					/* printf("2: c=%d ; %d\n",c,c); */
				} else {
					/* printf("microsoft character: %d\n", c); */
				}
		}

		j++;
	} while ((fehlernr!=0) && (c!=EOF));
	iconv_close(conv);

	i++;

	errno = 0;

#else
	c=fgetc(in);
#endif

	if (feof(in)) {
		quit(); 
		return 0; 
	} else {
		curr_ch = c;
		return c;
	}
} /* end read_char */

/* ------------------------------------------------ */

int get_current_char()
{
	return(curr_ch);
} /* end get_current_char */

/* ------------------------------------------------ */

/* set back stream p characters */
void goback_char(int p)
{
  printf("\nACHTUNG\n");
  fseek(in, -p, SEEK_CUR);
  printf("\nACHTUNG\n");
} /* end goback_char */

/* ------------------------------------------------ */

/* put c back onto stream */
void putback_char(CHAR c)
{
  UNGETC (c, in);
} /* end putback_char */

/* ------------------------------------------------ */

void files_reopen() {	open_files(infile, outfile); }

/* ------------------------------------------------ */
