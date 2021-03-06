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

%locations
%define api.namespace {SPL}
%define api.parser.class {SPL_Parser}

%code requires{
    namespace SPL {
        class SPL_Driver;
        class SPL_Scanner;
    }
    #include "spl_ast.hpp"
    #include "spl_symtab.hpp"
    #include "spl_compiler.hpp"

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

#define checkTypeEqual(left_type, right_type) (left_type == right_type)
#define checkTypeUnequal(left_type, right_type) (left_type != right_type)
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
    #include "spl_exception.hpp"

#undef yylex
#define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert


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
%token  <int>           READ
%token  <int>           SYS_PROC
%token  <int>           SYS_TYPE

%token  <bool>          BOOL
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

%type   <bool> direction
%type	<std::string*> program_head
%type   <AST_Compound*> routine_body
%type   <AST_Exp*> factor term expr expression
%type   <AST_Const*> const_value
%type   <AST_Assign*> assign_stmt
%type 	<AST_Stmt*> else_clause stmt non_label_stmt
%type 	<AST_If*> if_stmt case_stmt
%type   <AST_While*> while_stmt
%type   <AST_Repeat*> repeat_stmt
%type   <AST_For*> for_stmt
%type   <AST_Goto*> goto_stmt
%type   <AST_Func*> proc_stmt
%type   <AST_Compound*> compound_stmt
%type   <caseUnit*> case_expr
%type   <std::vector<caseUnit*>*> case_expr_list
%type   <std::vector<AST_Stmt*>*> stmt_list
%type   <std::vector<AST_Exp*>*> args_list

%type   <Symbol*> type_decl simple_type_decl array_type_decl record_type_decl
%type   <SymbolListType*> field_decl field_decl_list
%type   <SymbolListType*> parameters para_decl_list para_type_list
%type   <std::vector<std::string>*> name_list var_para_list val_para_list


%%
program:
        program_head  routine  DOT
        {

        }
        ;

program_head:
        PROGRAM  ID  SEMI
        {
            $$ = new std::string($2); // program head can be just a string
            driver.symtab.pushScope($2); // push a symbol
        }
        ;
routine:
        routine_head  routine_body
        {
           driver.astmng.addFunc($2, driver.symtab.getCurrentScopeIndex(), 0);
        }
        ;

sub_routine:
        routine_head  routine_body
        {
            driver.astmng.addFunc($2, driver.symtab.getCurrentScopeIndex(), driver.symtab.getPrevScopeIndex());
        }
        ;

routine_head:
        label_part  const_part  type_part  var_part  routine_part
        {
        	// do nothing
        }
        ;

label_part:
        {
        }
        ;

const_part:
        CONST  const_expr_list
        {

        }
        |
        {
        }
        ;

const_expr_list:
        const_expr_list  ID  EQUAL  const_value  SEMI
        {
            Symbol* symbol = new Symbol($2, CONST, $4->valType, driver.symtab.getCurrentScopeIndex());
            symbol->relevantASTNode = $4;
            driver.symtab.addVariable(symbol);
        }
        |  ID  EQUAL  const_value  SEMI
        {
             // 检查变量是否已经被定义过， 没定义过则添加到符号表
            Symbol* symbol =  new Symbol($1, CONST, $3->valType, driver.symtab.getCurrentScopeIndex());
            symbol->relevantASTNode = $3;
            driver.symtab.addVariable(symbol);
        }
        ;

const_value:
        INTEGER
        {
            $$ = new AST_Const($1);
            $$->valType = INT;
        }
        |  REAL
        {
            $$ = new AST_Const($1);
            $$->valType = REAL;
        }
        |  CHAR
        {
            $$ = new AST_Const($1);
            $$->valType = CHAR;
        }
        |  STRING
        {
            $$ = new AST_Const($1);
            $$->valType = STRING;
        }
        |  BOOL
        {
            $$ = new AST_Const($1);
            $$->valType = BOOL;
        }
        |  SYS_CON
        {
            $$ = new AST_Const($1);
        }
        ;

type_part:
        TYPE type_decl_list
        {

        }
        |
        {

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
            $3->name = $1;
            driver.symtab.addType($3);
        }
        ;

type_decl:
        simple_type_decl
        {
            $$ = $1;
        }
        |  array_type_decl
        {
            $$ = $1;
        }
        |  record_type_decl
        {
            $$ = $1;
        }
        ;

simple_type_decl:
        SYS_TYPE
        {
            $$ = new Symbol("", TYPE, (SPL_TYPE)$1, driver.symtab.getCurrentScopeIndex());
        }
        |  ID
        {
            Symbol* symbol = driver.symtab.lookupType($1.c_str());
            std::string errorMsg = "spl.exe: error: undefined symbol \"" + $1 + "\"";
            Assert(symbol != nullptr, errorMsg.c_str());
            $$ = new Symbol(*symbol);
            $$->name = "";
        }
        |  LP  name_list  RP
        {
            // TODO: enumeration type
        }
        |  const_value  DOTDOT  const_value
        {
            // TODO: subrange type
        }
        |  MINUS  const_value  DOTDOT  const_value
        {

        }
        |  MINUS  const_value  DOTDOT  MINUS  const_value
        {

        }
        |  ID  DOTDOT  ID
        {

        }
        ;

array_type_decl:
        ARRAY  LB  INTEGER  RB  OF  type_decl
        {
            Assert($3 >= 1, "spl.exe: error: illegal array index");
            SPL_TYPE elementType = $6->symbolType;
            Symbol* symbol = new Symbol("", TYPE, ARRAY, driver.symtab.getCurrentScopeIndex());
            symbol->elementType = elementType;
            symbol->scalarSize = $3;
//            if (elementType >= BOOL && elementType <= STRING)
//                delete $6;
//            else
                symbol->elementTypePtr = $6;
            $$ = symbol;
        }
        ;

record_type_decl:
        RECORD  field_decl_list  _END
        {
            Symbol* symbol = new Symbol("", TYPE, RECORD, driver.symtab.getCurrentScopeIndex());
            SymbolMapType* subSymbolMap = new SymbolMapType;
            for(size_t i = 0; i < $2->size(); i++)
            {
                std::string name = (*$2)[i]->name;
                std::string errorMsg =
                "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
                Assert(subSymbolMap->find(name) == subSymbolMap->end(), errorMsg.c_str());
                (*subSymbolMap)[name] = (*$2)[i];
            }
            symbol->subSymbolMap = subSymbolMap;
            symbol->subSymbolList = $2;
            $$ = symbol;
        }
        ;

field_decl_list:
        field_decl_list  field_decl
        {
            $1->insert($1->end(), $2->begin(), $2->end());
            delete $2;
            $$ = $1;
        }
        |  field_decl
        {
            $$ = $1;
        }
        ;

field_decl:
        name_list  COLON  type_decl  SEMI
        {
            SymbolListType* subSymbolList = new SymbolListType;
            for(size_t i = 0; i < $1->size(); i++)
            {
                Symbol* symbol = new Symbol(*$3);
                symbol->name = (*$1)[i];
                subSymbolList->push_back(symbol);
            }
            delete $1;
            delete $3;
            $$ = subSymbolList;
        }
        ;

name_list:
        name_list  COMMA  ID
        {
            $1->push_back($3);
            $$ = $1;
        }
        |  ID
        {
            std::vector<std::string>* newlist = new std::vector<std::string>();
            newlist->push_back($1);
            $$ = newlist;
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
        name_list  COLON  type_decl  SEMI
        {
            for(size_t i = 0; i < $1->size(); i++)
            {
                Symbol* symbol = new Symbol(*$3);
                symbol->name = (*$1)[i];
                symbol->scopeIndex = driver.symtab.getCurrentScopeIndex();
                //std::cout << symbol->name << " " << driver.symtab.getCurrentScopeIndex() << "\n";
                symbol->symbolClass = VAR;
                driver.symtab.addVariable(symbol);
            }
            delete $1;
            delete $3;
        }
        ;

routine_part:
        routine_part  function_decl
        {
            driver.symtab.popScope();
        }
        |  routine_part  procedure_decl
        {
            driver.symtab.popScope();
        }
        |  function_decl
        {
            driver.symtab.popScope();
        }
        |  procedure_decl
        {
            driver.symtab.popScope();
        }
        |
        {

        }
        ;

function_decl:
        function_head  SEMI  sub_routine  SEMI {
        	// 检查是否有返回值节点 很麻烦...
        }
        ;

function_head:
        FUNCTION  ID  parameters  COLON  simple_type_decl
        {
            Symbol* symbol = new Symbol($2, FUNC, $5->symbolType, driver.symtab.getCurrentScopeIndex());
            SymbolMapType* subSymbolMap = new SymbolMapType;
            for(size_t i = 0; i < $3->size(); i++)
            {
                std::string name = (*$3)[i]->name;
                std::string errorMsg =
                "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
                Assert(subSymbolMap->find(name) == subSymbolMap->end(), errorMsg.c_str());
                (*subSymbolMap)[name] = (*$3)[i];
            }
            symbol->subSymbolMap = subSymbolMap;
            symbol->subSymbolList = $3;
//            if ($5->symbolType >= BOOL && $5->symbolType <= STRING)
//                delete $5;
//            else
            symbol->returnTypePtr = $5;

            driver.symtab.addFunction(symbol);
            // new scope
            driver.symtab.pushScope($2);

            // 默认添加function名字到symbo table中作为一个变量
            driver.symtab.addVariable(new Symbol($2, VAR, $5->symbolType, driver.symtab.getPrevScopeIndex()));
            for(size_t i = 0; i < $3->size(); i++){
            	(*$3)[i]->scopeIndex = driver.symtab.getCurrentScopeIndex();
                driver.symtab.addVariable((*$3)[i]);
            }

        }
        ;

procedure_decl:
        procedure_head  SEMI  sub_routine  SEMI {}
        ;

procedure_head:
        PROCEDURE ID parameters
        {
            Symbol* symbol = new Symbol($2, FUNC, UNKNOWN, driver.symtab.getCurrentScopeIndex());
            SymbolMapType* subSymbolMap = new SymbolMapType;
            for(size_t i = 0; i < $3->size(); i++)
            {
                std::string name = (*$3)[i]->name;
                std::string errorMsg =
                "spl.exe: error: ignoring redeclaration of symbol \"" + name + "\".";
                Assert(subSymbolMap->find(name) == subSymbolMap->end(), errorMsg.c_str());
                (*subSymbolMap)[name] = (*$3)[i];
                // set parentscope
            }
            symbol->subSymbolMap = subSymbolMap;
            symbol->subSymbolList = $3;

            driver.symtab.addFunction(symbol);
            // new scope
            driver.symtab.pushScope($2);
            for(size_t i = 0; i < $3->size(); i++) {
            	(*$3)[i]->scopeIndex = driver.symtab.getCurrentScopeIndex();
            	driver.symtab.addVariable((*$3)[i]);
            }

        }
        ;

parameters:
        LP  para_decl_list  RP
        {
            $$ = $2;
        }
        |
        {
            $$ = new SymbolListType;
        }
        ;

para_decl_list:
        para_decl_list  SEMI  para_type_list
        {
            $1->insert($1->end(), $3->begin(), $3->end());
            delete $3;
            $$ = $1;
        }
        | para_type_list
        {
            $$ = $1;
        }
        ;

para_type_list:
        var_para_list COLON  simple_type_decl
        {
            std::vector<Symbol*>* newlist = new std::vector<Symbol*>();
            for(size_t i = 0; i < $1->size(); i++)
            {
                Symbol* symbol = new Symbol(*$3);
                symbol->name = (*$1)[i];
                symbol->symbolClass = VAR;
                symbol->paraType = REFER;
                newlist->push_back(symbol);
            }
            delete $3;
            $$ = newlist;
        }
        |  val_para_list  COLON  simple_type_decl
        {
            std::vector<Symbol*>* newlist = new std::vector<Symbol*>();
            for(size_t i = 0; i < $1->size(); i++)
            {
                Symbol* symbol = new Symbol(*$3);
                symbol->name = (*$1)[i];
                symbol->symbolClass = VAR;
                newlist->push_back(symbol);
            }
            delete $3;
            $$ = newlist;
        }
        ;

var_para_list:
        VAR  name_list
        {
            $$ = $2;
        }
        ;

val_para_list:
        name_list
        {
            $$ = $1;
        }
        ;

routine_body:
        compound_stmt { $$ = $1; }
        ;

compound_stmt:
        _BEGIN  stmt_list  _END {$$ = new AST_Compound($2);}
        ;

stmt_list:
        stmt_list  stmt  SEMI {$1->push_back($2); $$ = $1;}
        |  {$$ = new std::vector<AST_Stmt*>();}
        ;
// todo
stmt:
        INTEGER  COLON  non_label_stmt {$$ = $3; /*add the label into the symtable*/ }
        |  non_label_stmt {$$ = $1;}
        ;

non_label_stmt:
        assign_stmt {$$ = $1;}
        | proc_stmt {$$ = $1;}
        | compound_stmt {$$ = $1;}
        | if_stmt {$$ = $1;}
        | repeat_stmt {$$ = $1;}
        | while_stmt {$$ = $1;}
        | for_stmt {$$ = $1;}
        | case_stmt {$$ = $1;}
        | goto_stmt {$$ = $1;}
        ;

// todo: add symbol share between nodes
assign_stmt:
        ID  ASSIGN  expression {
		auto sym = driver.symtab.lookupVariable($1.c_str());
		if(!sym) {
			throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
		}
		if(checkTypeUnequal(sym->symbolType,$3->valType)) {
			throw splException{@1.begin.line, @1.begin.column, "invaild conversion from '" + typeToString($3->valType) + "' to '" + typeToString(sym->symbolType) + "'.\n"};
		}
		AST_Sym* lhs = new AST_Sym($1, sym->scopeIndex, sym);
		$$ = new AST_Assign(lhs, $3);
		lhs->valType = $3->valType;
        }
        | ID LB expression RB ASSIGN expression {
        // 查看类型是否匹配
	    auto sym = driver.symtab.lookupVariable($1.c_str());
	    if(!sym) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
	    }

	    if($3->valType != INT) {
		throw splException{@1.begin.line, @1.begin.column , "index of array '" + $1 + "' must be type INT, get " + typeToString($3->valType) +".\n"};
             }

            AST_Array* lhs = new AST_Array(new AST_Sym($1, sym->scopeIndex, sym), $3);
            $$ = new AST_Assign(lhs, $6);
        }
        | ID  DOT  ID  ASSIGN  expression {
        auto sym = driver.symtab.lookupVariable($1.c_str());
	if(!sym) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
	}
	if(sym->symbolType != RECORD) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not record type.\n"};
	}
	auto members = sym->subSymbolMap;
	if(members == nullptr) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' has not members.\n"};
	}
	auto member = members->find($3);
	if(member == members->end()) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' has not member " + $3 +"\n"};
	}

	if(member->second->symbolType != $5->valType) {
		throw splException{@1.begin.line, @1.begin.column , "invaild conversion from '" + typeToString($5->valType) + "' to '" + typeToString(member->second->symbolType) + "'.\n"};
	}


        AST_Dot* lhs = new AST_Dot(
                       	new AST_Sym($1, sym->scopeIndex, sym),
                       	new AST_Sym($3, member->second->scopeIndex, member->second));
        $$ = new AST_Assign(lhs, $5);
        $$->valType = member->second->symbolType;
        }
        ;

