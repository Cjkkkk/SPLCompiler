/* 
 * @file 		spl_parser.l
 * @brief		Syntax parser of SPL language.
 * @details	    This is a syntax parser based on Bison.
 * @author		Jiang Xiaochong
 * @date		3/12/2019
 * @version	    1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

%token  AND
%token  ARRAY
%token  BEGIN
%token  CASE
%token  CONST
%token  DIV
%token  DO
%token  DOWNTO
%token  ELSE
%token  END
%token  FOR
%token  FUNCTION
%token  GOTO
%token  IF
%token  IN
%token  MOD
%token  NOT
%token  OF
%token  OR
%token  PACKED
%token  PROCEDURE
%token  PROGRAM
%token  RECORD
%token  REPEAT
%token  SET
%token  THEN
%token  TO
%token  TYPE
%token  UNTIL
%token  VAR
%token  WHILE
%token  WITH

%token  SYS_CON
%token  SYS_FUNCT
%token  SYS_PROC
%token  SYS_TYPE
%token  READ

%token  INTEGER
%token  REAL
%token  CHAR
%token  STRING

%token  PLUS
%token  MINUS
%token  MUL
%token  DIV
%token  GE
%token  GT
%token  LE
%token  LT
%token  EQUAL
%token  UNEQUAL
%token  MOD
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
%token  NAME

%{
    #include <stdio.h>
    #include <stdlib.h>
	#include <string.h>
    
%}

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
        CONST  const_expr_list  {}
        |  {}
        ;

const_expr_list: 
        const_expr_list  NAME  EQUAL  const_value  SEMI {}
        |  NAME  EQUAL  const_value  SEMI {}
        ;

const_value: 
        INTEGER  {}
        |  REAL  {}
        |  CHAR  {}
        |  STRING  {}
        |  SYS_CON {}
        ;

type_part: 
        TYPE type_decl_list  {}
        |  {}
        ;

type_decl_list: 
        type_decl_list  type_definition  {}
        |  type_definition {}
        ;
        
type_definition: 
        NAME  EQUAL  type_decl  SEMI {}
        ;

type_decl: 
        simple_type_decl  {}
        |  array_type_decl  {}
        |  record_type_decl {}
        ;

simple_type_decl: 
        SYS_TYPE  {}
        |  NAME  {}
        |  LP  name_list  RP  {}
        |  const_value  DOTDOT  const_value  {}
        |  MINUS  const_value  DOTDOT  const_value {}
        |  MINUS  const_value  DOTDOT  MINUS  const_value {}
        |  NAME  DOTDOT  NAME {}
        ;

array_type_decl: 
        ARRAY  LB  simple_type_decl  RB  OF  type_decl {}
        ;

record_type_decl: 
        RECORD  field_decl_list  END {}
        ;

field_decl_list: 
        field_decl_list  field_decl {}
        |  field_decl {}
        ;

field_decl: 
        name_list  COLON  type_decl  SEMI {}
        ;

name_list: 
        name_list  COMMA  ID {}
        |  ID {}
        ;

var_part: 
        VAR  var_decl_list {}
        | {}
        ;

var_decl_list:  
        var_decl_list  var_decl {}
        |  var_decl {}
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
        FUNCTION  NAME  parameters  COLON  simple_type_decl {}
        ;

procedure_decl:  
        procedure_head  SEMI  sub_routine  SEMI {}
        ;

procedure_head:  
        PROCEDURE NAME parameters {}
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
        compound_stmt {}
        ;

compound_stmt: 
        BEGIN  stmt_list  END {}
        ;

stmt_list: 
        stmt_list  stmt  SEMI {}
        |  {}
        ;

stmt: 
        INTEGER  COLON  non_label_stmt {}
        |  non_label_stmt {}
        ;

non_label_stmt: 
        assign_stmt {}
        | proc_stmt {}
        | compound_stmt {}
        | if_stmt {}
        | repeat_stmt {}
        | while_stmt {}
        | for_stmt {}
        | case_stmt {}
        | goto_stmt {}
        ;

assign_stmt: 
        ID  ASSIGN  expression {}
        | ID LB expression RB ASSIGN expression {}
        | ID  DOT  ID  ASSIGN  expression {}
        ;

proc_stmt: 
        ID {}
        |  ID  LP  args_list  RP {}
        |  SYS_PROC {}
        |  SYS_PROC  LP  expression_list  RP {}
        |  READ  LP  factor  RP {}
        ;

if_stmt: 
        IF  expression  THEN  stmt  else_clause {}
        ;

else_clause: 
        ELSE stmt {}
        |  {}
        ;

repeat_stmt: 
        REPEAT  stmt_list  UNTIL  expression {}
        ;

while_stmt: 
        WHILE  expression  DO stmt {}
        ;

for_stmt: 
        FOR  ID  ASSIGN  expression  direction  expression  DO stmt {}
        ;

direction: 
        TO {}
        | DOWNTO {}
        ;

case_stmt: 
        CASE expression OF case_expr_list  END {}
        ;

case_expr_list: 
        case_expr_list  case_expr {}
        |  case_expr {}
        ;

case_expr: 
        const_value  COLON  stmt  SEMI {}
        |  ID  COLON  stmt  SEMI {}
        ;

goto_stmt: 
        GOTO  INTEGER {}
        ;

expression_list: 
        expression_list  COMMA  expression {}
        |  expression {}
        ;

expression: 
        expression  GE  expr {}
        |  expression  GT  expr {}
        |  expression  LE  expr {}
        |  expression  LT  expr {}
        |  expression  EQUAL  expr {}
        |  expression  UNEQUAL  expr {}
        |  expr {}
        ;

expr: 
        expr  PLUS  term {}
        |  expr  MINUS  term {}
        |  expr  OR  term {}
        |  term {}
        ;

term: 
        term  MUL  factor {}
        |  term  DIV  factor {}
        |  term  MOD  factor {}
        |  term  AND  factor {}
        |  factor {}
        ;

factor: 
        NAME {}
        |  NAME  LP  args_list  RP {}
        |  SYS_FUNCT {}
        |  SYS_FUNCT  LP  args_list  RP {}
        |  const_value {}
        |  LP  expression  RP {}
        |  NOT  factor {}
        |  MINUS  factor {}
        |  ID  LB  expression  RB {}
        |  ID  DOT  ID {}
        ;

args_list: 
        args_list  COMMA  expression {}
        |  expression {}
        ;

	
%%

// TODO: error detection

int main(int argc, char * argv[])
{
    if (argc > 1)
    {
        if (!(yyin = fopen(argv[1], "r"))) 
        {
            perror(argv[1]);
            return -1;
        }
        yylex();
        fclose(yyin);
        return 0;
    }
    return -1;
}
