/* 
 * @file        spl_lexer.l
 * @brief       Lexical analyzer of SPL language.
 * @details     This is a lexical analyzer based on Flex.
 * @author      Jiang Xiaochong
 * @date        3/12/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

%{ /*** C/C++ Declarations ***/
/* C++ string header, for string ops below */
#include <string>

/* Implementation of yyFlexScanner */ 
#include "spl_scanner.hpp"
#undef  YY_DECL
#define YY_DECL int SPL::SPL_Scanner::yylex( SPL::SPL_Parser::semantic_type * const lval, SPL::SPL_Parser::location_type *loc )

/* typedef to make the returns for the tokens shorter */
using token = SPL::SPL_Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return( token::END )

/* msvc2010 requires that we exclude this header file. */
#define YY_NO_UNISTD_H

/* update location on matching */
#define YY_USER_ACTION loc->step(); loc->columns(yyleng);

%}

/*** Flex Declarations and Options ***/

/* enable scanner to generate debug output. disable this for release
 * versions. */
%option debug

/* enable c++ scanner class generation */
%option c++

%option nodefault
%option yyclass="SPL::SPL_Scanner"
%option noyywrap

%%
%{
    /** code executed at the beginning of yylex **/
    yylval = lval;
%}
    /* keyword */
and         { return token::token::AND; }
array       { return token::token::ARRAY; }
begin       { return token::token::BEGIN; }
case        { return token::token::CASE; }
const       { return token::token::CONST; }
div         { return token::token::DIV; }
do          { return token::token::DO}
downto      { return token::token::DOWNTO; }
else        { return token::token::ELSE; }
end         { return token::token::END; }
for         { return token::token::FOR; }
function    { return token::FUNCTION; }
goto        { return token::GOTO; }
if          { return token::IF; }
in          { return token::IN; }
mod         { return token::MOD; }
not         { return token::NOT; }
of          { return token::OF; }
or          { return token::OR; }
packed      { return token::PACKED; }
procedure   { return token::PROCEDURE; }
program     { return token::PROGRAM; }
record      { return token::RECORD; }
repeat      { return token::REPEAT; }
set         { return token::SET; }
then        { return token::THEN; }
to          { return token::TO; }
type        { return token::TYPE; }
until       { return token::UNTIL; }
var         { return token::VAR; }
while       { return token::WHILE; }
with        { return token::WITH; }

    /* system constant*/
false       { return token::SYS_CON; }
true        { return token::SYS_CON; }
maxint      { return token::SYS_CON; }

    /* system function */
abs         { return token::SYS_FUNCT; }
chr         { return token::SYS_FUNCT; }
odd         { return token::SYS_FUNCT; }
ord         { return token::SYS_FUNCT; }
pred        { return token::SYS_FUNCT; }
sqr         { return token::SYS_FUNCT; }
sqrt        { return token::SYS_FUNCT; }
succ        { return token::SYS_FUNCT; }

    /* system procedure */
read        { return token::READ; }
write       { return token::SYS_PROC; }
writeln     { return token::SYS_PROC; }

    /* system type */
boolean     { return token::SYS_TYPE; }
char        { return token::SYS_TYPE; }
integer     { return token::SYS_TYPE; }
real        { return token::SYS_TYPE; }
string      { return token::SYS_TYPE; }

    /* integer */
-?[0-9]+    { return token::INTEGER; }

    /* real */
-?[0-9]+"."[0-9]* |
-?"."[0-9]+	|
-?[0-9]+E[-+]?[0-9]+ |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+ |
-?"."[0-9]*E[-+]?[0-9]+	{ return token::REAL; }   

    /* character */
'(\\.|''|[^'\n])'       { return token::CHAR; }

    /* string */
\"(\\.|\"\"|[^\"\n])*\" { return token::STRING; }

    /* operator */
"+"     { return token::PLUS; }
"-"     { return token::MINUS; }
"*"     { return token::MUL; }
"/"     { return token::DIV; }
">="    { return token::GE; }
">"     { return token::GT; }
"<="    { return token::LE; }
"<"     { return token::LT; }
"="     { return token::EQUAL; }
"<>"    { return token::UNEQUAL; }
"MOD"   { return token::MOD; }
"NOT"   { return token::NOT; }
":="    { return token::ASSIGN; }

    /* delimiter */
"("     { return token::LP; }
")"     { return token::RP; }
"["     { return token::LB; }
"]"     { return token::RB; }
","     { return token::COMMA; }
":"     { return token::COLON; }
".."    { return token::DOTDOT; }
"."     { return token::DOT; }
";"     { return token::SEMI; }

    /* identifier */
[A-Za-z][A-Za-z0-9_]*   { return token::NAME; }

    /* comments (C-like) */
"//".*;
"/*"                    { old_status = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN old_status; }

    /* white space */
[ \t\n]
.

%%