proc_stmt:
        ID  LP  RP
	{
	auto sym = driver.symtab.lookupFunction($1.c_str());
	if(!sym) {
		// 函数未定义
		throw splException{@1.begin.line, @1.begin.column , "procedure '" + $1 + "' is not declared in this scope.\n"};
	}
	std::vector<AST_Exp*>* emptyVec = new std::vector<AST_Exp*>();
	$$ = new AST_Func(true, $1, emptyVec, sym->scopeIndex, sym);
	}
        |  ID  LP  args_list  RP {
        auto sym = driver.symtab.lookupFunction($1.c_str());
	if(!sym) {
		// 函数未定义
		throw splException{@1.begin.line, @1.begin.column , "procedure '" + $1 + "' is not declared in this scope.\n"};
	}

	auto args_list = sym->subSymbolList;
	if(args_list->size()!=$3->size()){
		// 传入参数数目与定义不一致
		throw splException{@1.begin.line, @1.begin.column ,
		"function or procedure '" + $1 + "' expect " + std::to_string(args_list->size()) + " arguments, got " + std::to_string($3->size()) +".\n"};
	}
	int size = $3->size();
	for(auto i = 0 ; i < size ; i ++ ){
		// 检查数据类型是否一致
		if(checkTypeUnequal($3->at(i)->valType, args_list->at(i)->symbolType)){
			throw splException{@3.begin.line, @3.begin.column ,
			"function or procedure '" + $1 + "' expect type '" + typeToString(args_list->at(i)->symbolType) + "', got type '"+ typeToString($3->at(i)->valType) +"'.\n"};
		}
	}
        $$ = new AST_Func(true, $1, $3, 0, sym);
        }
        |  SYS_PROC  LP  RP
	{
	std::vector<AST_Exp*>* emptyVec = new std::vector<AST_Exp*>();
	$$ = new AST_Func($1, emptyVec);
	}
        |  SYS_PROC  LP  args_list  RP {
        $$ = new AST_Func($1, $3);
        }
        |  READ  LP  factor  RP
	{
//                std::vector<AST_Exp*>* factorVec = new std::vector<AST_Exp*>();
//                factorVec->push_back($3);
//                $$ = new AST_Func($1, factorVec);
	}
        ;

