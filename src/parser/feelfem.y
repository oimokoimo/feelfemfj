/*
 * feelfem.y parser for feelfemfj
 * 
 * Date     2026/06/27
 * 
 * Copyright (C) 2026 Hidehiro Fujio and contributors
 * SPDX-License-Identifier: BSD-3-Clause
 * Repository: https://github.com/oimokoimo/feelfemfj
 *
 * Notes:
 *  - 
 *
 */

 %{
#include <cstdio>
#include <cstdlib>

int yylex(void);
int yyparse(void);
void yyerror(const char* s);

extern int yylineno;
extern char *yytext;


%}


%union
{
  double        num;   /* float value    */
  char*         str;   /* string value   */
}

%token <num> NUMBER
%token <str> IDENTIFIER


/* tokens */
%token MESH VAR SCHEME PROGRAMMODEL
%token POINT EDGE REGION DOMAIN VERTICES
%token DOUBLE INT FEM EWISE VFEM
%token IF THEN ELSE ENDIF
%token LE GE EQ NE
%token AND OR NOT
%token SOLVE SOLVER QUADRATURE WEQ
%token INTEGRAL BINTEGRAL DBC NBC ON

/* precedence */
%left OR
%left AND
%left EQ NE
%left '<' '>' LE GE
%left '+' '-'
%left '*' '/'
%right '^'
%right UMINUS UPLUS NOT

%%

program
    : sections
    ;

sections
    : /* empty */
    | sections section
    ;

section
    : mesh_section
    | var_section
    | scheme_section
    ;

/* ===================== mesh ===================== */

mesh_section
    : MESH '{' mesh_items '}'
    ;

mesh_items
    : /* empty */
    | mesh_items mesh_item
    ;

mesh_item
    : point_statement
    | edge_statement
    | region_statement
    | domain_statement
    | vertices_statement
    ;

point_statement
    : POINT point_list ';'
    ;

point_list
    : point_argument
    | point_list ',' point_argument
    ;

point_argument
    : IDENTIFIER '(' expr_list ')'
    {
        std::printf("point: %s\n",$1);
        std::free($1);
    }
    ;

edge_statement
    : EDGE edge_list ';'
    ;

edge_list
    : edge_argument
    | edge_list ',' edge_argument
    ;

edge_argument
    : IDENTIFIER '(' identifier_list ')'
    ;

region_statement
    : REGION region_list ';'
    ;

region_list
    : region_argument
    | region_list ',' region_argument
    ;

region_argument
    : IDENTIFIER '[' IDENTIFIER ']' '(' region_identifier_list ')'
    ;

region_identifier_list
    : identifier_list
    | identifier_list ':' hole_identifier_list
    ;

hole_identifier_list
    : '[' identifier_list ']'
    | hole_identifier_list ',' '[' identifier_list ']'
    ;

domain_statement
    : DOMAIN domain_list ';'
    ;

domain_list
    : domain_argument
    | domain_list ',' domain_argument
    ;

domain_argument
    : IDENTIFIER '(' identifier_list ')'
    ;

vertices_statement
    : VERTICES '(' expression ')' ';'
    ;

/* ===================== var ===================== */

var_section
    : VAR '{' var_items '}'
    ;

var_items
    : /* empty */
    | var_items var_item
    ;

var_item
    : scalar_var_statement
    | fem_var_statement
    | ewise_var_statement
    | vfem_var_statement
    ;

scalar_var_statement
    : DOUBLE scalar_decl_list ';'
    | INT    scalar_decl_list ';'
    ;

scalar_decl_list
    : scalar_decl
    | scalar_decl_list ',' scalar_decl
    ;

scalar_decl
    : IDENTIFIER
    | IDENTIFIER '=' expression
    ;

fem_var_statement
    : FEM field_decl_list ';'
    ;

ewise_var_statement
    : EWISE field_decl_list ';'
    ;

vfem_var_statement
    : VFEM field_decl_list ';'
    ;

field_decl_list
    : field_decl
    | field_decl_list ',' field_decl
    ;

field_decl
    : IDENTIFIER '[' IDENTIFIER ']'
    ;

/* ===================== scheme ===================== */

scheme_section
    : SCHEME '{' scheme_items '}'
    ;

scheme_items
    : /* empty */
    | scheme_items scheme_item
    ;

scheme_item
    : program_model_statement
    | solve_block
    | statement
    ;

program_model_statement
    : PROGRAMMODEL IDENTIFIER ';'
    ;

statement
    : assignment_statement
    | if_statement
    | ';'
    ;

assignment_statement
    : lvalue '=' expression ';'
    ;

lvalue
    : IDENTIFIER
    | IDENTIFIER '(' argument_list_opt ')'
    ;

if_statement
    : IF '(' expression ')' THEN statement_list ENDIF ';'
    | IF '(' expression ')' THEN statement_list ELSE statement_list ENDIF ';'
    ;

statement_list
    : /* empty */
    | statement_list statement
    ;

/* ======== solve block ============================= */
solve_block
    : SOLVE '[' identifier_list ';' identifier_list ']' '{' solve_items '}'
     {printf("solve block parsed\n");}
    ;

solve_items
    :  /* empty */
    | solve_items solve_item
    ;

solve_item
    : solver_statement
    | quadrature_statement
    | weq_statement
    | dbc_statement
    | nbc_statement
    | statement
    ;

solver_statement
    : SOLVER IDENTIFIER ';'
    ;

quadrature_statement
    : QUADRATURE IDENTIFIER ';'
    ;

weq_statement
    : WEQ ':' integral_form ';'
    | WEQ ':' integral_form '=' expression ';'
      {printf("weq persed\n");}
    | WEQ ':' integral_form '=' integral_form ';'
    ;

dbc_statement
    : DBC ':' IDENTIFIER '=' expression ',' ON identifier_list ';'
    ;

nbc_statement
    : NBC ':' IDENTIFIER '=' expression ',' ON identifier_list ';'
    ;

integral_form
    : integral_term
    | integral_form '+' integral_term
    | integral_form '-' integral_term
    ;

integral_term
    : integral_expr
    | '(' integral_form ')'
    ;

integral_expr
    : INTEGRAL '(' expression ')'
    | INTEGRAL '[' IDENTIFIER ']' '(' expression ')'
    | BINTEGRAL '(' expression ')'
    | BINTEGRAL '[' IDENTIFIER ']' '(' expression ')'
    ;
/* ===================== common ===================== */

expression
    : primary_expression
    | unary_expression
    | binary_expression
    ;

primary_expression
    : NUMBER
    | IDENTIFIER
    | IDENTIFIER '(' argument_list_opt ')'
    | '(' expression ')'
    ;

unary_expression
    : '+' expression %prec UPLUS
    | '-' expression %prec UMINUS
    | NOT expression
    ;

binary_expression
    : expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '^' expression
    | expression '<' expression
    | expression '>' expression
    | expression LE expression
    | expression GE expression
    | expression EQ expression
    | expression NE expression
    | expression AND expression
    | expression OR expression
    ;

argument_list_opt
    : /* empty */
    | expr_list
    ;

expr_list
    : expression
    | expr_list ',' expression
    ;

identifier_list
    : IDENTIFIER
    | identifier_list ',' IDENTIFIER
    ;
    
%%

void yyerror(const char* s)
{
    std::fprintf(stderr,
    "parse error at line %d: near \"%s\" %s\n",yylineno,yytext, s);
}

int main(int argc, char** argv)
{
    if (yyparse() == 0) {
        std::printf("Parse OK\n");
        return 0;
    }

    std::printf("Parse failed\n");
    return 1;
}
