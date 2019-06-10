//
// Created by DELL on 2019/4/30.
//

#include <fstream>
#include "spl_codeGen.hpp"
#include "spl_compiler.hpp"
#include "spl_IR.hpp"

void SPL_CodeGen::x86Instruction(const string& label, const string& ins, const string& op1, const string& op2) {
    if(label.empty()) {
        outfile << "    " << ins;
    } else {
        outfile << label << ":    " << ins;
    }
    if(!op2.empty()) {
        outfile << "    " << op1 << ", " << op2;
    } else {
        outfile << "    " << op1;
    }
    outfile << "\n";
}

void SPL_CodeGen::GenerateMachineCode() {
    ir->setToMain();
    ir->getCurrentIR()[0]->label = "main";
    writeDirectives("global", ir->getCurrentIR()[0]->label);
    //writeDirectives("global", "main");
    writeDirectives("section", ".text");
    writeDirectives("extern", "printf");
    // 遍历所有函数
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
        writeSectionTextSubroutine();
    }

    writeStringLiteral();
//    writeDirectives("section", ".data"); // initialized global variable
    writeDirectives("section", ".bss"); // uninitialized global variable
    writeSectionConstData();
}

void SPL_CodeGen::writeStringLiteral(){
    for(auto& str : string_literals) {
        x86Instruction(str.first, "", "", "");
        x86Instruction("", str.second, "", "");
    }
}


void SPL_CodeGen::writeDirectives(const std::string& instr, const std::string& op) {
    x86Instruction("", instr, op, "");
}


