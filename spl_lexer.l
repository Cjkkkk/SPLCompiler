/* 
 * @file        spl_lexer.l
 * @brief       Lexical analyzer of SPL language.
 * @details     This is a lexical analyzer based on Flex.
 * @author      Jiang Xiaochong
 * @date        3/12/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

%{
    #include "spl_parser.tab.h"

    int old_status;
%}

%%
    /* keyword */
and         { return AND; }
array       { return ARRAY; }
begin       { return BEGIN; }
case        { return CASE; }
const       { return CONST; }
div         { return DIV; }
do          { return DO}
downto      { return DOWNTO; }
else        { return ELSE; }
end         { return END; }
for         { return FOR; }
function    { return FUNCTION; }
goto        { return GOTO; }
if          { return IF; }
in          { return IN; }
mod         { return MOD; }
not         { return NOT; }
of          { return OF; }
or          { return OR; }
packed      { return PACKED; }
procedure   { return PROCEDURE; }
program     { return PROGRAM; }
record      { return RECORD; }
repeat      { return REPEAT; }
set         { return SET; }
then        { return THEN; }
to          { return TO; }
type        { return TYPE; }
until       { return UNTIL; }
var         { return VAR; }
while       { return WHILE; }
with        { return WITH; }

    /* system constant*/
false       { return SYS_CON; }
true        { return SYS_CON; }
maxint      { return SYS_CON; }

    /* system function */
abs         { return SYS_FUNCT; }
chr         { return SYS_FUNCT; }
odd         { return SYS_FUNCT; }
ord         { return SYS_FUNCT; }
pred        { return SYS_FUNCT; }
sqr         { return SYS_FUNCT; }
sqrt        { return SYS_FUNCT; }
succ        { return SYS_FUNCT; }

    /* system procedure */
read        { return READ; }
write       { return SYS_PROC; }
writeln     { return SYS_PROC; }

    /* system type */
boolean     { return SYS_TYPE; }
char        { return SYS_TYPE; }
integer     { return SYS_TYPE; }
real        { return SYS_TYPE; }
string      { return SYS_TYPE; }

    /* integer */
-?[0-9]+	            { return INTEGER; }

    /* real */
-?[0-9]+"."[0-9]* |
-?"."[0-9]+	|
-?[0-9]+E[-+]?[0-9]+ |
-?[0-9]+"."[0-9]*E[-+]?[0-9]+ |
-?"."[0-9]*E[-+]?[0-9]+	{ return REAL; }   

    /* character */
'(\\.|''|[^'\n])'       { return CHAR; }

    /* string */
\"(\\.|\"\"|[^"\n])*\"  { return STRING; }

    /* operator */
"+"     { return PLUS; }
"-"     { return MINUS; }
"*"     { return MUL; }
"/"     { return DIV; }
">="    { return GE; }
">"     { return GT; }
"<="    { return LE; }
"<"     { return LT; }
"="     { return EQUAL; }
"<>"    { return UNEQUAL; }
"MOD"   { return MOD; }
"NOT"   { return NOT; }
":="    { return ASSIGN; }

    /* delimiter */
"("     { return LP; }
")"     { return RP; }
"["     { return LB; }
"]"     { return RB; }
","     { return COMMA; }
":"     { return COLON; }
".."    { return DOTDOT; }
"."     { return DOT; }
";"     { return SEMI; }

    /* identifier */
[A-Za-z][A-Za-z0-9_]*   { return NAME; }

    /* comments (C-like) */
"//".*;
"/*"                    { old_status = YY_START; BEGIN COMMENT; }
<COMMENT>"*/"           { BEGIN old_status; }

    /* white space */
[ \t\n]
.

%%