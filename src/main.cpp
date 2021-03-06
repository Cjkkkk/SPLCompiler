/* 
 * @file        main.cpp
 * @brief       Program entry.
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/13/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include <iostream>
#include <fstream>
#include <cstdio>
#include "spl_driver.hpp"
#include "spl_exception.hpp"
//#include "consoleColor.hpp"

int main(int argc, char *argv[])
{
    SPL::SPL_Driver driver;
    if (argc > 1)
    {
        try
        {
            fstream fout;
            fout.open("./astvisio/ast1.txt", ios::out);
            driver.parse(argv[1]);
            driver.printAST(fout);
            fout.close();

            // emit IR
            driver.emitIR();

            // opt IR
            // driver.optimizeIR();

            // code Gen
            driver.codeGen();
        }
        catch (splException &e)
        {
            std::cout << argv[1] << ": " << e.line << ":" << e.offset << ": error: " << e.errorMessage << "\n";
        }
        return 0;
    }
    else
    {
        std::cout << "spl.exe: error: no input files" << std::endl;
        return 1;
    }
}