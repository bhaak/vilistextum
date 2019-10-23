/*
 * Copyright (c) 1998-2019 Patric Mueller
 * bhaak@gmx.net
 * http://bhaak.dyndns.org/vilistextum/
 *
 * Released under the GNU GPL Version 2 - http://www.gnu.org/copyleft/gpl.html
 *
 * 04.09.01: added some more bullet_styles.
 * 15.03.04: lists generate less newlines
 *
 */

#include <stdio.h>
#include <string.h>

#include "html.h"
#include "main.h"
#include "text.h"

#include "debug.h"

CHAR bullet_style=' ';

/* ------------------------------------------------ */

#if 0
int check_style()
{
	while (ch!='>')
	{
		ch=get_attr();
#ifdef debug
		printf("checkstyle; attr: %s; Inhalt: %s#\n",attr_name,attr_ctnt);
#endif
		if CMP("TYPE", attr_name)
		{
			if CMP("disc", attr_ctnt)   { return '*'; }
			if CMP("square", attr_ctnt) { return '+'; }
			if CMP("circle", attr_ctnt) { return 'o'; }
		}
	}
	return 0;
} /* end check_style */
#endif

/* ------------------------------------------------ */

void start_uls()
{
	/*int ret; */
#ifdef proc_debug
	printf("start_uls()\n");
#endif
	line_break();

	push_align(LEFT);

	/* * o + # @ - = ~ $ % */
	if (bullet_style==' ') { bullet_style='*'; }
	else if (bullet_style=='*') { bullet_style='o'; }
	else if (bullet_style=='o') { bullet_style='+'; }
	else if (bullet_style=='+') { bullet_style='#'; }
	else if (bullet_style=='#') { bullet_style='@'; }
	else if (bullet_style=='@') { bullet_style='-'; }
	else if (bullet_style=='-') { bullet_style='='; }
	else if (bullet_style=='=') { bullet_style='~'; }
	else if (bullet_style=='~') { bullet_style='$'; }
	else if (bullet_style=='$') { bullet_style='%'; }

	if (option_latex) {
		line_break();
		wort_plus_string_escape(STRING("\\begin{itemize}"), FALSE);
		line_break();
	}

	spaces += tab;
#ifdef proc_debug
	printf("start_uls() ende\n");
#endif
} /* end start_uls */

void end_uls()
{
	spaces -= tab;
	line_break();

	if (bullet_style=='%') { bullet_style='$'; }
	else if (bullet_style=='$') { bullet_style='~'; }
	else if (bullet_style=='~') { bullet_style='='; }
	else if (bullet_style=='=') { bullet_style='-'; }
	else if (bullet_style=='-') { bullet_style='@'; }

	else if (bullet_style=='@') { bullet_style='#'; }
	else if (bullet_style=='#') { bullet_style='+'; }
	else if (bullet_style=='+') { bullet_style='o'; }
	else if (bullet_style=='o') { bullet_style='*'; }
	else if (bullet_style=='*') { bullet_style=' '; }

	if (option_latex) {
		wort_plus_string_escape(STRING("\\end{itemize}"), FALSE);
		line_break();
	}

	pop_align();
} /* end end_uls */

/* ------------------------------------------------ */

void start_ols()
{
	if (option_latex) {
		line_break();
		wort_plus_string_escape(STRING("\\begin{enumerate}"), FALSE);
		line_break();
		spaces += tab;
	} else {
		start_uls();
	}
} /* end start_ols */

/* ------------------------------------------------ */

void end_ols()
{
	if (option_latex) {
		line_break();
		spaces -= tab;
		wort_plus_string_escape(STRING("\\end{enumerate}"), FALSE);
		line_break();
	} else {
		end_uls();
	}
} /* end end_ols */

/* ------------------------------------------------ */

void start_lis()
{
	if (option_latex) {
		if (!is_zeile_empty()) { line_break(); }
		wort_plus_string_escape(STRING("\\item "), FALSE);
	} else {
		spaces-=2;

		/* don't output line break, if this list item is immediately
		   after a start or end list tag. start_uls and end_uls have
		   already take care of the line break */
		if (!is_zeile_empty()) { line_break(); }

		wort_plus_ch(bullet_style);

		wort_ende();
		spaces+=2;
	}
} /* end start_lis */

/* ------------------------------------------------ */

void end_lis() {
}

/* ------------------------------------------------ */

int definition_list=0;
void end_dd();

/* Definition List */
void start_dl()
{
	end_dd();
	start_p();
	if (option_latex) {
		line_break();
		wort_plus_string_escape(STRING("\\begin{description}"), FALSE);
	}
} /* end start_dl */

void end_dl()
{
	paragraphen_ende();

	end_dd();

	if (option_latex) {
		wort_plus_string_escape(STRING("\\end{description}"), FALSE);
	}
} /* end_dl */

/* Definition Title */
void start_dt()
{
	end_dd();

	line_break();
	if (option_latex) {
		wort_plus_string_escape(STRING("\\item["), FALSE);
	}
} /* end start_dt */

void end_dt()
{
	if (option_latex) {
		wort_plus_string_escape(STRING("]"), FALSE);
	}
} /* end_dt */

/* Definition Description */
void start_dd()
{
	end_dd();

	line_break();
	spaces+=tab;

	definition_list=1;
} /* end  */

void end_dd()
{
	if (definition_list==1)
	{
		spaces-=tab;
		definition_list=0;
	}
} /* end_dd */

