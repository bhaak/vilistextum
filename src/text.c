/*
 * Copyright (C) 1998-2004 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
 *
 * 04.09.01: Fixed --shrink-lines=n
 */

#include "multibyte.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "text.h"
#include "main.h"
#include "html.h"
#include "debug.h"
#include "fileio.h"
#include "util.h"

int LEFT = 1,
  CENTER = 2,
  RIGHT  = 3;

int breite=76,
    hr_breite=76,
    paragraph=0,
    leerer_paragraph=0,

    tab=4,         /* Tabulator Schrittweite */
    spaces=0,      /* Space einfügen bei Zeilenanfang */
	  nooutput=0,    /* für SCRIPT u.ä.: keine Ausgabe */
  	orderedlist=0, /* OL */

    div_test=0;

CHAR wort[DEF_STR_LEN];

CHAR zeile[DEF_STR_LEN];
int     zeilen_len=0,       /* scheinbare Länge der Zeile  */
  zeilen_len_old=0,
  zeilen_pos=0,       /* wahre Länge der Zeile  */
  wort_len=0,         /* scheinbare Länge des Worts */
  wort_pos=0,         /* wahre Länge des Worts  */
  anz_leere_zeilen=0, /* wieviele Zeilen waren bisher leer */
  noleadingblanks=0;  /* keine Leerzeilen am Anfang. */

/* ------------------------------------------------ */

void center_zeile()
{
  int i,j;
#ifdef proc_debug
  printf("\ncenter_zeile()\n");
#endif

  if (!palm)
  {
    if (zeilen_len!=0)
    {
      j=(breite-zeilen_len)/2;

#ifdef center_debug
      printf(" center-zeile; len: %d; j: %d\n",zeilen_len,j);
      printf(" %s\n",zeile);
#endif
      for (i=zeilen_pos+j+2; i>=0; i--)
      {
#ifdef center_debug
	printf(" %c",zeile[i]);
#endif
	zeile[i+j]=zeile[i];
      }
      for (i=0; i<j; i++) { zeile[i]=' '; }
#ifdef center_debug
      printf(" \n%s\n\n",zeile);
      printf(" --------------------------------------------------\n");
#endif
    }
#ifdef proc_debug
  printf("center_zeile() ende\n");
#endif
  }
} /* end center_zeile */

/* ------------------------------------------------ */

void right_zeile()
{
  int i,j;
#ifdef proc_debug
  printf("\nright_zeile()\n");
#endif

  if (!palm)
  {
    if (zeilen_len!=0)
    {
      j=breite-zeilen_len;

#ifdef right_debug
      printf(" right-zeile; len: %d; j: %d\n",zeilen_len,j);
      printf(" %s\n",zeile);
#endif
      for (i=zeilen_pos+j+2; i>=0; i--)
      {
#ifdef right_debug
	printf(" %c",zeile[i]);
#endif
	zeile[i+j]=zeile[i];
      }
      for (i=0; i<j; i++) { zeile[i]=' '; }
#ifdef right_debug
      printf(" \n%s\n\n",zeile);
      printf(" --------------------------------------------------\n");
#endif
    }
  }
#ifdef proc_debug
  printf("right_zeile() ende\n");
#endif
} /* end right_zeile */

/* ------------------------------------------------ */

/* return true, if z is all spaces or nonbreakable space */
int only_spaces(CHAR *z)
{
  int len=STRLEN(z);
	int i, ret=1;
	CHAR j;

	for (i=0; i<len; i++) { j=z[i]; ret = (ret && ((j==' ')||(j==160))); }
	return(ret);
} /* end only_spaces */

/* ------------------------------------------------ */

