/*
 * Copyright (C) 1998-2001 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
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

// ------------------------------------------------

void open_files(char *input, char *output)
{
	// cp file names for later use with files_reopen
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
} // end open_files

// ------------------------------------------------

#ifdef MULTIBYTE
void convert_string(char *str, CHAR *converted_string)
{
	size_t result=(size_t)(-1);
	//wchar_t outstring[33]; 
	//wchar_t ret;
	iconv_t conv;
	char output[DEF_STR_LEN];
	char *inp, *outp;
	int fehlernr=0;
	size_t insize, outsize;
	//int k;

	// set locale based on environment variables
	setlocale(LC_CTYPE, "");

/* 	printf("\nstr: "); */
/* 	for (k=0; k<strlen(str); k++) { printf("%c %x ", str[k], str[k]); } */
/* 	printf("\n"); */

	insize = strlen(str);
	outsize = DEF_STR_LEN;

	inp = str;
	outp = output;

	if ((conv = iconv_open("utf-8", "char"))==(iconv_t)(-1))
		{	printf("iconv_open failed in convert_string: Can't convert from %s to UTF-8?\n", locale_charset()); exit(1); }

/* 	printf("convert_string str: "); */
/* 	for (k=0; k<strlen(str); k++) { printf("0x%x ", str[k]&0xff); } */
/* 	printf("\n"); */

	result = iconv(conv, &inp, &insize, &outp, &outsize);
	fehlernr = errno;

/* 		printf("errno %d result %d \n", fehlernr, result); */

/* 	printf("convert_string output: "); */
/* 	for (k=0; k<strlen(output); k++) { printf("%d ", output[k]& 255); } */
/* 	printf("\n"); */

	if (fehlernr==E2BIG) { fprintf(stderr, "errno==E2BIG\n"); }
	else if (fehlernr==EILSEQ) { 
		fprintf(stderr, "Can't convert '%s' as character set %s\n", str, locale_charset());
		fprintf(stderr, "Check your language settings with locale(1)\n");
	}
	else if (fehlernr==EINVAL) { fprintf(stderr, "errno==EINVAL\n"); }
		
	//fprintf(out,"%s\n", output);
	//fputs(output,out); fputc('\n', out);
	//printf("strlen(output) %d\n", strlen(output));
	//write(out, output, strlen(output)); fputc('\n', out);
	output[strlen(output)] = '\0';
	//printf("output schluss -%s-\n", output);
	//strcpy(converted_string, output);

	setlocale(LC_CTYPE, "en_US.utf-8");
	mbstowcs(converted_string, output, strlen(output));

	//printf("converted_string -%ls-\n", converted_string);
	iconv_close(conv);

	//printf("+3+\n");
} // end convert_string
#endif

// ------------------------------------------------

void output_string(CHAR *str)
{
  //Fprintf(out,"%s\n", str);
  //fprintf(stderr, "Hallo\n");
#ifdef MULTIBYTE
	//printf("+1+\n");
  //use_locale();
	//print_locale();
	//printf("+2+\n");  
	//printf("%ls\n", str);
	//TODO
	//char string[DEF_STR_LEN];
	//wcstombs(string, str, strlen(str));
	//fprintf(out,"%s\n", string);

  //fprintf(out,"%ls\n", str);
	if (option_output_utf8) {
		// internal locale ist utf-8
		fprintf(out,"%ls\n", str);
	} else {
		//char string[DEF_STR_LEN];
		//int k;
		//wcstombs(string, str, wcslen(str));
		//setlocale(LC_CTYPE, "en_US.utf-8");
		//fprintf(out,"%s\n", string);
		size_t result=(size_t)(-1);
		//wchar_t outstring[33]; 
		//wchar_t ret;
		iconv_t conv;
		char input[DEF_STR_LEN], output[DEF_STR_LEN];
		char *inp, *outp;
		int fehlernr=0;
		size_t insize, outsize;

		//for (k=0; k<DEF_STR_LEN; k++) { input[k] = 0x00; } // OPTIMIERT
		//for (k=0; k<DEF_STR_LEN; k++) { output[k] = 0x00; } // OPTIMIERT

		result = wcstombs(input, str, DEF_STR_LEN);

		if (result==-1) { printf("Couldn't convert string\n"); }
		insize = strlen(input);
		outsize = DEF_STR_LEN;

		inp = input;
		outp = output;

		if ((conv = iconv_open(get_iconv_charset(), "utf-8"))==(iconv_t)(-1))
			{	printf("iconv_open failed in read_char: wrong character set?\n"); perror(get_iconv_charset()); exit(1); }

/*  		printf("\nstr: "); */
/* 		for (k=0; k<wcslen(str); k++) { printf("0x%x ", str[k]); } */
/*  		printf("\n"); */

/* 		printf("input: "); */
/* 		for (k=0; k<strlen(input); k++) { printf("%d %x ", input[k], input[k]); } */
/* 		printf("\n"); */

		result = iconv(conv, &inp, &insize, &outp, &outsize);
		fehlernr = errno;

/* 		printf("errno %d result %d \n", fehlernr, result); */

/* 		printf("output: "); */
/* 		for (k=0; k<strlen(output); k++) { printf("%d %x ", output[k]& 255, output[k] &255); } */
/* 		printf("\n"); */
/* 		//exit(0); */

		if (fehlernr==E2BIG) { fprintf(stderr, "errno==E2BIG\n"); }
		else if (fehlernr==EILSEQ) { fprintf(stderr, "errno==EILSEQ\n"); }
		else if (fehlernr==EINVAL) { fprintf(stderr, "errno==EINVAL\n"); }
		
		//fprintf(out,"%s\n", output);
		//fputs(output,out); fputc('\n', out);
		//printf("strlen(output) %d\n", strlen(output));
		//write(out, output, strlen(output)); fputc('\n', out);
		output[DEF_STR_LEN-outsize] = '\0';
		fwrite(output, sizeof(output[0]), strlen(output), out); fputc('\n', out);

		iconv_close(conv);
	}

	//printf("+3+\n");
#else
  fprintf(out,"%s\n", str);
#endif
} // end output_string

