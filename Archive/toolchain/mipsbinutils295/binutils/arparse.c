#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#line 2 "arparse.y"
/* arparse.y - Stange script language parser */

/*   Copyright (C) 1992, 93, 95, 97, 98, 1999 Free Software Foundation, Inc.

This file is part of GNU Binutils.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */


/* Contributed by Steve Chamberlain
   		  sac@cygnus.com

*/
#define DONTDECLARE_MALLOC
#include "bfd.h"
#include "bucomm.h"
#include "arsup.h"
extern int verbose;
extern int yylex PARAMS ((void));
static int yyerror PARAMS ((const char *));
#line 36 "arparse.y"
typedef union {
  char *name;
struct list *list ;

} YYSTYPE;
#line 51 "y.tab.c"
#define NEWLINE 257
#define VERBOSE 258
#define FILENAME 259
#define ADDLIB 260
#define LIST 261
#define ADDMOD 262
#define CLEAR 263
#define CREATE 264
#define DELETE 265
#define DIRECTORY 266
#define END 267
#define EXTRACT 268
#define FULLDIR 269
#define HELP 270
#define QUIT 271
#define REPLACE 272
#define SAVE 273
#define OPEN 274
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    5,    0,    4,    4,    6,    7,    7,    7,    7,    7,
    7,    7,    7,    7,    7,    7,    7,    7,    7,    7,
    7,   16,   17,   13,   18,   14,   19,   15,    8,    9,
   12,   11,    3,    3,    1,    1,    2,    2,   20,   20,
   10,
};
short yylen[] = {                                         2,
    0,    2,    2,    0,    2,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    0,    2,    2,    1,    2,    2,    1,    1,    2,    2,
    3,    4,    1,    0,    3,    0,    3,    0,    1,    0,
    1,
};
short yydefred[] = {                                      1,
    0,    4,    0,   19,   41,   20,    0,   27,   38,   24,
    0,   38,    0,   18,   38,   38,   28,    0,    3,    0,
    6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
   16,   17,    0,    0,   30,    0,    0,    0,    0,   29,
    5,   38,   31,   39,    0,    0,    0,   37,   33,   32,
   35,
};
short yydgoto[] = {                                       1,
   43,   34,   50,    3,    2,   19,   20,   21,   22,   23,
   24,   25,   26,   27,   28,   29,   30,   31,   32,   45,
};
short yysindex[] = {                                      0,
    0,    0, -251,    0,    0,    0, -257,    0,    0,    0,
 -253,    0, -240,    0,    0,    0,    0, -234,    0, -221,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   -3,   -6,    0,   -6,   -3,   -6,   -6,    0,
    0,    0,    0,    0, -220, -219,  -17,    0,    0,    0,
    0,
};
short yyrindex[] = {                                      0,
    0,    0,    1,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0, -216, -239,    0, -231, -228, -225, -224,    0,
    0,    0,    0,    0,    0, -215, -234,    0,    0,    0,
    0,
};
short yygindex[] = {                                      0,
    6,  -12,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 258
short yytable[] = {                                      36,
    2,   33,   38,   39,    4,   35,    5,    6,    7,    8,
    9,   10,   11,   12,   13,   14,   15,   26,   37,   40,
   16,   17,   18,   51,   40,   25,   44,   40,   36,   47,
   36,   22,   23,   40,   40,   41,   42,   44,   48,   49,
   36,   34,   46,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   21,
};
short yycheck[] = {                                      12,
    0,  259,   15,   16,  256,  259,  258,  259,  260,  261,
  262,  263,  264,  265,  266,  267,  268,  257,  259,  259,
  272,  273,  274,   41,  259,  257,   44,  259,  257,   42,
  259,  257,  257,  259,  259,  257,   40,   44,  259,  259,
  257,  257,   37,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,   -1,
   -1,   -1,   -1,   -1,   -1,   -1,   -1,  257,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 274
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"NEWLINE","VERBOSE",
"FILENAME","ADDLIB","LIST","ADDMOD","CLEAR","CREATE","DELETE","DIRECTORY","END",
"EXTRACT","FULLDIR","HELP","QUIT","REPLACE","SAVE","OPEN",
};
char *yyrule[] = {
"$accept : start",
"$$1 :",
"start : $$1 session",
"session : session command_line",
"session :",
"command_line : command NEWLINE",
"command : open_command",
"command : create_command",
"command : verbose_command",
"command : directory_command",
"command : addlib_command",
"command : clear_command",
"command : addmod_command",
"command : save_command",
"command : extract_command",
"command : replace_command",
"command : delete_command",
"command : list_command",
"command : END",
"command : error",
"command : FILENAME",
"command :",
"extract_command : EXTRACT modulename",
"replace_command : REPLACE modulename",
"clear_command : CLEAR",
"delete_command : DELETE modulename",
"addmod_command : ADDMOD modulename",
"list_command : LIST",
"save_command : SAVE",
"open_command : OPEN FILENAME",
"create_command : CREATE FILENAME",
"addlib_command : ADDLIB FILENAME modulelist",
"directory_command : DIRECTORY FILENAME modulelist optional_filename",
"optional_filename : FILENAME",
"optional_filename :",
"modulelist : '(' modulename ')'",
"modulelist :",
"modulename : modulename optcomma FILENAME",
"modulename :",
"optcomma : ','",
"optcomma :",
"verbose_command : VERBOSE",
};
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 193 "arparse.y"

static int
yyerror (x)
     const char *x ATTRIBUTE_UNUSED;
{
  extern int linenumber;

  printf (_("Syntax error in archive script, line %d\n"), linenumber + 1);
  return 0;
}
#line 269 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 1:
#line 67 "arparse.y"
{ prompt(); }
break;
case 5:
#line 76 "arparse.y"
{ prompt(); }
break;
case 18:
#line 91 "arparse.y"
{ ar_end(); return 0; }
break;
case 20:
#line 93 "arparse.y"
{ yyerror("foo"); }
break;
case 22:
#line 100 "arparse.y"
{ ar_extract(yyvsp[0].list); }
break;
case 23:
#line 105 "arparse.y"
{ ar_replace(yyvsp[0].list); }
break;
case 24:
#line 110 "arparse.y"
{ ar_clear(); }
break;
case 25:
#line 115 "arparse.y"
{ ar_delete(yyvsp[0].list); }
break;
case 26:
#line 119 "arparse.y"
{ ar_addmod(yyvsp[0].list); }
break;
case 27:
#line 124 "arparse.y"
{ ar_list(); }
break;
case 28:
#line 129 "arparse.y"
{ ar_save(); }
break;
case 29:
#line 136 "arparse.y"
{ ar_open(yyvsp[0].name,0); }
break;
case 30:
#line 141 "arparse.y"
{ ar_open(yyvsp[0].name,1); }
break;
case 31:
#line 147 "arparse.y"
{ ar_addlib(yyvsp[-1].name,yyvsp[0].list); }
break;
case 32:
#line 151 "arparse.y"
{ ar_directory(yyvsp[-2].name, yyvsp[-1].list, yyvsp[0].name); }
break;
case 33:
#line 158 "arparse.y"
{ yyval.name = yyvsp[0].name; }
break;
case 34:
#line 159 "arparse.y"
{ yyval.name = 0; }
break;
case 35:
#line 164 "arparse.y"
{ yyval.list = yyvsp[-1].list; }
break;
case 36:
#line 166 "arparse.y"
{ yyval.list = 0; }
break;
case 37:
#line 171 "arparse.y"
{ 	struct list *n  = (struct list *) malloc(sizeof(struct list));
			n->next = yyvsp[-2].list; 
			n->name = yyvsp[0].name;
			yyval.list = n;
		 }
break;
case 38:
#line 176 "arparse.y"
{ yyval.list = 0; }
break;
case 41:
#line 188 "arparse.y"
{ verbose = !verbose; }
break;
#line 502 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
