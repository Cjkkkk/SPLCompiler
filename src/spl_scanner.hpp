/*
 * @file        spl_scanner.hpp
 * @brief       Lexer base class announcement.
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/13/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef __SPLSCANNER_HPP__
#define __SPLSCANNER_HPP__ 1

#if !defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "location.hh"
#include "spl_parser.tab.hh"

namespace SPL
{

class SPL_Scanner : public yyFlexLexer
{
  public:
    SPL_Scanner(std::istream *in) : yyFlexLexer(in){};
    virtual ~SPL_Scanner(){};

    // get rid of override virtual function warning
    using FlexLexer::yylex;

    virtual int yylex(SPL::SPL_Parser::semantic_type *const lval,
                      SPL::SPL_Parser::location_type *location);
    // YY_DECL defined in spl_lexer.l
    // Method body created by flex in spl_lexer.yy.cc

  private:
    /* yyval ptr */
    SPL::SPL_Parser::semantic_type *yylval = nullptr;
};

} /* end namespace SPL */

#endif /* END __SPLSCANNER_HPP__ */