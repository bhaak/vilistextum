/*
 * Copyright (C) 1998-2004 Patric M�ller
 * bhaak@gmx.net
 * http://www.mysunrise.ch/users/bhaak/vilistextum/
 *
 * Released under the GNU GPL - http://www.gnu.org/copyleft/gpl.html
 * 
 * 04.09.01: added some more bullet_styles.
 *
 */

#include <stdio.h>
#include <string.h>

#include "html.h"
#include "text.h"

#include "debug.h"

CHAR bullet_style=' ';

/* ------------------------------------------------ */

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

/* ------------------------------------------------ */

void start_uls()
{
  /*int ret; */
#ifdef proc_debug
  printf("start_uls()\n");
#endif

  push_align(LEFT);
/*  ret=check_style();
  if (ret==0)
  {*/
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

/*  } else {
    bullet_style=ret;
  }*/
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

  pop_align();
} /* end end_uls */

/* ------------------------------------------------ */

void start_ols()
{
	start_uls();
	/*	spaces+=2; orderedlist++; */
} /* end start_ols */

/* ------------------------------------------------ */

void end_ols()
{
	end_uls();
	/*	spaces-=2; orderedlist--; */
} /* end end_ols */

/* ------------------------------------------------ */

void start_lis()
{
  spaces-=2;
  line_break();
  
	/*wort_plus_ch(bullet_style); */

	/*if (orderedlist>0) { wort_plus_string("-"); } */
	/*else  {  */
	wort_plus_ch(bullet_style);
	/* } */

  wort_ende();
  spaces+=2;
} /* end start_lis */

/* ------------------------------------------------ */


int definition_list=0;
void end_dd();

/* Definition List */
void start_dl()
{
	end_dd();
	start_p();
} /* end start_dl */

void end_dl()
{
	paragraphen_ende();

	end_dd();
} /* end_dl */

/* Definition Title */
void start_dt()
{	
	end_dd();
	
	line_break();
} /* end start_dt */

void end_dt()
{
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

