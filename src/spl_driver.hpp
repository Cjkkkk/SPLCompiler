/*
 * @file        spl_driver.hpp
 * @brief       Compiler's frontend.
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/13/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#ifndef __SPLDRIVER_HPP__
#define __SPLDRIVER_HPP__ 1

#include <cstddef>
#include <istream>
#include <string>

#include "spl_parser.tab.hh"
#include "spl_scanner.hpp"
#include "spl_IR.hpp"
#include "spl_SSA.hpp"
namespace SPL
{
class SPL_Driver
{
  public:
    SPL_Driver() : ir(&symtab){};

    virtual ~SPL_Driver();

    /**
   * parse - parse from a file
   * @param filename - valid string with input file
   */
    void parse(const char *const filename);
    /**
   * parse - parse from a c++ input stream
   * @param is - std::istream&, valid input stream
   */
    void parse(std::istream &stream);

    std::ostream &print(std::ostream &stream);
    void emitIR();
    void printIR();
    void genSSATree() {
        ssaIr.root = genSSA(0);
    }
    SSANode* genSSA(int index);
    SymbolTable symtab;
    AST_Manager astmng;
  private:
    void parse_helper(std::istream &stream);
    SPL_IR ir;
    SPL_SSA ssaIr;
    SPL::SPL_Parser *parser = nullptr;
    SPL::SPL_Scanner *scanner = nullptr;
};

} /* end namespace SPL */
#endif /* END __SPLDRIVER_HPP__ */