void print_zeile()
{
  int printzeile;
#ifdef proc_debug
  printf("\nprint_zeile()\n");
  printf("align[align_nr]: %d\n", get_align());
#endif

  if ((shrink_lines) && only_spaces(zeile)) 
  { 
		zeile[0]='\0';
		zeilen_len=0; zeilen_pos=0;
		anz_leere_zeilen++;	
  } else {
		anz_leere_zeilen=0;
  }	

  if (shrink_lines==0)
  {
		printzeile = (!((zeilen_len==0)&&(zeilen_len_old==0)));
  } else {
		printzeile = (!((anz_leere_zeilen>shrink_lines)||(noleadingblanks==0)));
  }

  /* Don't allow leading blank lines */
	if (noleadingblanks==0) { noleadingblanks = !only_spaces(zeile); }

  /*  fprintf(stderr, "anz_leere_zeilen %d zeilen_len %d zeilen_len_old %d noleadingblanks %d nooutput %d zeile %ls \n", anz_leere_zeilen, zeilen_len, zeilen_len_old, noleadingblanks, nooutput, zeile); */

  if (printzeile) 
	{
	  if (get_align()==LEFT)   {}
	  if (get_align()==CENTER) { center_zeile(); }
	  if (get_align()==RIGHT)  { right_zeile(); }

	  if (!nooutput) { output_string(zeile); }
	  /*#ifdef debug		 */
	  else { /* print_error("keine ausgabe von ", zeile); */ }
	  /*#endif */
		
	  zeile[0]='\0';
	  zeilen_len_old=zeilen_len;
	  zeilen_len=0; zeilen_pos=0;
  }
#ifdef proc_debug
  printf("print_zeile() ende\n");
#endif
} /* end print_zeile */

/* ------------------------------------------------ */

int is_zeile_empty()
{
	return(zeile[0]=='\0');
} /* end is_zeile_empty */

/* ------------------------------------------------ */

void status()
{
	printf(" paragraph: %d; div_test: %d; align[align_nr]: %d; z_o: %d\n",paragraph, div_test, get_align(), zeilen_len_old);

#ifdef wort_ende_debug
  printf(" zeile: %s\n",zeile);
  printf(" wort: %s\n",wort);
	printf(" nooutput:   %d\n", nooutput);
  printf(" zeilen_len: %d ; wort_len: %d\n",zeilen_len,wort_len);
  printf(" zeilen_pos: %d ; wort_pos: %d\n",zeilen_pos,wort_pos);
#endif
}

/* ------------------------------------------------ */

void zeile_plus_wort(CHAR *s, int wl, int wp)
{
  int i=zeilen_pos,
      j=0;
#ifdef proc_debug
  printf("\nzeile_plus_wort()\n");
#endif

  while (i<zeilen_pos+wp) { zeile[i] = s[j]; j++; i++; }
  zeile[i] = '\0';
  zeilen_len += wl; zeilen_pos += wp;
#ifdef proc_debug
  printf("zeile_plus_wort() ende\n");
#endif
} /* end zeile_plus_wort */

/* ------------------------------------------------ */

void wort_plus_string_nocount(CHAR *s)
{
  int len=STRLEN(s),
      i=wort_pos,
      j=0;
#ifdef proc_debug
  printf("\nwort_plus_string_nocount()\n");
#endif

  if (!palm)
  {
    while (i<wort_pos+len) { wort[i] = s[j]; j++; i++; }
    wort[i] = '\0';
    wort_pos += len;
  }

#ifdef proc_debug
  printf("wort_plus_string_nocount() ende\n");
#endif
} /* end wort_plus_string_nocount */

/* ------------------------------------------------ */

void wort_plus_string(CHAR *s)
{
  int len=STRLEN(s),
      i=wort_pos,
      j=0;
#ifdef wort_ende_debug
  printf("\nwort_plus_string()\n");
#endif
#ifdef wort_ende_debug
  printf("s: %ls len: %d\n", s,len);
#endif

  while (i<wort_pos+len) { wort[i] = s[j]; j++; i++; }
  wort[i] = '\0';
  wort_pos += len; wort_len += len;

#ifdef proc_debug
  printf("wort_plus_string() ende\n");
#endif
} /* end wort_plus_string */

/* ------------------------------------------------ */

void wort_plus_ch(int c)
{

#ifdef default_debug
  printf("==wort_plus_ch(%d)\n", c);
#endif
  wort[wort_pos++] = c;
  wort_len++;
} /* end wort_plus */

/* ------------------------------------------------ */