void SPL_CodeGen::writeSectionTextSubroutine() {
    vector<Instruction*>& instr = ir->getCurrentIR();
    // 打出函数名
    x86Instruction((*instr.begin())->label, "", "", "");
    push_rbp();
    allocateStack();
    for(auto it = instr.begin() + 1; it != instr.end(); it++) {
        auto ins = *it;
        switch(ins->op) {
            case PLUS_:
            case MINUS_:
                generatePlusAndMinus(ins);
                break;
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
    //检查参数是否在寄存器中了
    x86_reg reg1 = bringToReg(ins->arg1);
    x86_reg reg2 = bringToReg(ins->arg2);
    x86Instruction("", "add", reg_to_string(reg1), reg_to_string(reg2));
    reg_available.find(reg1)->second = STACK;
    reg_to_stack.find(reg1)->second = fetchStackVariable(ins->res->name);
    freeReg(reg1, true);
    freeReg(reg2, false);
}


void SPL_CodeGen::generateMulAndDivide(Instruction *ins) {
    // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    x86Instruction(ins->label, "mov", "eax", ins->arg1->name);
    x86Instruction(ins->label, "mov", "ebx", ins->arg2->name);
    x86Instruction(ins->label, opTox86Ins(ins->op), "ebx", "");
}

void SPL_CodeGen::generateAssign(Instruction *ins) {
    // x = 3 或者 x = y
    if( checkOperandClass(ins->arg1, KNOWN)) {
        auto size = ins->arg1->getSize();
        x86Instruction(ins->label,
                    "mov " + x86SizeToString(size),
                    "[ " + ins->res->name + " ]",
                    std::to_string(ins->arg1->value.valInt));
    } else {
        // 把变量移动到寄存器中
        auto reg = bringToReg(ins->arg1);
        x86Instruction(ins->label,
                       "mov",
                       "[ " + ins->res->name + " ]",
                       reg_to_string(reg));
        freeReg(reg, false);
    }
}

void SPL_CodeGen::generateLogic(Instruction* ins) {

}

void SPL_CodeGen::generateNull(Instruction* ins) {
    x86Instruction(ins->label, "", "", "");
}

void SPL_CodeGen::generateRet(Instruction* ins) {
    deallocateStack();
    pop_rbp();
    x86Instruction(ins->label, "ret", "", "");
}

void SPL_CodeGen::generateGoto(Instruction* ins) {
    x86Instruction(ins->label, "jmp", ins->res->name, "");
}


void SPL_CodeGen::generateCall(Instruction* ins) {
    x86Instruction("", "mov", "rdi", "format");
    x86Instruction("", "mov", "rsi", "rdx");
    x86Instruction(ins->label, "call", "printf", "");
    string_literals.insert({"format", "db  \"%ld\", 10, 0"});
}

void SPL_CodeGen::writeSectionConstData() {
    for(auto& data: bss_data) {
        x86Instruction(data.first, x86SizeToBssInit(data.second.first, data.second.second), "", "");
    }
}

void SPL_CodeGen::push_rbp() {
    x86Instruction("", "push", "rbp", "");
    x86Instruction("", "mov", "rbp", "rsp");
}

void SPL_CodeGen::pop_rbp() {
    x86Instruction("", "pop", "rbp", "");
}
void SPL_CodeGen::allocateStack() {
    vector<Instruction*>& instr = ir->getCurrentIR();
    // 堆栈上的数据用offset索引
    uint16_t offset = 0;
    for(auto ins : instr) {
        collect_bss_data(ins);
        if(isCalculateOp(ins->op)) {
            // 现在栈上把临时变量的位置留出来
            auto it = name_to_stack.find(ins->res->name);
            if(it == name_to_stack.end()) {
                // 还没有在内存中， 添加到内存
                if(offset % 4 != 0) offset = (offset / 4 + 1) * 4;
                auto size = ins->res->getSize();
                offset += size;
                std::cout << ins->res->name << " " << offset << "\n";
                // 初始化变量在堆栈上的位置
                name_to_stack.insert({ins->res->name, offset});
            }
        }
    }
    offset = (offset / 16 + 1) * 16;
    stack_size = offset;
    // 在堆栈上分配空间

    x86Instruction("", "sub", "rsp", std::to_string(stack_size));
}

void SPL_CodeGen::deallocateStack() {
    x86Instruction("", "add", "rsp", std::to_string(stack_size));
    stack_size = 0;
}

x86_reg SPL_CodeGen::get_x86_reg() {
    for(auto reg: reg_order) {
        auto reg_it = reg_available.find(reg);
        if(reg_it->second == FREE) {
            // 说明是空闲的
            return reg;
        }
    }
    return r1;
}


int SPL_CodeGen::fetchStackVariable(std::string& variable) {
    auto memory_offset_it = name_to_stack.find(variable);
    if(memory_offset_it == name_to_stack.end()) {
        throw invalid_argument{"debug info > should at load in memory " + variable};
    }
    return memory_offset_it->second;
}


x86_reg SPL_CodeGen::loadLiteralToReg(int i) {
    auto reg = get_x86_reg();
    // 装载字面量到寄存器中
    x86Instruction("", "mov", reg_to_string(reg), std::to_string(i));

    // 记录该寄存器中存放的是常数
    reg_available.find(reg)->second = LITERAL;
    return reg;
}


x86_reg SPL_CodeGen::bringToReg(Operand* operand) {
    x86_reg reg;
    if(checkOperandClass(operand, KNOWN)) {
        // 是常量
        reg = loadLiteralToReg(operand->value.valInt);
    } else if(checkOperandClass(operand, VAR)) {
        // 加载变量到寄存器中
        // 变量默认是全局的
        reg = get_x86_reg();
        x86Instruction("", "mov", reg_to_string(reg), "[ " + operand->name + " ]");
        // 添加寄存器到内存的映射
        reg_available.find(reg)->second = GLOBAL;
        reg_to_global.find(reg)->second = operand->name;
    } else {
        reg = get_x86_reg();
        auto offset = fetchStackVariable(operand->name);
        x86Instruction("", "mov", reg_to_string(reg), "[ rbp - " + std::to_string(offset) + " ]");
        // 添加寄存器到内存的映射
        reg_available.find(reg)->second = STACK;
        reg_to_stack.find(reg)->second = offset;
    }
    return reg;
}

/**
 * 获取寄存器
 * @param reg [x86_reg] 寄存器
 */

void SPL_CodeGen::get_x86_reg(x86_reg reg) {

}

/**
 * 释放寄存器
 * @param reg [x86_reg] 寄存器
 * @param write_back [bool] 是否写回堆栈或者全局变量
 */

void SPL_CodeGen::freeReg(x86_reg reg, bool write_back) {
    if(!write_back) {
        reg_available.find(reg)->second = FREE;
        return;
    }
    auto it = reg_available.find(reg);
    if(it->second == STACK) {
        auto offset = reg_to_stack.find(reg)->second;
        x86Instruction("", "mov", "[ rbp - " + std::to_string(offset) + " ]", reg_to_string(reg));
    } else if(it->second == GLOBAL) {
        auto name = reg_to_global.find(reg)->second;
        x86Instruction("", "mov", "[ " + name + " ]", reg_to_string(reg));
    } else if (it->second == LITERAL) {

    } else {
        throw invalid_argument{"debug info> already free" + reg_to_string(reg)};
    }
    it->second = FREE;
}


void SPL_CodeGen::collect_bss_data(Instruction* ins) {
    if(ins->arg1 && checkOperandClass(ins->arg1, VAR)) {
        auto it = bss_data.find(ins->arg1->name);
        if(it == bss_data.end()) bss_data.insert({ins->arg1->name, {ins->arg1->getSize(), 1}});
    }
    if(ins->arg2 && checkOperandClass(ins->arg2, VAR)) {
        auto it = bss_data.find(ins->arg2->name);
        if(it == bss_data.end()) bss_data.insert({ins->arg2->name, {ins->arg2->getSize(),1}});
    }
    if(ins->res && checkOperandClass(ins->res, VAR)) {
        auto it = bss_data.find(ins->res->name);
        if(it == bss_data.end()) bss_data.insert({ins->res->name, {ins->res->getSize(), 1}});
    }
}