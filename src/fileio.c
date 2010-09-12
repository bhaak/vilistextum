/*
 * Copyright (c) 1998-2010 Patric Müller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
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

extern char* internal_locale;
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
    fprintf(stderr, "Couldn't open input file %s!\n",input);
    exit(20);
  }

  if ((out = (strcmp(output, "-") ? fopen(output, "w") : stdout))==0)
  {
    fprintf(stderr, "Couldn't open output file %s!\n",output);
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
	char *ret;

	/* set locale based on environment variables */
	ret = setlocale(LC_CTYPE, "");
  if (ret==NULL) { 
		fprintf(stderr, "setlocale failed with: %s\n\n", getenv("LC_CTYPE"));
		exit(1);
	}

	insize = strlen(str);
	if (insize > DEF_STR_LEN) { insize = DEF_STR_LEN; }
	outsize = DEF_STR_LEN;

	inp = str;
	outp = output;

	if ((conv = iconv_open("utf-8", "char"))==(iconv_t)(-1))
		{	fprintf(stderr, "convert_string: iconv_open failed. Can't convert from %s to UTF-8.\n", getenv("LC_CTYPE")); exit(1); }

	result = iconv(conv, &inp, &insize, &outp, &outsize);
	fehlernr = errno;

	if (fehlernr==E2BIG) { fprintf(stderr, "errno==E2BIG\n"); }
	else if (fehlernr==EILSEQ) { 
		fprintf(stderr, "convert_string: Can't interpret '%s' as character from charset %s\n", str, getenv("LC_CTYPE"));
		fprintf(stderr, "convert_string: Check your language settings with locale(1)\n");
	}
	else if (fehlernr==EINVAL) { fprintf(stderr, "convert_string: errno==EINVAL\n"); }
		
	output[strlen(output)] = '\0';

	ret = setlocale(LC_CTYPE, internal_locale);
	if (ret==NULL) { 
		fprintf(stderr, "setlocale failed with: %s\n\n", internal_locale);
		exit(1);
	}
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

		if (result==-1) {
			/* this should not happen */
			fprintf(stderr, "output_string: Couldn't convert string: %ls\n", str);
		}
		insize = strlen(input);
		outsize = DEF_STR_LEN;

		inp = input;
		outp = output;

		if ((conv = iconv_open(get_iconv_output_charset(), "utf-8"))==(iconv_t)(-1))
			{	fprintf(stderr, "output_string: iconv_open failed, wrong character set?\n"); perror(get_iconv_output_charset()); exit(1); }

		/* printf("%s %s\n", get_iconv_output_charset(), inp); */
		result = iconv(conv, &inp, &insize, &outp, &outsize);
		fehlernr = errno;

		if (fehlernr==E2BIG) { fprintf(stderr, "output_string: errno==E2BIG\n"); }
		else if (fehlernr==EILSEQ) { fprintf(stderr, "output_string: errno==EILSEQ in output_string\n"); fprintf(stderr, "input: %s\n", inp); }
		else if (fehlernr==EINVAL) { fprintf(stderr, "output_string: errno==EINVAL\n"); }
		
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
	char *inp, *outp;
	size_t insize = 1, outsize = 32;

	inp = input;
	outp = output;
	
	/* make source the strings are cleared */
	for (j=0; j<33; j++) {
		input[j] = '\0';
		output[j] = '\0';
	}

	/* check if the conversion from the character set from the HTML document
	   to utf-8 is possible */
	if ((conv = iconv_open("utf-8", get_iconv_charset()))==(iconv_t)(-1)) {
		fprintf(stderr, "read_char: iconv_open failed, wrong character set?\n");
		perror(get_iconv_charset());
		exit(1);
	}

	j=0;
	do { 	 
		c=fgetc(in);		
		
		input[j] = c;
		
		errno=0;
		insize = j+1;
		result = iconv(conv, &inp, &insize, &outp, &outsize);
		fehlernr = errno;

		if (fehlernr==E2BIG) { fprintf(stderr, "read_char: errno==E2BIG\n"); }
		/* character is invalid  */
		else if (fehlernr==EILSEQ) { 
			fprintf(stderr, "read_char: errno==EILSEQ; invalid byte sequence for %s: ", get_iconv_charset());
			for (k=0; k<j;k++) {
				fprintf(stderr, "%d ",(unsigned char)input[k]);
			}
			fprintf(stderr, "\n");
			fehlernr=0; j=0;
			c = '?';
		}
		/* incomplete but still valid byte sequence */
		else if (fehlernr==EINVAL) { /* printf("errno==EINVAL\n"); */ }
		/* valid character found */
		else if (fehlernr==0) {
			if (mbstowcs(outstring, output, strlen(output)) != -1) {
				c = outstring[0];
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