void wort_ende()
{
  int i=0;
#ifdef proc_debug
    printf("wort_ende()\n");
#endif
  if (wort_len > 0)
  {
    wort[wort_pos] = '\0';

    if (zeilen_len+wort_len+1 > breite)
    {
#ifdef wort_ende_debug
      printf(" z0: zeilen_len: %d\n",zeilen_len);
      printf(" z0: zeilen_pos: %d\n",zeilen_pos);
#endif
      print_zeile();
      i=0;
      while (i<spaces) { zeile_plus_wort(ONESPACE,1,1); i++; }
			if (orderedlist>0) { zeile_plus_wort(ONESPACE,1,1); }
      zeile_plus_wort(wort, wort_len, wort_pos);
    }
    else if (zeilen_len != 0)
    {
#ifdef wort_ende_debug
      printf(" z1: zeilen_len: %d ; wort_len %d \n",zeilen_len, wort_len);
      printf(" z1: zeilen_pos: %d ; wort_pos %d\n",zeilen_pos, wort_pos);
#endif
			/* Leerzeichen + Wort */
      zeile_plus_wort(ONESPACE,1,1); zeile_plus_wort(wort,wort_len, wort_pos);
    }
    else /* zeilen_len==0 => neuer Paragraphenanfang */
    {
#ifdef wort_ende_debug
      printf(" z2: zeilen_len: %d\n",zeilen_len);
      printf(" z2: zeilen_pos: %d\n",zeilen_pos);
#endif
      i=0;
      while (i<spaces) { zeile_plus_wort(ONESPACE,1,1); i++; }
			if (orderedlist>0) { zeile_plus_wort(ONESPACE,1,1); }
      zeile_plus_wort(wort,wort_len, wort_pos);
    }
    wort_pos = 0;
    wort_len = 0;
    /* if (zeilen_len==0) { leerer_paragraph++; } */
  }

#ifdef proc_debug
  printf("wort_ende() ende\n");
#endif
} /* end wort_ende */

/* ------------------------------------------------ */

void line_break()
{
  wort_ende();
  print_zeile();
} /* end line_break */

/* ------------------------------------------------ */

void paragraphen_ende()
{
  if (paragraph!=0)
  {
#ifdef proc_debug
    printf("paragraph_ende()\n");
    printf("zeilen_len: %d\n", zeilen_len);
#endif
    line_break();
    print_zeile();
    paragraph--;

    pop_align();
  }
#ifdef proc_debug
  printf("paragraphen_ende() ende\n");
#endif
} /* end paragraphen_ende */

/* ------------------------------------------------ */

void neuer_paragraph()
{
#ifdef proc_debug
  printf("neuer_paragraph()\n");
#endif
  /*fprintf(stderr, "paragraph %d\n", paragraph); */
  if (paragraph!=0) { paragraphen_ende(); }
  line_break();
  print_zeile();
  paragraph++;
  /* leerer_paragraph--; */
#ifdef proc_debug
  printf("neuer_paragraph() ende\n");
#endif
} /* end neuer_paragraph */

/* ------------------------------------------------ */

void hr()
{
  int i, hr_width=hr_breite-4, hr_align=CENTER;
#ifdef proc_debug
  printf("hr()\n");
#endif
  while (ch!='>')
  {
    ch=get_attr();
#ifdef attr_debug
    printf(" HR-Attr: %s; Inhalt: %s#\n",attr_name,attr_ctnt);
#endif
    if CMP("ALIGN", attr_name)
    {
      uppercase_str(attr_ctnt);
      if CMP("LEFT", attr_ctnt) { hr_align=LEFT;   }
      else if CMP("CENTER",  attr_ctnt) { hr_align=CENTER; }
      else if CMP("RIGHT",   attr_ctnt) { hr_align=RIGHT;  }
      else if CMP("JUSTIFY", attr_ctnt) { hr_align=LEFT;  }			
      else { if (errorlevel>=2) { fprintf(stderr, "No LEFT|CENTER|RIGHT found!\n");} }
    }
    else if CMP("WIDTH", attr_name)
    {
      i=STRLEN(attr_ctnt);
      if (attr_ctnt[i-1]=='%') {
        attr_ctnt[i-1] = '\0';
        hr_width = ATOI(attr_ctnt);
        if (hr_width==100) { hr_width = hr_breite-4; }
        else { hr_width = hr_breite*hr_width/100; }
      } else {
        hr_width = ATOI(attr_ctnt)/8;
        if (hr_width>hr_breite-4) { hr_width = hr_breite-4; }
      }
    }
  }

  neuer_paragraph();
  push_align(hr_align);
  for (i=0; i<hr_width; i++) { wort_plus_ch('-'); }
  paragraphen_ende();
#ifdef proc_debug
  printf("hr() ende\n");
#endif
} /* end hr */
