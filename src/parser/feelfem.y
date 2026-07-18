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

%code requires {
#include <vector>
#include "../ast/Ast.hpp"

}
 %{
#include <cstdio>
#include <cstdlib>


#include "../ast/Ast.hpp"

int yylex(void);
int yyparse(void);
void yyerror(const char* s);

extern int yylineno;
extern char *yytext;

feelfem2::VarSection * gVarSection = nullptr;


%}

//%code requires {
//#include <vector>
//#include "../ast/Ast.hpp"
//
//}


%union
{
  double        num;   /* float value    */
  char*         str;   /* string value   */

  feelfem2::AstNode* node;
  std::vector<std::string>* identifierList;
  feelfem2::Expression* expr;

  std::vector<feelfem2::Expression*>*exprList;
  std::vector<feelfem2::PointDecl*>* pointDeclList;

  std::vector<feelfem2::EdgeDecl *>* edgeDeclList;

  std::vector<feelfem2::VariableDeclarator*>* varDecList;
  std::vector<feelfem2::FieldDeclarator*>*fieldDecList;
  std::vector<feelfem2::Declaration*>* declarationList;
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
%type <pointDeclList> point_list
%type <node> point_argument

/* identifier list */
%type <identifierList> identifier_list

/* expression */
%type <node> expression_list

%type <expr> expression
%type <exprList> expr_list

%type <expr> primary_expression
%type <expr> binary_expression
%type <expr> unary_expression


/* var section */
%type <node> var_section
%type <declarationList> var_items
%type <node> var_item

%type <node> field_decl
%type <node> fem_var_statement
%type <node> ewise_var_statement
%type <node> vfem_var_statement
%type <fieldDecList> field_decl_list

%type <node> scalar_decl
%type <node> scalar_var_statement
%type <varDecList> scalar_decl_list

%type <node> edge_argument
%type <edgeDeclList> edge_list
%type <node> edge_statement


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
          auto* statement =
              new feelfem2::PointStatement(
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* point : *$2)
          {
              statement->AddPoint(point);
          }
       //   statement->printout();

          delete $2;

          $$ = statement;
      }
    ;

point_list
    : point_argument
      {
          $$ = new std::vector<feelfem2::PointDecl*>;

          $$->push_back(
              static_cast<feelfem2::PointDecl*>($1)
          );
      }
    | point_list ',' point_argument
      {
          $1->push_back(
              static_cast<feelfem2::PointDecl*>($3)
          );

          $$ = $1;
      }
    ;

point_argument
    : IDENTIFIER '(' expr_list ')'
    {
          auto *p = new feelfem2::PointDecl(
             std::string($1),
             $3,
             feelfem2::SourceLocation{ yylineno , 0 }
          );
          $$ = p;
          free($1);
    }
    ;

edge_statement
    : EDGE edge_list ';'
      {
          auto* statement =
              new feelfem2::EdgeStatement(
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* edge : *$2)
          {
              statement->AddEdge(edge);
          }
//          statement->printout();

          delete $2;
          $$ = statement;
      }
    ;

edge_list
    : edge_argument
      {
          $$ = new std::vector<feelfem2::EdgeDecl*>;

          $$->push_back(
              static_cast<feelfem2::EdgeDecl*>($1)
          );
      }
    | edge_list ',' edge_argument
      {
          $1->push_back(
              static_cast<feelfem2::EdgeDecl*>($3)
          );

          $$ = $1;
      }
    ;

