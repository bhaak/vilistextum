/*
 * Copyright (C) 1998-2004 Patric Müller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 *
 *  history
 *  19.04.2001: get_attr parses text of a alt attribute 
 *  20.04.2001: added references ala lynx
 *  24.08.2001: Frisskommentar could'nt cope with <!--comment-->  
 *  03.09.2001: check_for_center worked only correct if align was last attribute
 *  22.03.2002: process_meta crashed if Contenttype was provided, but no charset
 *  10.04.2002: corrected check_for_center to prevent align_errors.
 *  28.01.2003: TAB and CR treated as white space.
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "html_tag.h"
#include "text.h"
#include "microsoft.h"
#include "latin1.h"
#include "fileio.h"
#include "debug.h"
#include "charset.h"
#include "util.h"

int pre=0; /* for PRE-Tag */
int processed_meta=0; /* only parse meta tags once */

CHAR attr_name[DEF_STR_LEN], /* Attribut Name of a HTML-Tag */
	    attr_ctnt[DEF_STR_LEN]; /* Attribut Inhalt of a HTML-Tag */

/* ------------------------------------------------ */

/* get the next attribute writes it to attr_name and attr_ctnt. */
/* attr_name is converted to uppercase.  */
int get_attr() /* FIXME change to get_attr(char *name, char *ctnt) */
{
  int i;
	CHAR temp[DEF_STR_LEN];
#ifdef proc_debug 
  printf("get_attr()\n");
#endif

  attr_name[0] = '\0';
  attr_ctnt[0] = '\0';

/* printf("character %c %d\n", ch, ch); */

  while ((isspace(ch)) && (ch!='>')) { ch=read_char(); /* printf("read_char %c %d\n", ch, ch);*/ }
  if (ch=='>') { return '>'; };
  /*  printf("nach return %c %d\n", ch, ch); */

  /* read attribute's name */
  i=1;
  attr_name[0] = ch;

	while ((ch!='=') && (ch!='>'))
	{ ch=read_char(); attr_name[i++] = ch; }
  attr_name[i-1] = '\0';
	
  if (ch=='>') { attr_ctnt[0]='\0'; return '>'; }

  /* content of attribute */
	ch=read_char();
	/* skip white_space */
	while ((isspace(ch)) && (ch!='>')) { ch=read_char(); }
	temp[0] = '\0';

	/* if quoted */
  if ((ch=='"') || (ch=='\''))
  {
		/* attribute looks like alt="bla" or alt='bla'. */
		/* we'll have to remember what the quote was. */
		int quote=ch;
    i=0;
    ch=read_char();
    while(quote!=ch) { temp[i++] = ch; ch=read_char(); }
    temp[i] = '\0';
    ch=read_char();
  }
  else
  {
		/* attribute looks like alt=bla */
    i=1;
    temp[0] = ch;
    while ((ch!='>') && (!isspace(ch))) { ch=read_char(); temp[i++] = ch; }
    temp[i-1] = '\0';
  }

	/*	printf("AttributInhalt: #%s#\n", temp); */
	uppercase_str(attr_name);
	/*printf(" %s %s\n", attr_name, temp); */
  if CMP("ALT", attr_name) { parse_entities(temp); }
  /*printf("AttributInhalt: %s#-#\n", temp); */
	CPYSS(attr_ctnt, temp);
	/*printf("Attribut: %s; Inhalt: %s#-#\n", attr_name, attr_ctnt); */

#ifdef attr_debug
  printf("Attribut: %s; Inhalt: %s#-#\n", attr_name, attr_ctnt);
#endif
#ifdef proc_debug
  printf("get_attr() ende\n");
#endif
  return ch;
} /* end get_attr */

/* ------------------------------------------------  */