if_stmt:
        IF  expression  THEN  stmt  else_clause {$$ = new AST_If($2, $4, $5);}
        ;

else_clause:
        ELSE stmt {$$ = $2;}
        |  {$$ = nullptr;}
        ;

repeat_stmt:
        REPEAT  stmt_list  UNTIL  expression {$$ = new AST_Repeat($2, $4);}
        ;

while_stmt:
        WHILE  expression  DO stmt {
        $$ = new AST_While($2, $4);
        }
        ;

for_stmt:
        FOR  ID  ASSIGN  expression  direction  expression  DO stmt {
        	// todo 查询变量是否存在
	    auto sym = driver.symtab.lookupVariable($2.c_str());

	    if(!sym) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $2 + "' is not declared in this scope.\n"};
	    }

	    if(checkTypeUnequal(sym->symbolType,$4->valType)) {
            	throw splException{@1.begin.line, @1.begin.column, "invaild conversion from '" + typeToString($4->valType) + "' to '" + typeToString(sym->symbolType) + "'.\n"};
            }
            AST_Assign* init = new AST_Assign(new AST_Sym($2, 0, sym), $4);
            $$ = new AST_For(init, $5, $6, $8);
        }
        ;

direction:
        TO {$$ = true;}
        | DOWNTO {$$ = false;}
        ;