// ------------------------------------------------

void quit()
{
  if (!is_zeile_empty()) { print_zeile(); }

  href_output();

  fclose(in);
  fclose(out);
  //	fprintf(stderr, "Alles hat ein Ende nur die Wurst hat zwei\n");	
  exit(0);
} /* end quit */

// ------------------------------------------------

int read_char() 
{
	int c = ' ';
#ifdef MULTIBYTE
	int fehlernr=0;
	static int i=0;
	int j=0; 
	size_t result=(size_t)(-1);
	wchar_t outstring[33]; 
	wchar_t ret;
	iconv_t conv;
	char input[33], output[33];
	char *inp, *outp;
	size_t insize = 1, outsize = 32;

	//for (k=0; k<33; k++) { input[k] = 0x00; output[k] = 0x00; }  // OPTIMIERT
	
	inp = input;
	outp = output;

	//use_locale();
  //print_locale();

	//printf("iconv_charset %s\n", get_iconv_charset());

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

		if (c>=128) { 
			//printf("\n%c durchlauf i=%d c=%u %d \n", c, i, c, j); 
		}
		//for (k=0; k<j; k++) { printf("%d %x ", input[k], input[k]); }
		//printf("\n");
		//for (k=0; k<j; k++) { printf("%d %x ", output[k], output[k]); }
		//printf("\n");

		//printf("strlen(output) %d\n", strlen(output));

		if (fehlernr==E2BIG) { printf("errno==E2BIG\n"); }
		else if (fehlernr==EILSEQ) { 
			printf("errno==EILSEQ\n"); 
			//printf("c=%d j=%d ", c, j);
			//for (k=0; k<j; k++) { printf("%d %x ", input[k], input[k]); }
			//printf("\n");
		}
		else if (fehlernr==EINVAL) { /* printf("errno==EINVAL\n");*/ }
		else if (fehlernr==0) {
			if (j>0) {
				result = mbstowcs(outstring, output, strlen(output));
				ret = outstring[0];
				//printf("result: %d j %d ", result, j);
				//if (result!=-1) { printf("outstring %ls\n", outstring); }
				//else { printf("c %x outstring[0..1] %ld %ld\n", c, outstring[0], outstring[1]);}
				//printf("c %x outstring[0..1] %x %x %x\n", c, outstring[0], outstring[1], outstring[2]);
				//printf("c %x  input[0..2] %x %x %x\n", c, input[0], input[1], input[2]);
				//printf("c %x output[0..2] %x %x %x\n", c, output[0], output[1], output[2]);
				c = outstring[0];
			}
		}
		//printf("durchlauf %d %lc %d %d\n", i,c,feof(in), j++);
		j++;
	} while ((fehlernr!=0) && (c!=EOF));
	iconv_close(conv);

	i++;

	errno = 0;
	//printf("%s strlen(output) %d\n", output, strlen(output));
#else
	c=fgetc(in);
#endif

	if (errno!=0) { perror("Caution error: "); }

	if (feof(in)) {
#ifdef MULTIBYTE
			//if (errno==EILSEQ) { printf("EILSEQ\n"); }
#endif			
		quit(); 
		return 0; 
	} else {
		curr_ch = c;
		return c;
	}
} // end read_char

// ------------------------------------------------

int get_current_char()
{
	return(curr_ch);
} // end get_current_char

// ------------------------------------------------

// set back stream p characters
void goback_char(int p)
{
  printf("\nACHTUNG\n");
  fseek(in, -p, SEEK_CUR);
  printf("\nACHTUNG\n");
} // end goback_char

// ------------------------------------------------

// put c back onto stream
void putback_char(CHAR c)
{
  UNGETC (c, in);
} // end putback_char

// ------------------------------------------------

void files_reopen() {	open_files(infile, outfile); }

// ------------------------------------------------
