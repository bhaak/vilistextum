/*
 * Copyright (C) 1998-2001 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
 *
 * 08.03.02: align[0] hasn't been set by push_align
 * 18.02.02: some multibyte code not enclosed by define's
 *           uppercase now available in onebyte and multibyte version
 *           include ctype.h for toupper
 * 10.04.02: changed the align stack code to let it work on the amiga
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

#include "main.h"
#include "debug.h"
#include "multibyte.h"


#ifdef MULTIBYTE
# include <wctype.h>
#endif

// Dynamic align added by autophile@starband.net 29 Mar 2002
int *align = NULL,
  	align_nr=0;
int align_size = 0;

// ------------------------------------------------

int get_align()
{
  // Dynamic align added by autophile@starband.net 29 Mar 2002
  if (align==NULL)
  {
    align = (int *)malloc(256*sizeof(int));
    align[0] = 1; // default LEFT alignment.
  }
	//printf("align[%d] %d\n", align_nr, align[align_nr]);
	return(align[align_nr]);
} // end get_align

// ------------------------------------------------

void push_align(int a)
{
	int *tmp_align = NULL;
#ifdef proc_debug
  printf("push_align(%d)\n", a);
#endif
  align_nr++;

  // Dynamic align added by autophile@starband.net 29 Mar 2002
  if (align_nr > align_size)
  {
    align_size += 256;
    if (align == NULL) {
      align = (int *)malloc(align_size*sizeof(int));
			align[0] = 1; // default LEFT alignment.
    } else {
      //align = (int *)realloc(align, align_size*sizeof(int));
			tmp_align = (int *)malloc(align_size*sizeof(int));
		  if (align_size!=0) { memcpy(tmp_align, align, (align_size-256)*sizeof(int)); } 
			align = tmp_align;
		}
  }

	//	if (div_test!=0) { align[align_nr]=div_test; } else { 
	align[align_nr]=a; //}
}

void pop_align()
{
  if (align_nr==0) { if (errorlevel>=5) { fprintf(stdout, "Error: align_nr=0\n");} }
  else { align_nr--; }
	//printf("align_nr %d\n", align_nr);
#ifdef proc_debug
  printf("pop_align()\n");
#endif
}

// ------------------------------------------------

#ifdef MULTIBYTE
wint_t uppercase(wint_t c)
{
  if ((c>='a') && (c<='z')) { c=towupper(c); }
  return c;
} /* end uppercase */
#else
int uppercase(int c)
{
  if ((c>='a') && (c<='z')) { c=toupper(c); }
  return c;
} /* end uppercase */
#endif

// ------------------------------------------------

void uppercase_str(CHAR *s)
{
  int i=0;
#ifdef proc_debug
  printf("uppercase()\n");
#endif
#ifdef proc_debug
  printf("%s -> ", s);
#endif
  while(s[i]!='\0') { s[i]=uppercase(s[i]); i++; }

#ifdef proc_debug
  printf("%s\n", s);
  printf("uppercase_str() ende\n");
#endif
} /* end uppercase_str */

// ------------------------------------------------

// copy the character to the string
void set_char(CHAR *s, CHAR c)
{
  //	printf("set_char1: %s=\n", s);
  s[0] = c;
  s[1] = '\0';
  //	printf("set_char2: %s=\n", s);
} // end set_char

// ------------------------------------------------

int x2dec(CHAR *str, int base)
{
  int i=0,
	nr=0;
  int len=STRLEN(str);

  for (i=0;i<len;i++)
  {
    nr*=base;
    if ((str[i]>='0') && (str[i]<='9')) { nr += str[i]-'0'; }
    else 
    {  
#ifdef MULTIBYTE
      str[i] = towupper(str[i])-'A'+10; 
#else
      str[i] = toupper(str[i])-'A'+10; 
#endif
	  if ((str[i]>=10) && (str[i]<base)) { nr += str[i]; }
	  else { return(nr/base); }
    }
  }
  return nr;
} /* end x2dec */

// ------------------------------------------------

void print_error(char *error, CHAR *text)
{
#ifdef MULTIBYTE
	fprintf(stderr, "%s%ls\n", error, text);
#else
	fprintf(stderr, "%s%s\n", error, text);
#endif

} /* end print_error */