edge_argument
    : IDENTIFIER '(' identifier_list ')'
      {
          auto* edge =
              new feelfem2::EdgeDecl(
                  std::string($1),
                  $3,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          $$ = edge;
          free($1);
      }
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
      {
          auto* section =
              new feelfem2::VarSection(
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declaration : *$3)
          {
              section->AddDeclaration(
                  static_cast<feelfem2::Declaration*>(declaration)
              );
          }

          delete $3;

        //  section->printout();
          gVarSection = section;
          $$ = section;
      }
    ;

var_items
    : /* empty */
      {
          $$ = new std::vector<feelfem2::Declaration*>;
      }
    | var_items var_item
      {
          $1->push_back(
              static_cast<feelfem2::Declaration*>($2)
          );

          $$ = $1;
      }
    ;

var_item
    : scalar_var_statement
      {
         $$ = $1;
      }
    | fem_var_statement
      {
         $$ = $1;
      }
    | ewise_var_statement
      {
         $$ = $1;
      }
    | vfem_var_statement
      {
         $$ = $1;
      }
    ;

scalar_var_statement
    : DOUBLE scalar_decl_list ';'
      {
          auto* declaration =
              new feelfem2::VariableDeclaration(
                  feelfem2::ScalarType::Double,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declarator : *$2)
          {
              declaration->AddDeclarator(declarator);
          }

          delete $2;

//          declaration->printout();
          $$ = declaration;
      }
    | INT scalar_decl_list ';'
      {
          auto* declaration =
              new feelfem2::VariableDeclaration(
                  feelfem2::ScalarType::Int,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declarator : *$2)
          {
              declaration->AddDeclarator(declarator);
          }

          delete $2;

 //         declaration->printout();
          $$ = declaration;
      }
    ;


scalar_decl_list
    : scalar_decl
      {
          $$ = new std::vector<feelfem2::VariableDeclarator*>;

          $$->push_back(
              static_cast<feelfem2::VariableDeclarator*>($1)
          );
      }
    | scalar_decl_list ',' scalar_decl
      {
          $1->push_back(
              static_cast<feelfem2::VariableDeclarator*>($3)
          );

          $$ = $1;
      }
    ;


scalar_decl
    : IDENTIFIER
      {
          auto* decl =
              new feelfem2::VariableDeclarator(
                  std::string($1),
                  feelfem2::SourceLocation{yylineno, 0}
              );

          $$ = decl;
          free($1);
      }
    | IDENTIFIER '=' expression
      {
          auto* decl =
              new feelfem2::VariableDeclarator(
                  std::string($1),
                  $3,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          $$ = decl;
          free($1);
      }
    ;

fem_var_statement
    : FEM field_decl_list ';'
      {
          auto* declaration =
              new feelfem2::FieldDeclaration(
                  feelfem2::FieldType::Fem,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declarator : *$2)
          {
              declaration->AddDeclarator(declarator);
          }

          delete $2;

//          declaration->printout();
          $$ = declaration;
      }
    ;

ewise_var_statement
    : EWISE field_decl_list ';'
      {
          auto* declaration =
              new feelfem2::FieldDeclaration(
                  feelfem2::FieldType::Ewise,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declarator : *$2)
          {
              declaration->AddDeclarator(declarator);
          }

          delete $2;

//          declaration->printout();
          $$ = declaration;
      }
    ;

vfem_var_statement
    : VFEM field_decl_list ';'
      {
          auto* declaration =
              new feelfem2::FieldDeclaration(
                  feelfem2::FieldType::Vfem,
                  feelfem2::SourceLocation{yylineno, 0}
              );

          for (auto* declarator : *$2)
          {
              declaration->AddDeclarator(declarator);
          }

          delete $2;

//          declaration->printout();
          $$ = declaration;
      }
    ;

field_decl_list
    : field_decl
      {
        $$ = new std::vector<feelfem2::FieldDeclarator*>;
        $$->push_back(
            static_cast<feelfem2::FieldDeclarator*>($1)
        );
      }
    | field_decl_list ',' field_decl
      {
        $1->push_back(
            static_cast<feelfem2::FieldDeclarator*>($3)
        );
        $$ = $1;
      }
    ;

field_decl
    : IDENTIFIER '[' IDENTIFIER ']'
      {
          auto* field = new feelfem2::FieldDeclarator(
              std::string($1),
              std::string($3),
              feelfem2::SourceLocation{yylineno, 0}
          );

//          field->printout();
          $$ = field;

          free($1);
          free($3);
      } 
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
       { $$ = new feelfem2::NumberExpr(
              $1,
              feelfem2::SourceLocation{yylineno, 0}
               );
         //$$->printout();
       }
    | IDENTIFIER
       { $$ = new feelfem2::IdentifierExpr( 
                    std::string($1),
                    feelfem2::SourceLocation{yylineno, 0}
              );
               free($1);

       }

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
      {
          $$ = new std::vector<feelfem2::Expression*>;
          $$->push_back($1);
      }
    | expr_list ',' expression
      {
          $1->push_back($3);
          $$ = $1;
      }
    ;

identifier_list
    : IDENTIFIER
      {
          $$ = new std::vector<std::string>;
          $$->push_back(std::string($1));
          free($1);
      }
    | identifier_list ',' IDENTIFIER
      {
          $1->push_back(std::string($3));
          free($3);
          $$ = $1;
      }
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

// varSection

        if(gVarSection)
        {
          std::cout << "\n--- VarSection AST ---\n";
          gVarSection->printout();
        }


        return 0;
    }

    std::printf("Parse failed\n");
    return 1;
}
