//
// Created by DELL on 2019/4/30.
//

#include <fstream>
#include "spl_codeGen.h"

void SPL_CodeGen::GenerateMachineCode() {
    ir->setToMain();
    writeDirectives("global", ir->getCurrentIR()[0]->label);
    writeDirectives("section", ".text");
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
        writeSectionTextSubroutine();
        outfile << "\n";
        // collectConstData();
    }
    writeStringLiteral();
    writeDirectives("section", ".data"); // initialized global variable
    writeDirectives("section", ".bss"); // uninitialized global variable
    writeSectionConstData();
}

void SPL_CodeGen::writeStringLiteral(){
    for(auto& ins : StringLiteral) {
        outfile << ins->res->name << ":\n";
        outfile << "\t" << "db" << "\n";
    }
}
void SPL_CodeGen::writeDirectives(const std::string& instr, const std::string& op) {
    outfile << "\t" << instr <<"\t" << op << endl;
}

std::string opTox86Ins(SPL_OP op) {
    switch(op) {
        case PLUS_:
            return "add";
        case MINUS_:
            return "sub";
        case MUL_:
            return "mul";
        case DIV_:
            return "div";
        default:
            return "error";
    }
}

void SPL_CodeGen::writeSectionTextSubroutine() {
    vector<Instruction*>& instr = ir->getCurrentIR();
    for(auto ins : instr) {
        switch(ins->op) {
            case PLUS_:
            case MINUS_:
                outfile << "\t" << opTox86Ins(ins->op) << "\t" << ins->arg1->name << "\n";
            case MUL_:
            case DIV_:
                // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
                // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
                outfile << "\t" << "mov" << "\t" << "eax" << ins->arg1->name << "\n";
                outfile << "\t" << "mov" << "\t" << "ebx" << ins->arg2->name << "\n";
                outfile << "\t" << opTox86Ins(ins->op) << "\t" << "ebx" << "\n";
                break;
            case OP_NULL:
                outfile << ins->label <<"\n";
                break;
            case OP_GOTO:
                outfile << ins->label << "\t" << "jmp" << "\t" << ins->res->name << "\n";
                break;
            case OP_ASSIGN:
                if(checkOperandClass(ins->arg1, CONST) && checkOperandIsLiteral(ins->arg1)
                    && checkOperandType(ins->arg1, STRING)) {
                    StringLiteral.push_back(ins);
                }
                outfile << ins->label << "\t" << "mov" << "\t" << ins->res->name << "\t"<< ins->arg1->name <<"\n";
                break;
            case OP_IF:
                outfile << ins->label << "\t" << "cmp" << "\t" << ins->arg1->name << "\t"<< "0x0\n";
                outfile << ins->label << "\t" << "jne" << "\t" << ins->res->name << "\n";
                break;
            case OP_IF_Z:
                outfile << ins->label << "\t" << "cmp" << "\t" << ins->arg1->name << "\t"<< "0x0\n";
                outfile << ins->label << "\t" << "je" << "\t" << ins->res->name << "\n";
                break;
            case OP_RET:
                outfile << ins->label << "\t" << "ret" << "\n";
                break;
            default:
                break;
        }
    }
}

void SPL_CodeGen::writeSectionConstData() {

}