case_stmt:
        CASE expression OF case_expr_list  _END {
            Assert(!$4->empty(), "");
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
        |  case_expr {$$ = new std::vector<caseUnit*>(); $$->push_back($1);}
        ;

case_expr:
        const_value  COLON  stmt  SEMI {$$ = new caseUnit($1, $3);}
        |  ID  COLON  stmt  SEMI {
        auto sym = driver.symtab.lookupVariable($1.c_str());
	if(!sym) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
	}
        $$ = new caseUnit(new AST_Sym($1, 0, sym),$3);
        }
        ;

goto_stmt:
        GOTO  INTEGER {$$ = new AST_Goto($2);}
        ;

expression:
        expression  GE  expr {
        $$ = new AST_Math(GE_, $1, $3);
        if(checkTypeUnequal($1->valType,SPL_TYPE::INT) && checkTypeUnequal($1->valType,SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
			"operator '>=' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}
	if(checkTypeUnequal($3->valType ,SPL_TYPE::INT) && checkTypeUnequal($3->valType,SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
			"operator '>=' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}

	$$->valType = SPL_TYPE::BOOL;
        }
        |  expression  GT  expr {
        $$ = new AST_Math(GT_, $1, $3);
        if(checkTypeUnequal($1->valType, SPL_TYPE::INT) && checkTypeUnequal($1->valType ,SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
			"operator '>' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}
	if(checkTypeUnequal($3->valType,SPL_TYPE::INT) && checkTypeUnequal($3->valType ,SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
			"operator '>' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}

	$$->valType = SPL_TYPE::BOOL;
        }
        |  expression  LE  expr {
        $$ = new AST_Math(LE_, $1, $3);
        if(checkTypeUnequal($1->valType ,SPL_TYPE::INT) && checkTypeUnequal($1->valType, SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
			"operator '<=' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}
	if(checkTypeUnequal($3->valType, SPL_TYPE::INT) && checkTypeUnequal($3->valType, SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
			"operator '<=' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}

	$$->valType = SPL_TYPE::BOOL;
        }
        |  expression  LT  expr {
        $$ = new AST_Math(LT_, $1, $3);
        if(checkTypeUnequal($1->valType,SPL_TYPE::INT) && checkTypeUnequal($1->valType, SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
                	"operator '<' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}
	if(checkTypeUnequal($3->valType,SPL_TYPE::INT) && checkTypeUnequal($3->valType, SPL_TYPE::REAL)){
		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
			"operator '<' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}

	$$->valType = SPL_TYPE::BOOL;

        }
        |  expression  EQUAL  expr {
        $$ = new AST_Math(EQUAL_, $1, $3);
        if(checkTypeUnequal($1->valType, $3->valType)){
		// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
		"operator '==' expect two operands with same type .\n"};
	}
	$$->valType = SPL_TYPE::BOOL;
        }
        |  expression  UNEQUAL  expr {
        $$ = new AST_Math(UNEQUAL_, $1, $3);
        if(checkTypeUnequal($1->valType, $3->valType)){

		throw splException{@3.begin.line, @3.begin.column ,
                "operator '!=' expect two operands with same type .\n"};
	}

        $$->valType = SPL_TYPE::BOOL;

        }
        |  expr {$$ = $1;}
        ;

expr:
        expr  PLUS  term {
        $$ = new AST_Math(PLUS_, $1, $3);
        if(checkTypeUnequal($1->valType, SPL_TYPE::INT) && checkTypeUnequal($1->valType ,SPL_TYPE::REAL)){

	// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
		"operator '+' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}

	if(checkTypeUnequal($3->valType,SPL_TYPE::INT) && checkTypeUnequal($3->valType,SPL_TYPE::REAL)){

	// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column,
		"operator '+' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}

	if(checkTypeEqual($1->valType, SPL_TYPE::REAL) || checkTypeEqual($3->valType, SPL_TYPE::REAL)) {
		$$->valType = SPL_TYPE::REAL;
	}else{
		$$->valType = SPL_TYPE::INT;
	}
        }
        |  expr  MINUS  term {
        $$ = new AST_Math(MINUS_, $1, $3);
        if(checkTypeUnequal($1->valType ,SPL_TYPE::INT) && checkTypeUnequal($1->valType, SPL_TYPE::REAL)){

	// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column,
		"operator '-' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}

	if(checkTypeUnequal($3->valType ,SPL_TYPE::INT) && checkTypeUnequal($3->valType, SPL_TYPE::REAL)){

	// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column,
		"operator '-' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}
	if(checkTypeEqual($1->valType, SPL_TYPE::REAL) || checkTypeEqual($3->valType, SPL_TYPE::REAL)) {
		$$->valType = SPL_TYPE::REAL;
	}else{
		$$->valType = SPL_TYPE::INT;
	}
        }
        |  expr  OR  term {
        $$ = new AST_Math(OR_, $1, $3);
        if(checkTypeUnequal($1->valType, SPL_TYPE::BOOL)){

	// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
		"operator 'OR' expect type 'BOOL', got '" + typeToString($1->valType) + "'.\n"};
	}

	if(checkTypeUnequal($3->valType, SPL_TYPE::BOOL)){

	// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
		"operator 'OR' expect type 'BOOL', got '" + typeToString($3->valType) + "'.\n"};
	}
	$$->valType = SPL_TYPE::BOOL;
        }
        |  term {$$ = $1;}
        ;

term:
        term  MUL  factor {
        $$ = new AST_Math(MUL_, $1, $3);
        if(checkTypeUnequal($1->valType, SPL_TYPE::INT) && checkTypeUnequal($1->valType,SPL_TYPE::REAL)){
	// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
		"operator '*' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}

	if(checkTypeUnequal($3->valType, SPL_TYPE::INT) && checkTypeUnequal($3->valType, SPL_TYPE::REAL)){

	// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
		"operator '*' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}
	if(checkTypeEqual($1->valType, SPL_TYPE::REAL) || checkTypeEqual($3->valType, SPL_TYPE::REAL)) {
		$$->valType = SPL_TYPE::REAL;
	}else{
		$$->valType = SPL_TYPE::INT;
	}
        }
        |  term  DIV  factor {
        $$ = new AST_Math(DIV_, $1, $3);
        if(checkTypeUnequal($1->valType ,SPL_TYPE::INT) && checkTypeUnequal($1->valType, SPL_TYPE::REAL)){
	// 类型不匹配
		throw splException{@1.begin.line, @1.begin.column ,
		"operator '/' expect type 'INT' or 'REAL', got '" + typeToString($1->valType) + "'.\n"};
	}

	if(checkTypeUnequal($3->valType, SPL_TYPE::INT) && checkTypeUnequal($3->valType, SPL_TYPE::REAL)){
	// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
		"operator '/' expect type 'INT' or 'REAL', got '" + typeToString($3->valType) + "'.\n"};
	}
	if(checkTypeEqual($1->valType, SPL_TYPE::REAL) || checkTypeEqual($3->valType,SPL_TYPE::REAL)) {
		$$->valType = SPL_TYPE::REAL;
	}else{
		$$->valType = SPL_TYPE::INT;
	}
	}
        |  term  MOD  factor {
        $$ = new AST_Math(MOD_, $1, $3);
        if(checkTypeUnequal($1->valType,SPL_TYPE::INT )){

         	// 类型不匹配
         	throw splException{@1.begin.line, @1.begin.column ,
			"operator 'MOD' expect type 'INT', got '" + typeToString($1->valType) + "'.\n"};
		}
        if(checkTypeUnequal($3->valType, SPL_TYPE::INT )){

		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
			"operator 'MOD' expect type 'INT', got '" + typeToString($3->valType) + "'.\n"};
	}
	$$->valType = SPL_TYPE::INT;
        }
        |  term  AND  factor {
        $$ = new AST_Math(AND_, $1, $3);
        if(checkTypeUnequal($1->valType, SPL_TYPE::BOOL)){
         	// 类型不匹配
         	throw splException{@1.begin.line, @1.begin.column ,
                		"operator 'AND' expect type 'BOOL', got '" + typeToString($1->valType) + "'.\n"};
         	}
        if(checkTypeUnequal($3->valType ,SPL_TYPE::BOOL)){

		// 类型不匹配
		throw splException{@3.begin.line, @3.begin.column ,
		"operator 'AND' expect type 'BOOL', got '" + typeToString($3->valType) + "'.\n"};
	}
	$$->valType = SPL_TYPE::BOOL;
        }

        |  factor {$$ = $1;}
        ;

factor:
        ID {
        // 检查sym table查看table的类型
        auto sym = driver.symtab.lookupVariable($1.c_str());
        if(!sym) {
        	throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
        }
        $$ = new AST_Sym($1, sym->scopeIndex, sym);
        $$->valType = sym->symbolType;
        }
        |  ID  LP  RP
        {
        // 检查sym table 查看func的返回值类型
        auto sym = driver.symtab.lookupFunction($1.c_str());
        if(!sym) {
                // 函数未定义
		throw splException{@1.begin.line, @1.begin.column , "function or procedure '" + $1 + "' is not declared in this scope.\n"};
        }
	std::vector<AST_Exp*>* emptyVec = new std::vector<AST_Exp*>();
	$$ = new AST_Func(false, $1, emptyVec, sym->scopeIndex, sym);
        $$->valType = sym->symbolType;
        }
        |  ID  LP  args_list  RP {
        // 检查sym table 查看func的返回值类型
        auto sym = driver.symtab.lookupFunction($1.c_str());
	if(!sym) {
		// 函数未定义
		throw splException{@1.begin.line, @1.begin.column , "function or procedure '" + $1 + "' is not declared in this scope.\n"};
	}
	if(sym->symbolType == UNKNOWN) {
		// procedure 不可以做为函数右值
		throw splException{@1.begin.line, @1.begin.column , "procedure '" + $1 + "' can not be used as rvalue.\n"};

	}
	// 查看参数列表的类型是否一致
	auto args_list = sym->subSymbolList;
	if(args_list->size()!=$3->size()){
		// 传入参数数目与定义不一致
		throw splException{@1.begin.line, @1.begin.column ,
		"function or procedure '" + $1 + "' expect " + std::to_string(args_list->size()) + " arguments, got " + std::to_string($3->size()) +".\n"};
	}
	int size = $3->size();
	for(auto i = 0 ; i < size ; i ++ ){
		// 检查数据类型是否一致
		if(checkTypeUnequal($3->at(i)->valType, args_list->at(i)->symbolType)){
			throw splException{@3.begin.line, @3.begin.column ,
                        "function or procedure '" + $1 + "' expect type '" + typeToString(args_list->at(i)->symbolType) + "', got type '"+ typeToString($3->at(i)->valType) +"'.\n"};
		}

		// 检查是否传递常量或者临时变量给引用类型
		if(args_list->at(i)->paraType == REFER && $3->at(i)->nodeType != AST_SYM) {
			throw splException{@3.begin.line, @3.begin.column ,
                        "can not pass temp result or literal as reference in function or procedure '" + $1 + "'n" };
		}
	}
        $$ = new AST_Func(false, $1, $3, sym->scopeIndex, sym);
        // 推导为返回值的数据类型
        $$->valType = sym->symbolType;
        }
        |  SYS_FUNCT  LP  RP
	{
            std::vector<AST_Exp*>* emptyVec = new std::vector<AST_Exp*>();
            $$ = new AST_Func($1, emptyVec);
	}
        |  SYS_FUNCT  LP  args_list  RP {
        	$$ = new AST_Func($1, $3);
        }
        |  const_value {
        $$ = $1;
        }
        |  LP  expression  RP {$$ = $2;}
        |  NOT  factor {
        $$ = new AST_Math(NOT_, $2, nullptr);
        $$->valType = SPL_TYPE::BOOL;
        }
        |  MINUS  factor {
        $$ = new AST_Math(MINUS__, $2, nullptr);
        $$->valType = $2->valType;
        }
        |  ID  LB  expression  RB {
        auto sym = driver.symtab.lookupVariable($1.c_str());
	if(!sym) {
		// 函数未定义
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
	}
	if($3->valType != INT) {
		throw splException{@1.begin.line, @1.begin.column , "index of array '" + $1 + "' must be type INT, get " + typeToString($3->valType) +".\n"};
	}
        $$ = new AST_Array(new AST_Sym($1, sym->scopeIndex, sym),$3);
        $$->valType = INT;
        }
        |  ID  DOT  ID {
        auto sym = driver.symtab.lookupVariable($1.c_str());
	if(!sym) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not declared in this scope.\n"};
	}
	$$ = new AST_Sym($1, sym->scopeIndex, sym);
	if(sym->symbolType != RECORD) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' is not record type.\n"};
	}
	auto members = sym->subSymbolMap;
	if(members == nullptr) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' has not members.\n"};
	}
	auto member = members->find($3);
	if(member == members->end()) {
		throw splException{@1.begin.line, @1.begin.column , "variable '" + $1 + "' has not member " + $3 +"\n"};
	}
        $$ = new AST_Dot(
	new AST_Sym($1, sym->scopeIndex, sym),
	new AST_Sym($3, member->second->scopeIndex, member->second));
	$$->valType = member->second->symbolType;
	}
        ;

args_list: 
        args_list  COMMA  expression {
            $1->push_back($3);
            $$ = $1;
        }
        | expression {
            $$ = new std::vector<AST_Exp*>();
            $$->push_back($1);
        }
        ;

%%

// TODO: error detection

void SPL::SPL_Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "spl.exe: error: " << err_message << " at " << l << "\n";
}