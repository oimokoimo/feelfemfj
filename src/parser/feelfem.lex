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
"element"               { return ELEMENT; }

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

"solve"                 { return SOLVE; }
"solver"                { return SOLVER; }
"quadrature"            { return QUADRATURE; }
"weq"                   { return WEQ; }
"integral"              { return INTEGRAL; }
"bintegral"             { return BINTEGRAL; }
"dbc"                   { return DBC; }
"nbc"                   { return NBC; }
"on"                    { return ON; }


"&&"                    { return AND; }
"||"                    { return OR; }
"!"                     { return NOT; }

"<="                    { return LE; }
">="                    { return GE; }
"=="                    { return EQ; }
"!="                    { return NE; }



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


{DIGIT}+("."{DIGIT}+)?([eE][+-]?{DIGIT}+)? {
    yylval.num = std::atof(yytext);
    return NUMBER;
}

{ID} {
    yylval.str = strdup(yytext);
    return IDENTIFIER;
}


.                       {
                            fprintf(stderr,
                                    "unknown character '%s' at line %d\n",
                                    yytext, yylineno);
                        }

%%