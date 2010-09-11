/*
 * Copyright (c) 1998-2010 Patric Müller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 *
 * history:
 * v0.1 ??.??.???? : lost due Harddisk-Crash
 * v1.x 26.04.1998 : variable width,
 *                   italic, underline, bold
 *                   HR implemented
 * v2.x 13.06.98   : UL, LI implemented
 *      21.06.98   : modified interface
 *      25.06.98   : fixed problem with paragraph-breaking & SGML entities
 *      27.06.98   : LI fixed, implemented IMAGE
 *      28.06.98   : HR: WIDTH, ALIGN,
 *                   DL, DT, DD,
 *                   PRE, TABLE (sort of),
 *                   EM, STRONG implemented
 *      15.07.98   : $91,$92,$93,$94 --> `,',","
 *      26.07.98   : fixed a bug with
 *                   get_attr(): got confused over empty attributs
 *                               <HR NOSHADE>
 *                   paragraph handling: got screwed up if there were
 *                                       only <p>s, but not </p>s
 *      28.07.98   : added "& " for those who don't know &amp;
 *      18.09.98   : added CODE
 *                   added all &xxx;
 *                   added "&#38;"  '&', "&#62;"  '<', "&#60;"  '>'
 *      02.02.99   : added "&#160;" ' ', "&#169;" '©', "&#180;" '`',
 *                         "&#228;" 'ä', "&#252;" 'ü', "&#246;" 'ö',
 *                         "&#196;" 'Ä', "&#220;" 'Ü', "&#214;" 'Ö',
 *                         "&#223;" 'ss',
 *                   added RIGHT, /RIGHT,
 *                   added all "&#xxx;",
 *                   AREA, /AREA, MAP, /MAP,
 *                   !--
 *      27.02.99   : better implementation of BLOCKQUOTE
 *      09.03.99   : fixed bug which read wrong breite
 *
 *      01.09.00   : added support for palm, added &#145-152
 *      30.10.00   : added "&#34;"
 *      10.11.00   : bad bug, Remember Character!=String,
 *                   convert B,I,U to ASCII-Characters,
 *                   ignoring now SCRIPT and SUP
 *      30.01.01   : added all special microsoft ascii char between 127-160,
 *                   no more warnings with wall
 *      14.02.01   : better command parsing (still no getopt)
 *                   added microsoft entities
 *      20.02.01   : unknown &#xxx; will be printed as text
 *                   microsoft_entities broke latin1 recognition (how appropriate)
 *                   errorlevel introduced
 *      20.02.01   : SCRIPT was not implemented
 *                   Default Image text editable
 *                   Comment !-- was buggy, stopped at just a >
 *                   get_attr eats whitespace now
 *      22.02.01   : added --remove-empty-alt
 *                   OL are not numbered but get a "-"
 *      26.02.01   : broken entities (&nbsp) are recognized
 *                   removed a small table bug
 *                   no longer gets confused about something like <</b>
 *      09.03.01   : getopt implemented
 *                   added some xhtml like empty tags <br/> <hr/>,
 *                   whitespace error in get_attr fixed
 *                   html 3.2 spec misunderstood, div needs linebreak
 *      08.04.01   : fixed friss_kommentar
 *      09.04.01   : first public release
 *
 *      19.04.01   : --version
 *      20.04.01   : stupid, error messages should go to stderr
 *      23.04.01   : links are shown as references ala lynx
 *                   notitle added
 *                   something broke gnu getopt_long if used with short options and arguments
 *      03.09.01   : added hexadecimal entities
 *      04.09.01   : Fixed --shrink-lines=n
 *      15.01.02   : Fixed unicode implementation, no named entities with unicode at the moment
 *      31.01.02   : Fixed source code to run again on solaris
 *      27.02.02   : Fixed stdin/stdout for non-linux systems
 *
 * TO DO
 * -----
 * fix OL vs UL
 * CENTER and table don't get along well
 *
 */

