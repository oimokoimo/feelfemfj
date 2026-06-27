%{
#include "feelfem.tab.h"
#include <cstdlib>
#include <cstring>
%}

%option noyywrap
%option yylineno

DIGIT   [0-9]
ID      [A-Za-z_][A-Za-z0-9_]*

%%

[ \t\r\n]+              ;

"//".*                  ;
"/*"([^*]|\*+[^*/])*\*+"/" ;

"mesh"                  { return MESH; }
"var"                   { return VAR; }
"scheme"                { return SCHEME; }

"ProgramModel"          { return PROGRAMMODEL; }

"point"                 { return POINT; }
"edge"                  { return EDGE; }
"region"                { return REGION; }
"domain"                { return DOMAIN; }
"vertices"              { return VERTICES; }

"double"                { return DOUBLE; }
"int"                   { return INT; }
"fem"                   { return FEM; }
"ewise"                 { return EWISE; }
"vfem"                  { return VFEM; }

"if"                    { return IF; }
"then"                  { return THEN; }
"else"                  { return ELSE; }
"endif"                 { return ENDIF; }

"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }

"<="                    { return LE; }
">="                    { return GE; }
"=="                    { return EQ; }
"!="                    { return NE; }

{DIGIT}+("."{DIGIT}+)?([eE][+-]?{DIGIT}+)? {
                            return NUMBER;
                        }

{ID}                    { return IDENTIFIER; }

"{"                     { return '{'; }
"}"                     { return '}'; }
"("                     { return '('; }
")"                     { return ')'; }
"["                     { return '['; }
"]"                     { return ']'; }
";"                     { return ';'; }
","                     { return ','; }
":"                     { return ':'; }

"+"                     { return '+'; }
"-"                     { return '-'; }
"*"                     { return '*'; }
"/"                     { return '/'; }
"^"                     { return '^'; }
"="                     { return '='; }
"<"                     { return '<'; }
">"                     { return '>'; }

.                       {
                            fprintf(stderr,
                                    "unknown character '%s' at line %d\n",
                                    yytext, yylineno);
                        }

%%