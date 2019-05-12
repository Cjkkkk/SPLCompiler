//
// Created by DELL on 2019/4/30.
//

#ifndef SPLCOMPILER_SPL_CODEGEN_H
#define SPLCOMPILER_SPL_CODEGEN_H

#include <map>
#include <list>
#include <fstream>
#include "spl_IR.hpp"

class SPL_CodeGen {
public:
    explicit SPL_CodeGen( SPL_IR* ir_): ir(ir_){
        outfile.open("assem/hello.s",std::ios::out);
    };
    ~SPL_CodeGen() {
        outfile.close();
    }
    void GenerateMachineCode();

    void writeDirectives();

    void writeSectionText();

    void writeSectionData();


    std::map<std::string, int> name_to_memory;
    SPL_IR* ir;
    std::ofstream outfile;
};


#endif //SPLCOMPILER_SPL_CODEGEN_H