/* Solaris needs this to activate getopt */
#if defined (__SVR4) && defined (__sun)
 #define _XOPEN_SOURCE
 #define _XOPEN_VERSION 4
#endif

#include <../config.h>

#include <unistd.h>
#include <string.h>

#ifdef HAVE_GETOPT_H
 #define _GNU_SOURCE
 #include <getopt.h>
#endif

#include <stdio.h>
#include <stdlib.h>

#include "html.h"
#include "fileio.h"
#include "charset.h"
#include "debug.h"

/* commandline options */
int palm = 0,              /* if true, enable pda specific restrictions    */
  convert_tags = 0,        /* if true, convert some tags (eg B,I) to ASCII characters */
  errorlevel = 1,          /* 0, no errormessages; 1, missing entities; 2, missing tags */
  convert_characters = 1,  /* if true,  convert 0x128-0x159 to latin1-characters */
  shrink_lines = 0,        /* no more than 2 empty lines */
  option_links = 0,        /* show links at end of output */
  option_links_inline = 0, /* show links after tag */
  option_title = 1,        /* true -> show title tag, else dont */

  remove_empty_alt = 0,   /* dont show [] for <IMG ALT="">  */
	option_no_image = 0,    /* don't show [Image] */
	option_no_alt   = 0,    /* don't show [alt text] for <IMG ALT="alt text" */

	option_latex = 0,    /* LaTeX output, experimental */
#ifdef MULTIBYTE
	option_output_utf8 = 0, /* true -> output everything in utf8 */
  sevenbit = 1,           /* true -> don't touch any character >127  */
  transliteration = 0;    /* use //TRANSLIT with libiconv */
#else
  sevenbit = 0;           /* true -> don't touch any character >127  */
#endif

CHAR *default_image=STRING("Image"); /* Default string for IMG without ALT-tag */
CHAR user_image[DEF_STR_LEN]; /* string supplied by user */

char help_text[] = 
"Usage: vilistextum [OPTIONS] [inputfile|-] [outputfile|-]\n"
"\n"
"OPTIONS:\n"
"  -a, --no-alt                  \n"
"  -c, --convert-tags            \n"
"  -e, --errorlevel NUMBER       \n"
"  -i, --defimage STRING         \n"
"  -l, --links                   \n"
"  -k, --links-inline            \n"
"  -m, --dont-convert-characters \n"
"  -n, --no-image                \n"
"  -p, --palm                    \n"
"  -r, --remove-empty-alt        \n"
"  -s, --shrink-lines [NUMBER]   \n"
"  -t, --no-title                \n"
"  -w, --width NUMBER            \n"
#ifdef MULTIBYTE
"\n"
"  -u, --output-utf-8            \n"
"  -x, --translit                \n"
"  -y, --charset CHARSET         \n"
#endif
"\n"
"  -h, --help           \n" /*        display this help and exit\n" */
"  -v, --version        \n" /*         output version information and exit\n" */
"Please report bugs to <bhaak@gmx.net>\n"
;

void help()
{
	printf("%s", help_text);

	exit(0);
} /* end help */

/* ------------------------------------------------ */

