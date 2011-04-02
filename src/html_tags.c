/*
 * Copyright (c) 1998-2011 Patric Mueller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
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

/* ------------------------------------------------ */

void html_br() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\\\"), FALSE);
	}
	line_break();
} /* end html_br */

/* ------------------------------------------------ */

void html_b() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlB{"), FALSE);
	} else {
		wort_plus_ch('*');
	}
} /* html_b */

void html_b_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	} else {
		wort_plus_ch('*');
	}
} /* html_b_end */

/* ------------------------------------------------ */

void html_i() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlI{"), FALSE);
	} else {
		wort_plus_ch('/');
	}
} /* end html_i */

void html_i_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	} else {
		wort_plus_ch('/');
	}
} /* end html_i_end */

/* ------------------------------------------------ */

void html_u() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlU{"), FALSE);
	} else {
		wort_plus_ch('_');
	}
} /* html_u */

void html_u_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	} else {
		wort_plus_ch('_');
	}
} /* html_u_end */

/* ------------------------------------------------ */

void html_strong() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlStrong{"), FALSE);
	} else {
		wort_plus_ch('*');
	}
} /* html_strong */

void html_strong_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	} else {
		wort_plus_ch('*');
	}
} /* html_strong_end */

/* ------------------------------------------------ */

void html_em() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlEm{"), FALSE);
	} else {
		wort_plus_ch('/');
	}
} /* html_em */

void html_em_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	} else {
		wort_plus_ch('/');
	}
} /* html_em_end */

/* ------------------------------------------------ */

void html_hn(int n) {
	start_p();
	if (option_latex) {
		switch (n) {
		case 1: wort_plus_string_escape(STRING("\\htmlHeadingOne{"), FALSE); break;
		case 2: wort_plus_string_escape(STRING("\\htmlHeadingTwo{"), FALSE); break;
		case 3: wort_plus_string_escape(STRING("\\htmlHeadingThree{"), FALSE); break;
		case 4: wort_plus_string_escape(STRING("\\htmlHeadingFour{"), FALSE); break;
		case 5: wort_plus_string_escape(STRING("\\htmlHeadingFive{"), FALSE); break;
		case 6: wort_plus_string_escape(STRING("\\htmlHeadingSix{"), FALSE); break;
		}
	}
} /* html_hn */

void html_hn_end(int n) {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	}
	paragraphen_ende();
} /* html_hn_end */

/* ------------------------------------------------ */

void html_img(CHAR *src_location) {
	if (option_latex) {
    int len = STRLEN(src_location);
		wort_plus_string_escape(STRING("\\htmlImg{"), FALSE);
		/* remove TLA file extension */
		if ((len > 4) && (src_location[len-4] == '.')) {
		  src_location[len-4] = '\0';
		}
		wort_plus_string_escape(src_location, FALSE);
		wort_plus_string_escape(STRING("}"), FALSE);
	}
} /* html_img */

/* ------------------------------------------------ */

void wort_plus_html_color(CHAR* color_command, CHAR* htmlcolor) {
	wort_plus_string_escape(color_command, FALSE);
	wort_plus_string_escape(STRING("{"), FALSE);
	if (htmlcolor[0] == '#') {
  	uppercase_str(htmlcolor); /* #00aa00 -> #00AA00 */
		wort_plus_string(attr_ctnt); /* 00AA00 */
	} else {
		wort_plus_string(attr_ctnt); /* Red */
	}
	wort_plus_string_escape(STRING("}"), FALSE);
} /* output_html_color */

/* ------------------------------------------------ */

int html_font_opened = 0;
void html_font()
{
#ifdef proc_debug
  printf("html_font()\n");
#endif
	if (option_latex) {
    while (ch!='>') {
      ch=get_attr();

      if CMP("COLOR", attr_name) {
				wort_plus_html_color(STRING("\\htmlColor"), attr_ctnt);
				wort_plus_string_escape(STRING("{"), FALSE);
        html_font_opened++;
      }
    }
	}

#ifdef proc_debug
  printf("html_font() ende\n");
#endif
} /* html_font */

void html_font_end()
{
	if (option_latex) {
	  if (html_font_opened > 0) {
		  wort_plus_string_escape(STRING("}"), FALSE);
	    html_font_opened--;
		}
	}

} /* html_font_end  */

/* ------------------------------------------------ */

void html_body()
{
#ifdef proc_debug
  printf("html_body()\n");
#endif
	if (option_latex) {
    while (ch!='>') {
      ch=get_attr();

      if CMP("TEXT", attr_name) {
				wort_plus_html_color(STRING("\\fgColor"), attr_ctnt);
      }
      if CMP("BGCOLOR", attr_name) {
				wort_plus_html_color(STRING("\\bgColor"), attr_ctnt);
      }
    }
	}

#ifdef proc_debug
  printf("html_body() ende\n");
#endif
} /* html_font */

/* ------------------------------------------------ */

void html_pre() {
	start_p();
	pre=TRUE;
	if (option_latex) {
		wort_plus_string_escape(STRING("\\begin{htmlPre}"), FALSE);
	}
} /* html_pre */

void html_pre_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\end{htmlPre}"), FALSE);
	}
	paragraphen_ende();
	pre=FALSE;
} /* html_pre_end */

/* ------------------------------------------------ */

void html_blockquote() {
	if (option_latex) {
		line_break();
		wort_plus_string_escape(STRING("\\begin{htmlBlockquote}"), FALSE);
		line_break();
	} else {
	  start_p();
	}
} /* html_pre */

void html_blockquote_end() {
	if (option_latex) {
		line_break();
		wort_plus_string_escape(STRING("\\end{htmlBlockquote}"), FALSE);
		line_break();
	} else {
	  paragraphen_ende();
	}
} /* html_pre_end */

/* ------------------------------------------------ */

void html_code() {
	if (option_latex) {
		wort_plus_string_escape(STRING("\\htmlCode{"), FALSE);
	}
} /* html_code */

void html_code_end() {
	if (option_latex) {
		wort_plus_string_escape(STRING("}"), FALSE);
	}
} /* html_code_end */

/* ------------------------------------------------ */
