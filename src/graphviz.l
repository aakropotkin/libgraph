/*
 * Filename: graphviz.l
 * Purpose : Lexer for the Graphviz/dot file parser.
 * Author  : Nikolaos Kavvadias (C) 2007-2020
 * Date    : 04-Oct-2014
 * Revision: 1.0.0 (14/10/04)
 *           Updated for github.
 */ 
%{

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "graphviz.tab.h"

#define MAXSIZE  1024
char any_name_s[MAXSIZE];

void count( void );

%}

%option noinput
%option nounput

ws	  [ \t\n]
anum  [a-zA-Z0-9_][a-zA-Z0-9_.]*
digit [0-9]
A	  [aA]
B	  [bB]
C	  [cC]
D	  [dD]
E	  [eE]
F	  [fF]
G	  [gG]
H	  [hH]
I	  [iI]
J	  [jJ]
K	  [kK]
L	  [lL]
M	  [mM]
N	  [nN]
O	  [oO]
P	  [pP]
Q	  [qQ]
R	  [rR]
S	  [sS]
T	  [tT]
U	  [uU]
V	  [vV]
W	  [wW]
X	  [xX]
Y	  [yY]
Z	  [zZ]

%%
"//".*          { count( void ); }
\"(\\.|[^\\"])*\"	{ count( void );
                  strcpy( any_name_s, yytext );
                  return( T_STRING ); }
{ws}+		        ;
"["             { count( void );
                  return( T_LBRACKET ); }
"]"             { count( void );
                  return( T_RBRACKET ); }
"( "             { count( void );
                  return( T_LPAREN ); }
" )"             { count( void );
                  return( T_RPAREN ); }
"{"             { count( void );
                  return( T_LBRACE ); }
"}"             { count( void );
                  return( T_RBRACE ); }
","             { count( void );
                  return( T_COMMA ); }
":"             { count( void );
                  return( T_COLON ); }
";"             { count( void );
                  return( T_SEMI ); }
"->"            { count( void );
                  return( T_DEDGE ); }
"--"            { count( void );
                  return( T_UEDGE ); }
"="             { count( void );
                  return( T_EQ ); }
{D}{I}{G}{R}{A}{P}{H}     { count( void );
                  return( T_DIGRAPH ); }
{G}{R}{A}{P}{H}           { count( void );
                  return( T_GRAPH ); }
{S}{U}{B}{G}{R}{A}{P}{H}  { count( void );
                  return( T_SUBGRAPH ); }
{A}{T}          { count( void );
                  return( T_AT ); }
{S}{T}{R}{I}{C}{T}        { count( void );
                  return( T_STRICT ); }
{N}{O}{D}{E}    { count( void );
                  return( T_NODE ); }
{E}{D}{G}{E}    { count( void );
                  return( T_EDGE ); }
{anum}          { count( void );
                  strcpy( any_name_s, yytext );
                  return( T_ID ); }

%%


  int
yywrap( void )
{
  return 1;
}

int column = 0;

  void
count( void )
{
  for ( int i = 0; yytext[i] != '\0'; i++ )
    if ( yytext[i] == '\n' )
      {
        column = 0;
      }
    else if ( yytext[i] == '\t' )
      {
        column += 8 - ( column % 8 );
      }
    else
      {
        column++;
      }

  ECHO;
}
