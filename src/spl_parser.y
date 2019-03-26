/* 
 * @file        spl_parser.y
 * @brief       Syntax parser of SPL language.
 * @details     This is a syntax parser based on Bison.
 * @author      Jiang Xiaochong
 * @date        3/12/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

/* use newer C++ skeleton file */
%skeleton "lalr1.cc"

/* require bison 3.0 or later */
%require  "3.0"

/* use C++ interface */
%language "c++"

/* add debug output code to generated parser. disable this for release
 * versions. */
%debug 

/* verbose error messages */
%define parse.error verbose

/* write out a header file containing the token defines */
%define api.namespace {SPL}
%define api.parser.class {SPL_Parser}

%code requires{
    namespace SPL {
        class SPL_Driver;
        class SPL_Scanner;
    }
    #include "spl_ast.hpp"
    #include "spl_symtab.hpp"
// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { SPL_Scanner &scanner }
%parse-param { SPL_Driver  &driver  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
    #include <string>
    #include <vector>
    /* include for all driver functions */
    #include "spl_driver.hpp"
#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

// TODO: 运算符
%right  ASSIGN
%left   OR
%left   AND
%left   NOT
%left   EQUAL UNEQUAL GE GT LE LT 
%left   PLUS MINUS
%left   MUL DIV MOD

%token  END     0       "end of file"

%token  ARRAY
%token  _BEGIN
%token  CASE
%token  CONST
%token  DO
%token  ELSE
%token  _END
%token  FOR
%token  FUNCTION
%token  GOTO
%token  IF
%token  IN
%token  OF
%token  PACKED
%token  PROCEDURE
%token  PROGRAM
%token  RECORD
%token  REPEAT
%token  SET
%token  THEN
%token  TYPE
%token  UNTIL
%token  VAR
%token  WHILE
%token  WITH
%token  TO
%token  DOWNTO

%token  <int>           SYS_CON
%token  <int>           SYS_FUNCT
%token                  READ
%token  <int>           SYS_PROC
%token  <int>           SYS_TYPE

%type   <bool>          BOOL
%token  <int>           INTEGER
%token  <double>        REAL
%token  <char>          CHAR
%token  <std::string>   STRING
%token  <std::string>   ID

%token  PLUS
%token  MINUS
%token  MUL
%token  DIV
%token  MOD
%token  OR
%token  AND
%token  EQUAL
%token  UNEQUAL
%token  GE
%token  GT
%token  LE
%token  LT
%token  NOT

%token  ASSIGN

%token  LP
%token  RP
%token  LB
%token  RB
%token  COMMA
%token  COLON
%token  DOTDOT
%token  DOT
%token  SEMI

%type   <bool>                  direction
%type   <AST_Const*>            const_value
%type   <AST_Exp*>              factor term expr expression
%type   <AST_Assign*>           assign_stmt
%type 	<AST_Stmt*>	            else_clause stmt non_label_stmt
%type 	<AST_If*>	            if_stmt case_stmt
%type   <AST_While*>            while_stmt
%type   <AST_Repeat*>           repeat_stmt
%type   <AST_For*>              for_stmt
%type   <AST_Goto*>             goto_stmt
%type   <AST_Compound*>         compound_stmt routine_body
%type   <caseUnit*>             case_expr
%type   <std::vector<caseUnit*>*>   case_expr_list
%type   <std::vector<AST_Stmt*>*>   stmt_list

// %type   <std::vector<Symbol>>   const_part const_expr_list
// %type   <std::vector<Symbol>>   type_part type_decl_list 
// %type   <std::vector<Symbol>>   var_decl var_decl_list 

// %type   <std::map<std::string, SPL_TYPE>>   field_decl field_decl_list

// %type   <Symbol>        type_decl 
// %type   <Symbol>        simple_type_decl 
// %type   <Symbol>        array_type_decl 
// %type   <Symbol>        record_type_decl
// %type   <std::vector<std::string>>  name_list

%locations

%%
program: 
        program_head  routine  DOT {}
        ;

program_head: 
        PROGRAM  ID  SEMI {}
        ;
routine: 
        routine_head  routine_body {}
        ;

sub_routine: 
        routine_head  routine_body {}
        ;

routine_head: 
        label_part  const_part  type_part  var_part  routine_part {}
        ;

label_part: 
        {}
        ;

const_part: 
        CONST  const_expr_list  
        { 
            // $$.swap($2); 
        }
        |  
        { 
            // $$.clear(); 
        }
        ;

const_expr_list: 
        const_expr_list  ID  EQUAL  const_value  SEMI 
        {
            // $$.swap($1);
            // build a new symbol and pushback
            // $$.push_back();

        }
        |  ID  EQUAL  const_value  SEMI 
        {
            // build a new symbol and pushback
            // $$.push_back();
        }
        ;

const_value: 
        INTEGER  {$$ = new AST_Const($1); }
        |  REAL  {$$ = new AST_Const($1); }
        |  CHAR  {$$ = new AST_Const($1); }
        |  STRING  {$$ = new AST_Const($1); }
        |  BOOL {$$ = new AST_Const($1);}
        |  SYS_CON {$$ = new AST_Const($1); }
        ;

type_part: 
        TYPE type_decl_list  
        { 
            //$$.swap($2); 
        }
        |  
        { 
            //$$.clear(); 
        }
        ;

type_decl_list: 
        type_decl_list  type_definition  
        {

        }
        |  type_definition 
        {

        }
        ;
        
type_definition: 
        ID  EQUAL  type_decl  SEMI 
        {

        }
        ;

type_decl: 
        simple_type_decl  {}
        |  array_type_decl  {}
        |  record_type_decl {}
        ;

simple_type_decl: 
        SYS_TYPE  {}
        |  ID  {}
        |  LP  name_list  RP  {}
        |  const_value  DOTDOT  const_value  {}
        |  MINUS  const_value  DOTDOT  const_value {}
        |  MINUS  const_value  DOTDOT  MINUS  const_value {}
        |  ID  DOTDOT  ID {}
        ;

array_type_decl: 
        ARRAY  LB  INTEGER  RB  OF  type_decl {}
        ;

record_type_decl: 
        RECORD  field_decl_list  _END {}
        ;

field_decl_list: 
        field_decl_list  field_decl {}
        |  field_decl {}
        ;

field_decl: 
        name_list  COLON  type_decl  SEMI {}
        ;

name_list: 
        name_list  COMMA  ID 
        {
            // $$.swap($1);
            // $$.push_back($3);
        }
        |  ID 
        {
            // $$.push_back($1);
        }
        ;

var_part: 
        VAR  var_decl_list 
        {

        }
        | 
        {

        }
        ;

var_decl_list:  
        var_decl_list  var_decl 
        {

        }
        |  var_decl 
        {

        }
        ;

var_decl:  
        name_list  COLON  type_decl  SEMI {}
        ;

routine_part:  
        routine_part  function_decl {}
        |  routine_part  procedure_decl {}
        |  function_decl {}
        |  procedure_decl {}
        | {}
        ;

function_decl: 
        function_head  SEMI  sub_routine  SEMI {}
        ;

function_head:  
        FUNCTION  ID  parameters  COLON  simple_type_decl {}
        ;

procedure_decl:  
        procedure_head  SEMI  sub_routine  SEMI {}
        ;

procedure_head:  
        PROCEDURE ID parameters {}
        ;

parameters: 
        LP  para_decl_list  RP {}
        |  {}
        ;

para_decl_list: 
        para_decl_list  SEMI  para_type_list {}
        | para_type_list {}
        ;

para_type_list: 
        var_para_list COLON  simple_type_decl {}
        |  val_para_list  COLON  simple_type_decl {}
        ;

var_para_list: 
        VAR  name_list {}
        ;

val_para_list: 
        name_list {}
        ;

routine_body: 
        compound_stmt {$$ = $1;}
        ;

compound_stmt: 
        _BEGIN  stmt_list  _END {$$ = new AST_Compound($2);}
        ;

stmt_list: 
        stmt_list  stmt  SEMI {$1->push_back($2); $$ = $1;}
        |  {$$->clear();}
        ;

stmt: 
        INTEGER  COLON  non_label_stmt {$$ = $3; /*add the label into the symtable*/ }
        |  non_label_stmt {$$ = $1;}
        ;

non_label_stmt: 
        assign_stmt {$$ = $1;}
        | proc_stmt {}
        | compound_stmt {$$ = $1;}
        | if_stmt {$$ = $1;}
        | repeat_stmt {$$ = $1;}
        | while_stmt {$$ = $1;}
        | for_stmt {$$ = $1;}
        | case_stmt {$$ = $1;}
        | goto_stmt {$$ = $1;}
        ;

// todo: check symbol table to get sym
assign_stmt: 
        ID  ASSIGN  expression {
            AST_Sym* lhs = new AST_Sym($1, nullptr);
	    $$ = new AST_Assign(lhs, $3);
	    //std::cout << $1 << ": " << $$->calculate()<<"\n";
        }
        | ID LB expression RB ASSIGN expression {
            AST_Array* lhs = new AST_Array(new AST_Sym($1, nullptr), $3);
            $$ = new AST_Assign(lhs, $6);
        }
        | ID  DOT  ID  ASSIGN  expression {
            AST_Dot* lhs = new AST_Dot(new AST_Sym($1, nullptr), 
                                       new AST_Sym($3, nullptr));
            $$ = new AST_Assign(lhs, $5);
        }
        ;

proc_stmt: 
        ID {}
        |  ID  LP  args_list  RP {}
        |  SYS_PROC {}
        |  SYS_PROC  LP  expression_list  RP {}
        |  READ  LP  factor  RP {}
        ;

if_stmt: 
        IF  expression  THEN  stmt  else_clause {$$ = new AST_If($2, $4, $5); $$->calculate();}
        ;

else_clause: 
        ELSE stmt {$$ = $2;}
        |  {$$ = nullptr;}
        ;

repeat_stmt: 
        REPEAT  stmt_list  UNTIL  expression {$$ = new AST_Repeat($2, $4);}
        ;

while_stmt: 
        WHILE  expression  DO stmt {$$ = new AST_While($2, $4);}
        ;

for_stmt: 
        FOR  ID  ASSIGN  expression  direction  expression  DO stmt {
            AST_Assign* init = new AST_Assign(new AST_Sym($2, nullptr), $4);
            $$ = new AST_For(init, $5, $6, $8);
        }
        ;

direction: 
        TO {$$ = true;}
        | DOWNTO {$$ = false;}
        ;

case_stmt: 
        CASE expression OF case_expr_list  _END {
            assert(!$4->empty());
            $$ = new AST_If(new AST_Math(EQUAL_, $2, $4->at(0)->val),
                                       $4->at(0)->stmt,
                                       nullptr);
            if($4->size() > 1){
                AST_If* track = $$;
                for(unsigned int i=1; i<$4->size(); i++, track=(AST_If*)track->getDoElse()){
                    track->addRight(new AST_If(new AST_Math(EQUAL_, $2, $4->at(i)->val),
                                               $4->at(i)->stmt,
                                               nullptr));
                }
                //the last null pointer refers to 'default'
            }
        }
        ;

case_expr_list: 
        case_expr_list  case_expr {$1->push_back($2); $$ = $1;}
        |  case_expr {$$->clear(); $$->push_back($1);}
        ;

case_expr: 
        const_value  COLON  stmt  SEMI {$$ = new caseUnit($1, $3);}
        |  ID  COLON  stmt  SEMI {$$ = new caseUnit(new AST_Sym($1, nullptr), 
                                                    $3);}
        ;

goto_stmt: 
        GOTO  INTEGER {$$ = new AST_Goto($2);}
        ;

expression_list: 
        expression_list  COMMA  expression {}
        |  expression {}
        ;

expression: 
        expression  GE  expr {$$ = new AST_Math(GE_, $1, $3);}
        |  expression  GT  expr {$$ = new AST_Math(GT_, $1, $3);}
        |  expression  LE  expr {$$ = new AST_Math(LE_, $1, $3);}
        |  expression  LT  expr {$$ = new AST_Math(LT_, $1, $3);}
        |  expression  EQUAL  expr {$$ = new AST_Math(EQUAL_, $1, $3);}
        |  expression  UNEQUAL  expr {$$ = new AST_Math(UNEQUAL_, $1, $3);}
        |  expr {$$ = $1;}
        ;

expr: 
        expr  PLUS  term {$$ = new AST_Math(PLUS_, $1, $3);}
        |  expr  MINUS  term {$$ = new AST_Math(MINUS_, $1, $3);}
        |  expr  OR  term {$$ = new AST_Math(OR_, $1, $3);}
        |  term {$$ = $1;}
        ;

term: 
        term  MUL  factor {$$ = new AST_Math(MUL_, $1, $3);}
        |  term  DIV  factor {$$ = new AST_Math(DIV_, $1, $3);}
        |  term  MOD  factor {$$ = new AST_Math(MOD_, $1, $3);}
        |  term  AND  factor {$$ = new AST_Math(AND_, $1, $3);}
        |  factor {$$ = $1;}
        ;

factor: 
        ID {$$ = new AST_Sym($1, nullptr);}
        |  ID  LP  args_list  RP {$$ = new AST_Sym($1, nullptr);}
        |  SYS_FUNCT {}
        |  SYS_FUNCT  LP  args_list  RP {}
        |  const_value {$$ = $1;}
        |  LP  expression  RP {$$ = $2;}
        |  NOT  factor {$$ = new AST_Math(NOT_, $2, nullptr);}
        |  MINUS  factor {$$ = new AST_Math(MINUS__, $2, nullptr);}
        |  ID  LB  expression  RB {$$ = new AST_Array(
                                                new AST_Sym($1, nullptr),
                                                $3);}
        |  ID  DOT  ID {$$ = new AST_Dot(
                                        new AST_Sym($1, nullptr),
                                        new AST_Sym($3, nullptr));}
        ;

args_list: 
        args_list  COMMA  expression {}
        |  expression {}
        ;

	
%%

// TODO: error detection

void SPL::SPL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "spl.exe: error: " << err_message << " at " << l << "\n";
}