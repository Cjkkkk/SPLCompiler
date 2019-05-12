//
// Created by DELL on 2019/4/30.
//

#include <fstream>
#include "spl_codeGen.h"

void SPL_CodeGen::GenerateMachineCode() {
    ir->setToMain();
    writeGlobal();
    writeText();
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
    }
}

void SPL_CodeGen::writeGlobal() {
    outfile << "\t.global " << ir->getCurrentIR()[0]->label << endl;
}

void SPL_CodeGen::writeText() {
    outfile<< "\t.text" << endl;
}