void parse_args(int argc, char *argv[])
{
  int c=0;
	char *argument="";

  while (1)
  {
#ifdef HAVE_GETOPT_H
		int option_index = 0;
		static struct option long_options[] =
		{
			{"palm",  0, 0, 'p'},
			{"dont-convert-characters", 0, 0, 'm'},
			{"shrink-lines", 2, 0, 's'},
			{"defimage", 1, 0, 'i'},
			{"width" , 1, 0, 'w'},
			{"convert-tags", 0, 0,'c'},
			{"errorlevel", 1, 0, 'e'},
			{"help", 0, 0, 'h'},
			{"version", 0, 0, 'v'},
			{"links", 0, 0, 'l'},
			{"links-inline", 0, 0, 'k'},
			{"no-title", 0, 0, 't'},

			{"remove-empty-alt", 0, 0, 'r'},
			{"no-image", 0, 0, 'n'},
			{"no-alt", 0, 0, 'a'},

			{"charset", 1, 0, 'y'},
			{"translit", 0, 0, 'x'},
			{"output-utf-8", 0, 0, 'u'},

			{"latex", 0, 0, 'L'},

			{0, 0, 0, 0}
		};
#endif

#ifdef DEBUG
		int i=0;
		for (i=0; i<argc; i++)
		{
			fprintf(stderr, "%s ", argv[i]);
		}
		printf("\n");
#endif
		
#ifdef  HAVE_GETOPT_H
		c = getopt_long (argc, argv, "pmsi:ce:hklLtrnavy:xuw:", long_options, &option_index);
#else
		c = getopt(argc, argv,       "pmsi:ce:hklLtrnavy:xuw:");
#endif
 #ifdef DEBUG
		fprintf(stderr, "c: %d c: %c \n\n", c,c); 
 #endif

		/* get argument */
		if (optarg==NULL) { argument = argv[optind]; }
		else { argument = optarg; } 

		if ((c == -1) && (c = '-')) break;

		switch (c)
		{
		case 'v':
#ifdef MULTIBYTE
				printf("Vilistextum %s (%s) with multibyte support\n", VERSION, RELEASEDATE);
#else
				printf("Vilistextum %s (%s)\n", VERSION, RELEASEDATE);
#endif
				exit(0);
#ifdef MULTIBYTE
		case 'y': set_iconv_charset(argument); break;
		case 'x': transliteration = 1; break;
		case 'u': option_output_utf8 = 1; break;
#endif
			break;
		case 'p':
		  /* settings for PDAs */
		  hr_breite = 30;
		  breite = DEF_STR_LEN-1; 
		  palm = 1;
			break;
		case 'w': 
		  breite = atoi(argument);
		  hr_breite = breite;
		  if (breite>=DEF_STR_LEN) { fprintf(stderr, "max(breite)=%d\n", DEF_STR_LEN); breite=DEF_STR_LEN-1; }
		  break;
		case 'm': convert_characters = 0; break;
		case 's': 
			shrink_lines = atoi(argument); if (shrink_lines==0) { shrink_lines = 1; } break;

		case 'r': remove_empty_alt = 1; break;
		case 'i': 
#ifdef MULTIBYTE
			convert_string(argument, user_image);
			default_image = user_image; break;
#else
			default_image = argument; break;
#endif
		case 'n': option_no_image = 1; break;
		case 'a': option_no_image = 1; option_no_alt = 1; break; 

		case 'c': convert_tags = 1; break;
		case 'e': errorlevel = atoi(argument); break;
		case 'h': help(); break;
		case 'l': option_links = 1; option_links_inline = 0; break;
		case 'k': option_links_inline = 1; option_links = 0; break;
		case 't': option_title = 0; break;

		case 'L': option_latex = 1; break;

		case '?': /* unknown option */
		  help();
		  break;
					
		default:
			fprintf(stderr, "?? getopt returned character code 0%o %c??\n", c, c);
		}
	}

#ifdef DEBUG                                                   
	if (optind < argc)
    {
	  if (argc-optind!=2) { help(); }
	  fprintf (stderr, "non-option ARGV-elements: ");
	  while (optind < argc)
		fprintf (stderr, "%s ", argv[optind++]);
	  fprintf (stderr, "\n");
	}
#endif
}                                

/* ------------------------------------------------ */

int main(int argc, char *argv[])
{    
#ifdef MULTIBYTE
	init_multibyte();
	use_default_charset();	
#endif
  parse_args(argc, argv);

  if (argc < 3) {	help(); }

  open_files(argv[argc-2],argv[argc-1]);

  html();

  quit();
  return(0);
} /* end main */
