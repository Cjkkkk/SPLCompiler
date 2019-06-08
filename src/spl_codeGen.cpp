//
// Created by DELL on 2019/4/30.
//

#include <fstream>
#include "spl_codeGen.h"

void SPL_CodeGen::x86Instruction(const string& label, const string& ins, const string& op1, const string& op2) {
    if(label.empty()) {
        outfile << "\t" << ins;
    } else {
        outfile << label << ":\t" << ins;
    }
    if(!op2.empty()) {
        outfile << " " << op1 << ", " << op2 << "\n";
    } else {
        outfile << " " << op1 << "\n";
    }
}

void SPL_CodeGen::GenerateMachineCode() {
    ir->setToMain();
    writeDirectives("global", ir->getCurrentIR()[0]->label);
    writeDirectives("section", ".text");

    // 遍历所有函数
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
        writeSectionTextSubroutine();
    }

    writeStringLiteral();
    writeDirectives("section", ".data"); // initialized global variable
    writeDirectives("section", ".bss"); // uninitialized global variable
    writeSectionConstData();
}

void SPL_CodeGen::writeStringLiteral(){
    for(auto& str : string_literals) {
        x86Instruction(str.first, "", "", "");
        x86Instruction("", "db", "\"" + *str.second->value.valString + "\"", "");
    }
}


void SPL_CodeGen::writeDirectives(const std::string& instr, const std::string& op) {
    x86Instruction("", instr, op, "");
}


void SPL_CodeGen::writeSectionTextSubroutine() {
    prepare_rbp();
    allocateStack();
    vector<Instruction*>& instr = ir->getCurrentIR();
    for(auto ins : instr) {
        switch(ins->op) {
            case PLUS_:
            case MINUS_:
                generatePlusAndMinus(ins);
            case MUL_:
            case DIV_:
                generateMulAndDivide(ins);
                break;
            case OP_NULL:
                generateNull(ins);
                break;
            case OP_GOTO:
                generateGoto(ins);
                break;
            case OP_ASSIGN:
                generateAssign(ins);
                break;
            case OP_IF:
            case OP_IF_Z:
                generateIf(ins);
                break;
            case OP_RET:
                generateRet(ins);
                break;
            case OP_CALL:
                generateCall(ins);
                break;
            default:
                break;
        }
    }
    clean_rbp();
}


void SPL_CodeGen::generateIf(Instruction* ins) {
    if(checkInstructionOp(ins, OP_IF)) {
        x86Instruction(ins->label, "cmp", ins->arg1->name, "0x0");
        x86Instruction(ins->label, "jne", ins->res->name, "");
    } else {
        x86Instruction(ins->label, "cmp", ins->arg1->name, "0x0");
        x86Instruction(ins->label, "je", ins->res->name, "");
    }
}


void SPL_CodeGen::generatePlusAndMinus(Instruction *ins) {
    x86Instruction(ins->label, opTox86Ins(ins->op), ins->arg1->name, "");
}


void SPL_CodeGen::generateMulAndDivide(Instruction *ins) {
    // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    x86Instruction(ins->label, "mov", "eax", ins->arg1->name);
    x86Instruction(ins->label, "mov", "ebx", ins->arg2->name);
    x86Instruction(ins->label, opTox86Ins(ins->op), "ebx", "");
}

void SPL_CodeGen::generateAssign(Instruction *ins) {
    if(checkOperandClass(ins->arg1, CONST) && checkOperandIsLiteral(ins->arg1)
       && checkOperandType(ins->arg1, STRING)) {
        string_literals.insert({ins->res->name, ins->arg1});
    }
    x86Instruction(ins->label, "mov", ins->res->name, ins->arg1->name);
}

void SPL_CodeGen::generateLogic(Instruction* ins) {

}

void SPL_CodeGen::generateNull(Instruction* ins) {
    x86Instruction(ins->label, "", "", "");
}

void SPL_CodeGen::generateRet(Instruction* ins) {
    x86Instruction(ins->label, "ret", "", "");
}

void SPL_CodeGen::generateGoto(Instruction* ins) {
    x86Instruction(ins->label, "jmp", ins->res->name, "");
}


void SPL_CodeGen::generateCall(Instruction* ins) {
    x86Instruction(ins->label, "call", ins->arg1->name, "");
}

void SPL_CodeGen::writeSectionConstData() {

}

void SPL_CodeGen::prepare_rbp() {
    x86Instruction("", "push", "rbp", "");
    x86Instruction("", "mov", "rbp", "rsp");
}

void SPL_CodeGen::clean_rbp() {
    x86Instruction("", "pop", "rbp", "");
}
void SPL_CodeGen::allocateStack() {
    vector<Instruction*>& instr = ir->getCurrentIR();
    // 堆栈上的数据用offset索引
    uint16_t offset = 0;
    for(auto ins : instr) {
        if(ins->op == OP_ASSIGN) {
            // sizeof();
        }
    }
    // 在堆栈上分配空间
    x86Instruction("", "sub", "rsp", std::to_string(offset));
}