void html()
{
	int i;
	CHAR str[DEF_STR_LEN];
  /*align[0]=LEFT; */

	for (i=0; i<DEF_STR_LEN; i++) { str[i]=0x00; }

  for (;;) {
    ch = read_char();
#ifdef default_debug
    printf("default: ch=%c ; %d\n",ch,ch);
#endif

    switch (ch) {
			case '<':
#ifdef debug
        printf("<: \n");
#endif
        html_tag();
        break;
				
			/* Entities  */
			case '&':
#ifdef debug
        printf("&: \n");
#endif
        i=1;
        str[0] = ch;
        do {
          ch = read_char();
          str[i++] = ch;
        }
        while ((isalnum(ch)) || (ch=='#'));

				/* if last char is no ';', then the string is no valid entity, maybe */
				/* it is something like &nbsp or even '& ' */
				if (ch!=';') { 
				  /* save last char  */
				  putback_char(ch); /*goback_char(1); */
				  /* no ';' at end */
				  str[i-1] = '\0'; }
				else {					
					/* valid entity */
					str[i] = '\0';
					/* strcpy(tmpstr, str); */
				}
#ifdef debug
        printf("check &: %s\n",str);
#endif
#ifdef DEBUG
				printf("-DEBUG-");
				for (i=0; i<10; i++) { printf("%d ", str[i]);}
				printf("-\n");
#endif
				parse_entity(&str[0]);
				wort_plus_string(str);
				#if 0
 				if ((entity_number(str)) || (latin1(str)) || (microsoft_entities(str))) {
					/* if true entity was known */
					wort_plus_string(str);
				}
        else {
					if (errorlevel>=1) { fprintf(stderr, "entity %s unknown\n",tmpstr); }
					wort_plus_string(tmpstr);
					/* FIXME MAYBE??? */
					/*goback_char(strlen(str)-2); */
					/*wort_plus_ch(str[0]); */
					/*wort_plus_ch(str[1]); */
				}
				#endif
#ifdef debug
        printf("&: %s\n",str);
#endif
        break;

			case 173: /* soft hyphen, just swallow it */
				break;

			case    9: /* TAB */
				if (pre) { 
					wort_plus_ch(0x09); 
				} else { 
					wort_ende(); 
				}
				break;

      case   13: /* CR */
      case '\n':
#ifdef debug
        printf("\\n, TAB or CR: \n");
#endif
        wort_ende();
        if (pre) { line_break(); }
        break;

      /* Microsoft... */
	case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: 
	case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d:	case 0x8e: case 0x8f:
	case 0x90: case 0x91: case 0x92: case 0x93: case 0x94: case 0x95: case 0x96: case 0x97:
	case 0x98: case 0x99: case 0x9a: case 0x9b: case 0x9c: case 0x9d: case 0x9e: case 0x9f:

	  if (convert_characters) { microsoft_character(ch); }
	  else wort_plus_ch(ch);
	  break;
		
      default:
#ifdef default_debug
        printf("default: ch=%c ; %d\n",ch,ch);
#endif
        if (pre==0) {
          if (ch==' ') { wort_ende(); }
          else { wort_plus_ch(ch); }
        }
        else { wort_plus_ch(ch); }
        break;
    }
  } /* next */
} /* end html */

/* ------------------------------------------------ */

/* used when there's only the align-attribut to be checked  */
void check_for_center()
{
	int found=0;
#ifdef proc_debug
  printf("check_for_center()\n");
#endif
  while (ch!='>')
  {
    ch=get_attr();
#ifdef debug
    printf(" Attr: %s; Inhalt: %s#\n",attr_name,attr_ctnt);
#endif
    if CMP("ALIGN", attr_name)
    {
			found=1;
      uppercase_str(attr_ctnt);
      if CMP("LEFT",   attr_ctnt) { push_align(LEFT);  }
      else if CMP("CENTER", attr_ctnt) { push_align(CENTER); }
      else if CMP("RIGHT",  attr_ctnt) { push_align(RIGHT); }
      else if CMP("JUSTIFY", attr_ctnt) { push_align(LEFT); }		
      else { if (errorlevel>=2) { fprintf(stderr, "Kein LEFT|CENTER|RIGHT gefunden!\n"); push_align(LEFT); } }
    }
  } 
  /* found no ALIGN  */
  if (found==0) { push_align(LEFT); }
	
#ifdef proc_debug
  printf("check_for_center() ende\n");
#endif
} /* end check_for_center */

/* ------------------------------------------------ */

void start_p()
{
#ifdef proc_debug
  printf("\nstart_p()\n");
#endif
  push_align(LEFT);
  neuer_paragraph();
  check_for_center();
#ifdef proc_debug
  printf("start_p() ende\n");
#endif
} /* end start_p */

/* ------------------------------------------------ */

void start_div(int a)
{
#ifdef proc_debug
  printf("start_div()\n"),
#endif
	/*  paragraphen_ende(); */

	line_break();

  if (a!=0) 	{ div_test=a; push_align(div_test); }
  else { check_for_center(); }
#ifdef proc_debug
  printf("start_div() ende\n");
#endif
} /* end start_div */

/* ------------------------------------------------ */

void end_div()
{
#ifdef proc_debug
  printf("end_div()\n"),
#endif
  wort_ende();

  if (paragraph!=0) { paragraphen_ende(); }
  else { print_zeile(); }
  pop_align(); /* einer für start_div */
  div_test = 0;
#ifdef proc_debug
  printf("end_div() ende\n");
#endif
} /* end_div */

/* ------------------------------------------------ */

void print_footnote_number(CHAR *temp, int number)
{	
#ifdef MULTIBYTE
	swprintf(temp, 1000, L"[%d]", number); 
#else
	snprintf(temp, 1000, "[%d]", number); 
#endif
}
void construct_footnote(CHAR *temp, int number, CHAR *link)
{
#ifdef MULTIBYTE
	swprintf(temp, 1000, L" %3d. %ls\n", number, link);
#else
	snprintf(temp, 1000, " %3d. %s\n", number, link); 
#endif
} /* end construct_footnote */

/* ------------------------------------------------ */

int references_count=0;
CHAR references[DEF_STR_LEN];
char *schemes[] = {"ftp://","file://" ,"http://" ,"gopher://" ,"mailto:" ,"news:" ,"nntp://" ,"telnet://" ,"wais://" ,"prospero://" };

