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

%code requires {
#include "../ast/Ast.hpp"
}


%union
{
  double        num;   /* float value    */
  char*         str;   /* string value   */

  feelfem2::AstNode* node;
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
%token ELEMENT
%token SOLVE SOLVER QUADRATURE WEQ
%token INTEGRAL BINTEGRAL DBC NBC ON

/* types */
%type <node>program_model_statement

%type <node> mesh_section
%type <node> mesh_items
%type <node> mesh_item
%type <node> point_statement
%type <node> point_list
%type <node> point_argument

%type <node> expr_list
%type <node> expression_list
%type <node> expression
%type <node> primary_expression
%type <node> binary_expression
%type <node> unary_expression

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
    | element_definition
    | quadrature_definition
    | scheme_section
    ;

/* ===================== mesh ===================== */

mesh_section
    : MESH '{' mesh_items '}'
       {
          $$ = $3;
       }
    ;

mesh_items
    : /* empty */
      {
        $$ = nullptr;
      }
    | mesh_items mesh_item
      {
        $$ = $1 ? $1 : $2;
      }
    ;

mesh_item
    : point_statement
        {
          $$ = $1;
        }
    | edge_statement
        {
          $$ = nullptr;
        }
    | region_statement
        {
          $$ = nullptr;
        }
    | domain_statement
        {
          $$ = nullptr;
        }
    | vertices_statement
        {
          $$ = nullptr;
        }
    ;

point_statement
    : POINT point_list ';'
        {
          $$ = $2;
        }
    ;

point_list
    : point_argument
    | point_list ',' point_argument
    ;

point_argument
    : IDENTIFIER '(' expr_list ')'
    {
          auto *p = new feelfem2::PointDecl(
             std::string($1),
             $3,
             feelfem2::SourceLocation{ yylineno , 0 }
          );
          p->printout();
          $$ = p;
          free($1);
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

/* ====================quadrature ====================== */
quadrature_definition
    : QUADRATURE IDENTIFIER '[' IDENTIFIER ']' '{' quadrature_items '}'
    ;

quadrature_items
    : /* empty */
    | quadrature_items quadrature_item
    ;

quadrature_item
    : quad_var_declaration
    | quad_assignment_statement
    | quad_point_statement
    ;

quad_var_declaration
    : DOUBLE quad_var_list ';'
    ;

quad_var_list
    : quad_var
    | quad_var_list ',' quad_var
    ;

quad_var
    : IDENTIFIER
    | IDENTIFIER '=' expression
    ;

quad_assignment_statement
    : IDENTIFIER '=' expression ';'
    ;

quad_point_statement
    : '(' expression_list ')' ':' expression ';'
    ;

expression_list
    : expression
       {
          $$ = nullptr;
       }
    | expression_list ',' expression
    ;


/* ====================element==================== */

element_definition
    : ELEMENT IDENTIFIER '[' IDENTIFIER']' '{' element_items '}'
    ;

element_items
    : /* empty */
    | element_items element_item
    ;

element_item
    : elem_var_declaration
    | elem_assignment_statement
    | elem_point_statement
    ;
elem_var_declaration
    : DOUBLE elem_var_list ';'
    ;

elem_var_list
    : elem_var
    | elem_var_list ',' elem_var
    ;

elem_var
    : IDENTIFIER
    | IDENTIFIER '=' expression
    ;

elem_assignment_statement
    : IDENTIFIER '=' expression ';'
    ;

elem_point_statement
    : '(' elem_coord_list ')' ':' expression ';'
    ;

elem_coord_list
    : expression
    | elem_coord_list ',' expression
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
    {
        auto* pm = new feelfem2::ProgramModel($2,feelfem2::SourceLocation(yylineno));
        pm->printout();
        $$ = pm; 
        free($2);
    }
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
        { $$ = $1; }
    | unary_expression
        { $$ = $1; }
    | binary_expression
        { $$ = $1; }
    ;

primary_expression
    : NUMBER
       { $$ = nullptr; }
    | IDENTIFIER
       { $$ = nullptr; free($1); }
    | IDENTIFIER '(' argument_list_opt ')'
       { $$ = nullptr; free($1); }
    | '(' expression ')'
       { $$ = $2; }
    ;

unary_expression
    : '+' expression %prec UPLUS
       { $$ = $2; }
    | '-' expression %prec UMINUS
       { $$ = $2; }
    | NOT expression
       { $$ = $2; }
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
