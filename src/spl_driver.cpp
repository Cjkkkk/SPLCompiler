/*
 * @file        spl_driver.cpp
 * @brief       Compiler's frontend.
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/13/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include <cassert>
#include <cctype>
#include <fstream>

#include "spl_driver.hpp"

SPL::SPL_Driver::~SPL_Driver()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void SPL::SPL_Driver::parse(const char *const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good())
    {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
    return;
}

void SPL::SPL_Driver::parse(std::istream &stream)
{
    if (!stream.good() && stream.eof())
    {
        return;
    }
    // else
    parse_helper(stream);
    return;
}

void SPL::SPL_Driver::parse_helper(std::istream &stream)
{

    delete (scanner);
    try
    {
        scanner = new SPL::SPL_Scanner(&stream);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" << ba.what()
                  << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try
    {
        parser =
            new SPL::SPL_Parser((*scanner) /* scanner */, (*this) /* driver */);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what()
                  << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept)
    {
        // std::cerr << "Parse failed!!\n";
        exit(EXIT_FAILURE);
    }
    return;
}

std::ostream &SPL::SPL_Driver::print(std::ostream &stream)
{
    // stream << red << "Results: " << norm << "\n";
    // stream << blue << "Uppercase: " << norm << uppercase << "\n";
    // stream << blue << "Lowercase: " << norm << lowercase << "\n";
    // stream << blue << "Lines: " << norm << lines << "\n";
    // stream << blue << "Words: " << norm << words << "\n";
    // stream << blue << "Characters: " << norm << chars << "\n";
    return (stream);
}