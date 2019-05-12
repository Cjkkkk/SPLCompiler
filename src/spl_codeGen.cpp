//
// Created by DELL on 2019/4/30.
//

#include <fstream>
#include "spl_codeGen.h"

void SPL_CodeGen::GenerateMachineCode() {
    ir->setToMain();
    writeDirectives();
    writeSectionText();
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
        writeSectionTextSubroutine();
    }
    writeSectionData();
}

void SPL_CodeGen::writeDirectives() {
    outfile << "\tglobal\t" << ir->getCurrentIR()[0]->label << endl;
}

void SPL_CodeGen::writeSectionText() {
    outfile<< "\tsection\t.text" << endl;
}

void SPL_CodeGen::writeSectionData() {
    outfile<< "\tsection\t.data" << endl;
}

void SPL_CodeGen::writeSectionTextSubroutine() {

}