/*
 * Copyright (c) 1998-2011 Patric Mueller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 * 23.04.01 : Ignoring SPAN, /SPAN and /LI
 *            IMG, APPLET, AREA and INPUT are searched for ALT attribute 
 * 13.08.01 : Ignoring DFN and /DFN
 * 24.08.01 : Fixed Frisskommentar
 * 02.09.01 : Ignoring BLINK, /BLINK, CITE and /CITE
 * 10.04.02 : Ignoring NOBR, /NOBR, SELECT, /SELECT, OPTION
 * 17.12.04 : html tags longer than DEF_STR_LEN are truncated
 *
 */

#include "multibyte.h"

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "html.h"
#include "text.h"
#include "main.h"
#include "lists.h"
#include "fileio.h"
#include "debug.h"
#include "charset.h"
#include "util.h"
#include "html_tags.h"

void html_tag()
{
  CHAR str[DEF_STR_LEN];
  int i=0;

  ch = uppercase(read_char());

	/* letter -> normal tag */
	/* '!' -> CDATA section or comment */
	/* '/' -> end tag */ 
	/* '?' -> XML processing instruction */
  if ((!isalpha(ch)) && (ch!='/') && (ch!='!') && (ch!='?'))
	{
    wort_plus_ch('<');
		putback_char(ch);
		/* fprintf(stderr, "no html tag: %c\n",ch); */
    return;
  }

	/* read html tag */
  while ((ch!='>') && (ch!=' ') && (ch!=13) && (ch!=10))
  {
    if (i<DEF_STR_LEN-1) { str[i++] = ch; }
    ch = uppercase(read_char());
  } /* post cond: i<=DEF_STR_LEN-1 */
	str[i] = '\0';

#ifdef debug
  fprintf(stderr, "html_tag: %ls\n",str);
  status();
#endif

	/* first all tags, that affect if there is any output at all */
  if CMP("SCRIPT", str)       { start_nooutput(); }
  else if CMP("/SCRIPT", str) { end_nooutput(); }
  else if CMP("STYLE", str)   { start_nooutput(); }
  else if CMP("/STYLE", str)  { end_nooutput(); }
  else if CMP("TITLE", str) { 
		if (option_latex) { neuer_paragraph(); wort_plus_string_escape(STRING("\\title{"), FALSE); } 
		else if (option_title) { push_align(LEFT); neuer_paragraph(); } 
		else { wort_ende(); print_zeile(); nooutput = 1; }
	} else if CMP("/TITLE", str) {
		if (option_latex) { wort_plus_string_escape(STRING("}"), FALSE); paragraphen_ende(); print_zeile(); } 
		else if (option_title) { paragraphen_ende(); print_zeile(); } 
		else { wort_ende(); clear_line(); print_zeile(); nooutput = 0; }
	}

	if (nooutput==0) {
		if CMP("/HTML", str) { /* fprintf(stderr, "File ended!\n"); */ quit(); }
		else if CMP("!DOCTYPE", str)  { while ((ch=read_char())!='>'); }
		else if CMP("META", str)      { find_encoding(); }
		else if CMP("?XML", str)      { find_xml_encoding(); }

		/* Linebreak */
		else if CMP("BR", str)  { html_br(); }
		else if CMP("BR/", str) { html_br(); } /* xhtml */

		else if CMP("P", str)  { start_p(); }
		else if CMP("/P", str) { paragraphen_ende(); }
		else if CMP("BLOCKQUOTE", str)  { html_blockquote(); }
		else if CMP("/BLOCKQUOTE", str) { html_blockquote_end(); }
		else if CMP("Q", str)  { wort_plus_ch('"'); }
		else if CMP("/Q", str) { wort_plus_ch('"'); }

		/* Convert these Tags */
#ifdef AMIGA
		else if CMP("B", str)       { wort_plus_string_nocount("\033[1m");  }
		else if CMP("/B", str)      { wort_plus_string_nocount("\033[22m"); }
		else if CMP("I", str)       { wort_plus_string_nocount("\033[3m");  }
		else if CMP("/I", str)      { wort_plus_string_nocount("\033[23m"); }
		else if CMP("U", str)       { wort_plus_string_nocount("\033[4m");  } /* deprecated */
		else if CMP("/U", str)      { wort_plus_string_nocount("\033[24m"); } /* deprecated */
		else if CMP("STRONG", str)  { wort_plus_string_nocount("\033[1m");  }
		else if CMP("/STRONG", str) { wort_plus_string_nocount("\033[22m"); }
		else if CMP("EM", str)      { wort_plus_string_nocount("\033[3m");  }
		else if CMP("/EM", str)     { wort_plus_string_nocount("\033[23m"); }
#else
		else if CMP("B", str)       { if (convert_tags) { html_b(); } }
		else if CMP("/B", str)      { if (convert_tags) { html_b_end(); } }
		else if CMP("I", str)       { if (convert_tags) { html_i(); } }
		else if CMP("/I", str)      { if (convert_tags) { html_i_end(); } }
		else if CMP("U", str)       { if (convert_tags) { html_u(); } } /* deprecated */
		else if CMP("/U", str)      { if (convert_tags) { html_u_end(); } } /* deprecated */
		else if CMP("STRONG", str)  { if (convert_tags) { html_strong(); } }
		else if CMP("/STRONG", str) { if (convert_tags) { html_strong_end(); } }
		else if CMP("EM", str)      { if (convert_tags) { html_em(); } }
		else if CMP("/EM", str)     { if (convert_tags) { html_em_end(); } }
		else if CMP("EMPH", str)    { if (convert_tags) { html_em(); } } /* sometimes used, but doesn't really exist */
		else if CMP("/EMPH", str)   { if (convert_tags) { html_em_end(); } } /* sometimes used, but doesn't really exist */
#endif


		/* headings */
		else if CMP("H1", str)  { html_hn(1);     }
		else if CMP("/H1", str) { html_hn_end(1); }
		else if CMP("H2", str)  { html_hn(2);     }
		else if CMP("/H2", str) { html_hn_end(2); }
		else if CMP("H3", str)  { html_hn(3);     }
		else if CMP("/H3", str) { html_hn_end(3); }
		else if CMP("H4", str)  { html_hn(4);     }
		else if CMP("/H4", str) { html_hn_end(4); }
		else if CMP("H5", str)  { html_hn(5);     }
		else if CMP("/H5", str) { html_hn_end(5); }
		else if CMP("H6", str)  { html_hn(6);     }
		else if CMP("/H6", str) { html_hn_end(6); }

		else if CMP("HR", str)  { hr(); }
		else if CMP("HR/", str) { hr(); } /* xhtml */

		else if CMP("A", str)   { href(); }
		else if CMP("/A", str)  { href_link_inline_output(); }

		else if CMP("LI", str)    { start_lis(); }
		else if CMP("/LI", str)   { end_lis(); }
		else if CMP("UL", str)    { start_uls(); }
		else if CMP("/UL", str)   { end_uls(); return; }
		else if CMP("DIR", str)   { start_uls(); }       /* deprecated */
		else if CMP("/DIR", str)  { end_uls(); return; } /* deprecated */
		else if CMP("MENU", str)  { start_uls(); }       /* deprecated */
		else if CMP("/MENU", str) { end_uls(); return; } /* deprecated */
		else if CMP("OL", str)    { start_ols(); }
		else if CMP("/OL", str)   { end_ols(); }

		else if CMP("DIV", str)      { start_div(0); }
		else if CMP("/DIV", str)     { end_div(); }
		else if CMP("CENTER", str)   { start_div(CENTER); } /* deprecated */
		else if CMP("/CENTER", str)  { end_div(); }         /* deprecated */
		else if CMP("RIGHT", str)    { start_div(RIGHT); }
		else if CMP("/RIGHT", str)   { end_div(); }

		/* tags with alt attribute */
		else if CMP("IMG", str)    { image(default_image, 1); }
		else if CMP("APPLET", str) { image(STRING("Applet"), 1); } /* deprecated */
		else if CMP("AREA", str)   { image(STRING("Area"), 0); }
		else if CMP("INPUT", str)  { image(STRING("Input"), 0); }

		/* table */
		else if CMP("TABLE", str)    { /*start_p();*/ push_align(LEFT); neuer_paragraph(); }
		else if CMP("/TABLE", str)   { paragraphen_ende(); }
		else if CMP("TD", str)       { wort_plus_ch(' '); }
		else if CMP("/TD", str)      {}
		else if CMP("TH", str)       { wort_plus_ch(' '); }
		else if CMP("/TH", str)      {}
		else if CMP("TR", str)       { line_break(); } /* start_p();  */
		else if CMP("/TR", str)      { /*paragraphen_ende();*/ }
		else if CMP("CAPTION", str)  {}
		else if CMP("/CAPTION", str) {}

		else if CMP("PRE", str)   { html_pre(); }
		else if CMP("/PRE", str)  { html_pre_end(); }

		else if CMP("DL", str)  { start_dl();} /* Definition List */
		else if CMP("/DL", str) { end_dl(); }
		else if CMP("DT", str)  { start_dt(); } /* Definition Title */
		else if CMP("/DT", str) { end_dt(); }
		else if CMP("DD", str)  { start_dd(); } /* Definition Description */
		else if CMP("/DD", str) { end_dd(); }

		/* tags for forms */
		else if CMP("FORM", str)      {}
		else if CMP("/FORM", str)     {}
		else if CMP("BUTTON", str)    {} /* TODO: extract name? */
		else if CMP("/BUTTON", str)   {}
		else if CMP("FIELDSET", str)  {}
		else if CMP("/FIELDSET", str) {}
		else if CMP("TEXTAREA", str)  {}
		else if CMP("/TEXTAREA", str) {}
		else if CMP("LEGEND", str)    {}
		else if CMP("/LEGEND", str)   {}
		else if CMP("LABEL", str)     {}
		else if CMP("/LABEL", str)    {}

		/* tags that have no visible effect */
		else if CMP("SAMP", str)      {}
		else if CMP("/SAMP", str)     {}
		else if CMP("CODE", str)      { html_code(); }
		else if CMP("/CODE", str)     { html_code_end(); }
		else if CMP("ABBR", str)      {}
		else if CMP("/ABBR", str)     {}
		else if CMP("ACRONYM", str)      {}
		else if CMP("/ACRONYM", str)     {}
		else if CMP("BIG", str)      {}
		else if CMP("/BIG", str)     {}
		else if CMP("VAR", str)      {}
		else if CMP("/VAR", str)     {}
		else if CMP("KBD", str)      {}
		else if CMP("/KBD", str)     {}

		/* tags that should have some visible effect */
		else if CMP("BDO", str)      {}
		else if CMP("/BDO", str)     {}
		else if CMP("INS", str)      {}
		else if CMP("/INS", str)     {}
		else if CMP("DEL", str)      {}
		else if CMP("/DEL", str)     {}
		else if CMP("S", str)         {} /* deprecated */
		else if CMP("/S", str)        {} /* deprecated */
		else if CMP("STRIKE", str)    {} /* deprecated */
		else if CMP("/STRIKE", str)   {} /* deprecated */

		else if CMP("FONT", str)      { html_font(); } /* deprecated */
		else if CMP("/FONT", str)     { html_font_end();} /* deprecated */
		else if CMP("BODY", str)      { html_body(); }
		else if CMP("/BODY", str)     {}

		/* those tags are ignored */
		else if CMP("HTML", str)      {}
		else if CMP("BASE", str)      {}
		else if CMP("/BASE", str)     {}
		else if CMP("LINK", str)      {}
		else if CMP("BASEFONT", str)  {} /* deprecated */

		else if CMP("HEAD", str)      {}
		else if CMP("/HEAD", str)     {}
		else if CMP("MAP", str)       {}
		else if CMP("/MAP", str)      {}
		else if CMP("SUP", str)       {}
		else if CMP("/SUP", str)      {}
		else if CMP("ADDRESS", str)   {}
		else if CMP("/ADDRESS", str)  {}
		else if CMP("TT", str)        {}
		else if CMP("/TT", str)       {}
		else if CMP("SUB", str)       {}
		else if CMP("/SUB", str)      {}
		else if CMP("NOSCRIPT", str)  {}
		else if CMP("/NOSCRIPT", str) {}
		else if CMP("SMALL", str)     {}
		else if CMP("/SMALL", str)    {}
		else if CMP("SPAN", str)      {}
		else if CMP("/SPAN", str)     {}
		else if CMP("DFN", str)       {}
		else if CMP("/DFN", str)      {}
		else if CMP("BLINK", str)     {}
		else if CMP("/BLINK", str)    {}
		else if CMP("CITE", str)      {}
		else if CMP("/CITE", str)     {}

		else if CMP("NOBR", str)      {}
		else if CMP("/NOBR", str)     {}
		else if CMP("SELECT", str)    {}
		else if CMP("/SELECT", str)   {}
		else if CMP("OPTION", str)    {}
		else if CMP("/OPTION", str)   {}

		else if CMP("FRAME", str)  {}
		else if CMP("/FRAME", str) {}
		else if CMP("FRAMESET", str)  {}
		else if CMP("/FRAMESET", str) {}
		else if CMP("NOFRAMES", str)  {}
		else if CMP("/NOFRAMES", str) {}
		else if CMP("IFRAME", str)    {}
		else if CMP("/IFRAME", str)   {}
		else if CMP("LAYER", str)     {}
		else if CMP("/LAYER", str)    {}
		else if CMP("ILAYER", str)    {}
		else if CMP("/ILAYER", str)   {}
		else if CMP("NOLAYER", str)   {}
		else if CMP("/NOLAYER", str)  {}

		else if CMP("COL", str)       {}
		else if CMP("COLGROUP", str)  {}
		else if CMP("/COLGROUP", str) {}
		else if CMP("ISINDEX", str)   {} /* deprecated */
		else if CMP("THEAD", str)     {}
		else if CMP("/THEAD", str)    {}
		else if CMP("TFOOT", str)     {}
		else if CMP("/TFOOT", str)    {}
		else if CMP("TBODY", str)     {}
		else if CMP("/TBODY", str)    {}
		else if CMP("PARAM", str)     {}
		else if CMP("/PARAM", str)    {} 
		else if CMP("OBJECT", str)    {}
		else if CMP("/OBJECT", str)   {}
		else if CMP("OPTGROUP", str)  {}
		else if CMP("/OPTGROUP", str) {}

		else if CMP("/AREA", str)     {}
		else if CMP("/IMG", str)      {}

		else if (STRNCMP("!--", str, 3)==0)  { 
			/*#ifdef debug */
			/*printf("Frisskommentar %ls\n", str); */
			/*#endif  */
			/*goback_char(3); */
			/* put the last 3 characters back to stream */
			/* needed for comments like "<--test-->" */
			putback_char(ch);
			putback_char(str[STRLEN(str)-1]);
			putback_char(str[STRLEN(str)-2]);
			/*printf("str: %ls\n", str); */
			/*printf(" put back %lc%lc%lc\n", str[STRLEN(str)-2], str[STRLEN(str)-1], ch); */
			ch = friss_kommentar(); 
			/*printf("ch_1: %c %d\n", ch, ch); */
			/*goback_char(1); */
			/*printf("ch_2: %c %d\n", ch, ch); */
			/*ch = read_char(); */
			/*printf("ch_3: %c %d\n", ch, ch); */
		} /* Comment */

		/* these have to be ignored, to avoid the following error to show up */
		else if CMP("SCRIPT", str)    {}
		else if CMP("/SCRIPT", str)   {}
		else if CMP("STYLE", str)     {}
		else if CMP("/STYLE", str)    {}
		else if CMP("TITLE", str)     {}
		else if CMP("/TITLE", str)    {}

		/* Tags used by heise.de */
		else if CMP("HEISETEXT", str)    {}
		else if CMP("/HEISETEXT", str)    {}
		else if CMP("HEISEADVERT", str)  {}
		else if CMP("/HEISEADVERT", str) {}
		else if CMP("CONTENT_AD_POSSIBLE", str) {}
		else if CMP("TPXAZ", str)    {}
		else if CMP("/TPXAZ", str)    {}
		else if CMP("TPXAZXTEXT", str)    {}
		else if CMP("/TPXAZXTEXT", str)    {}
		else if CMP("TPXB", str)    {}
		else if CMP("/TPXB", str)    {}
		else if CMP("TPXBUT", str)    {}
		else if CMP("/TPXBUT", str)    {}
		else if CMP("TPXBXIMG", str)    {}
		else if CMP("/TPXBXIMG", str)    {}
		else if CMP("TPXBXTEXT", str)    {}
		else if CMP("/TPXBXTEXT", str)    {}
		else if CMP("TPXLINK", str)    {}
		else if CMP("/TPXLINK", str)    {}
		else if CMP("TPXLINKXICON", str)    {}
		else if CMP("/TPXLINKXICON", str)    {}
		else if CMP("TPXLINKXTEXT", str)    {}
		else if CMP("/TPXLINKXTEXT", str)    {}

		/* new HTML5 elements */
		else if CMP("HEADER", str)   { start_div(0); }
		else if CMP("/HEADER", str)  { end_div(); }
		else if CMP("ARTICLE", str)  { start_div(0); }
		else if CMP("/ARTICLE", str) { end_div(); }
		else if CMP("FOOTER", str)   { start_div(0); }
		else if CMP("/FOOTER", str)  { end_div(); }
		else if CMP("HGROUP", str)   {}
		else if CMP("/HGROUP", str)  {}
		else if CMP("SECTION", str)  {}
		else if CMP("/SECTION", str) {}
		else if CMP("FIGURE", str)   { start_div(0); }
		else if CMP("/FIGURE", str)  { end_div(); }
		else if CMP("FIGCAPTION", str)   { start_div(0); }
		else if CMP("/FIGCAPTION", str)  { end_div(); }
		else if CMP("ASIDE", str)    { start_div(0); }
		else if CMP("/ASIDE", str)   { end_div(); }
		/* HTML5 media tags */
		else if CMP("AUDIO", str)    {}
		else if CMP("/AUDIO", str)   {}
		else if CMP("CANVAS", str)   {}
		else if CMP("/CANVAS", str)  {}
		else if CMP("EMBED", str)    {}
		else if CMP("/EMBED", str)   {}
		else if CMP("SOURCE", str)   {}
		else if CMP("/SOURCE", str)  {}
		else if CMP("VIDEO", str)    {}
		else if CMP("/VIDEO", str)   {}
		else if CMP("COMMAND", str)  {}
		else if CMP("/COMMAND", str) {}
		else if CMP("METER", str)    {}
		else if CMP("/METER", str)   {}
		else if CMP("PROGRESS", str)  {}
		else if CMP("/PROGRESS", str) {}
		else if CMP("TIME", str)     {}
		else if CMP("/TIME", str)    {}

		/* else if CMP("WBR", str)    { wort_ende(); } TODO */
		else { if (errorlevel>=2) { print_error("tag ignored: ", str);} } 
	}

  /* Skip attributes */
#ifdef debug
	printf("\nTag:     %ls;\n", str); 
#endif
  while (ch!='>')
  {
    ch = get_attr();
		
#ifdef debug
		printf("Attribute: %ls;\n", attr_name); 
		printf("Content:   %ls#\n", attr_ctnt);
#endif
  }
} /* end html_tag */
