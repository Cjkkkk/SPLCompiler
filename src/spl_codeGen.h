//
// Created by DELL on 2019/4/30.
//

#ifndef SPLCOMPILER_SPL_CODEGEN_H
#define SPLCOMPILER_SPL_CODEGEN_H

#include <map>
#include <list>
#include <set>
#include <fstream>
#include "spl_IR.hpp"

class SPL_CodeGen {
public:
    explicit SPL_CodeGen( SPL_IR* ir_): ir(ir_){
        outfile.open("assem/hello.asm",std::ios::out);
        callee_saved_registers = {
                "ebp",
                "ebx",
                "r12",
                "r13",
                "r14",
                "r15"
        };
    };
    ~SPL_CodeGen() {
        outfile.close();
    }
    void GenerateMachineCode();

    // write global test/ section .text ,etc
    void writeDirectives(const std::string& instr, const std::string& op);

    // generate function
    void writeSectionTextSubroutine();

    // write string literal as label
    void writeStringLiteral();

    // write global variable, since our target language does not support initialize global variable
    // we use .bss section
    void writeSectionConstData();


    // intermediate representations
    SPL_IR* ir;

    // output file
    std::ofstream outfile;

    // string literal
    std::vector<Instruction*> string_literals;

    std::map<std::string, int> name_to_memory;

    // callee saved
    std::set<std::string> callee_saved_registers;
};


#endif //SPLCOMPILER_SPL_CODEGEN_H