/* handles <A href"..."></A> */
void href()
{  
	CHAR tmp[DEF_STR_LEN];

	if (!option_links) { return; }

	while (ch!='>')
  {
    ch=get_attr();
    if CMP("HREF", attr_name) {
			if ((STRSTR(attr_ctnt, "://")!=NULL) || (STRNCMP("mailto:", attr_ctnt, 7)==0) || (STRNCMP("news:", attr_ctnt, 5)==0)) {
				references_count++;

				parse_entities(attr_ctnt);
				print_footnote_number(tmp, references_count);
				wort_plus_string(tmp);

				construct_footnote(tmp, references_count, attr_ctnt);
				STRCAT(references, tmp);
			}
    }
  } 
} /* end href */

/* ------------------------------------------------ */

void href_output() 
{
	if (option_links) {
		if (references_count!=0) {				
			output_string(STRING("\n References:"));
			output_string(references);
		}
	}
} /* end href_output */

/* ------------------------------------------------ */

/* find alt attribute in current tag */
void image(CHAR *alt_text, int show_alt)
{
  int found_alt=0;
#ifdef proc_debug
  printf("image()\n");
#endif
#ifdef DEBUG
	printf("alt_text %ls alt %d, image %d, remove_empty_alt %d\n", alt_text, option_no_alt, option_no_image, remove_empty_alt);
#endif
  while (ch!='>')
  {
    ch=get_attr();
    if CMP("ALT", attr_name)
    {
			/*printf("+1+\n"); */
			if (!(remove_empty_alt && CMP("", attr_ctnt))) { 
				/*printf("+2+\n"); */
				if (!option_no_alt)
				{ wort_plus_ch('['); wort_plus_string(attr_ctnt); wort_plus_ch(']');}
			}
      found_alt=1;
    }
  }
  if ((found_alt==0) && (show_alt)) {
		if (!option_no_image)
		{	wort_plus_ch('['); wort_plus_string(alt_text); wort_plus_ch(']'); }
	}
#ifdef proc_debug
  printf("image() ende\n");
#endif
} /* end image */

/* ------------------------------------------------ */

/* find alt attribute in current tag */
void process_meta()
{
#ifdef MULTIBYTE
	int found_ctnt=0;
	CHAR *locale;
	char stripped_locale[DEF_STR_LEN];
#endif
#ifdef proc_debug
	printf("process_meta()\n");
#endif

	if (!processed_meta)
	{
		/* printf("in process_meta()\n"); // DEBUG */
		/*processed_meta=1; */
#ifdef MULTIBYTE
		while (ch!='>')
		{
			/*printf("vor get_attr()\n");  // DEBUG */
			ch=get_attr();
			/* printf("nach get_attr()\n"); // DEBUG */
			/* printf("%ls %ls\n", attr_name, attr_ctnt); // DEBUG */
			if ((CMP("HTTP-EQUIV", attr_name)) || (CMP("NAME", attr_name))) 
			{
				if CMP("Content-Type", attr_ctnt) { found_ctnt=1; }
			} else if CMP("CONTENT", attr_name)
			{
				if (found_ctnt)
				{
					/*printf("2.5\n"); // DEBUG */
					found_ctnt=0;
					/* search and set character set */
					locale = wcsstr(attr_ctnt, L"charset=");
					/*printf("3 locale %ls\n", locale); // DEBUG */
					if (locale!=NULL) {
						processed_meta=1;
						locale += 8;
						/*printf("3\n"); // DEBUG */
						/*printf("locale gefunden -%ls-  \n", locale); // DEBUG */
						/*printf("4\n"); // DEBUG */
						strip_wchar(locale, stripped_locale);
						/*printf("strip_wchar %s\n", stripped_locale); // DEBUG */
						/*set_locale(stripped_locale);				 */
						set_iconv_charset(stripped_locale);
						/*files_reopen(); // PROBABLY USELESS? */
					}
				}
			}
		}
#endif
	}
#ifdef proc_debug
  printf("process_meta ende\n");
#endif
} /* end process_meta */

/* ------------------------------------------------ */

/* simple finite state machine to eat up complete comment '!--' */
CHAR friss_kommentar()
{
	int c, dontquit=1;
#ifdef proc_debug
	printf("Frisskommentar start\n");
#endif
	while (dontquit) 
	{
		c=read_char();
		/*#ifdef debug */
		/*printf("%c", c); */
		/*#endif */
		if (c=='-')
		{
		  c=read_char();
		  /*#ifdef debug */
			/*printf("%c", c); */
			/*#endif */
			while (c=='-') 
		  {
			  c=read_char();
			  /*#ifdef debug */
				/*printf("%c", c); */
				/*#endif */
				if (c=='>') { dontquit=0; }
			}
		}
	}

	/*#ifdef debug */
	/*printf("\n"); */
	/*#endif */
	/*#ifdef proc_debug */
	/*printf("Frisskommentar ende ch %c %d\n", c, c); */
	/*#endif */
	return c;
} /* end friss_kommentar */

/* ------------------------------------------------ */
