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
        outfile << label << ":" << ins;
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
//    writeDirectives("section", ".bss"); // uninitialized global variable
//    writeSectionConstData();
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

    // 将临时变量的值存储位置记录下来
    name_to_reg.insert({ins->res->name, reg1});
}


void SPL_CodeGen::generateMulAndDivide(Instruction *ins) {
    // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    x86Instruction(ins->label, "mov", "eax", ins->arg1->name);
    x86Instruction(ins->label, "mov", "ebx", ins->arg2->name);
    x86Instruction(ins->label, opTox86Ins(ins->op), "ebx", "");
}

void SPL_CodeGen::generateAssign(Instruction *ins) {
    if( checkOperandClass(ins->arg1, KNOWN)) {
        x86Instruction(ins->label,
                "mov word",
                "[rbp - " + std::to_string(fetchStackVariable(ins->res->name)) + "]",
                std::to_string(ins->arg1->value.valInt));

//        if(checkOperandType(ins->arg1, STRING)) {
//            string_literals.insert({ins->res->name, ins->arg1});
//        }
    } else {
        // 查看变量是否在内存之间
        auto reg = bringToReg(ins->arg1);
        x86Instruction(ins->label,
                       "mov",
                       "[rbp - " + std::to_string(fetchStackVariable(ins->res->name)) + "]",
                       reg_to_string(reg));
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
    string_literals.insert({"format", "db  \"%20ld\", 10, 0"});
    // x86Instruction(ins->label, "call", ins->arg1->name, "");
}

void SPL_CodeGen::writeSectionConstData() {

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
        if((checkInstructionOp(ins, OP_ASSIGN) || isCalculateOp(ins->op))
        && checkOperandClass(ins->res, VAR)) {
            auto it = name_to_memory.find(ins->res->name);
            if(it == name_to_memory.end()) {
                // 还没有在内存中， 添加到内存
                offset = (offset / 4 + 1) * 4;
                auto size = ins->res->getSize();
                offset += size;
                std::cout << ins->res->name << " " << offset << "\n";
                // 初始化变量在堆栈上的位置
                name_to_memory.insert({ins->res->name, offset});
                // 初始化变量是否在寄存器中 not_in是一个特殊的寄存器代表变量不在寄存器中
                name_to_reg.insert({ins->res->name, not_in});
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
    for(auto it = reg_order.begin() ; it != reg_order.end() ; it ++) {
        auto reg_it = reg_to_memory.find(*it);
        if(reg_it->second == 0) {
            // 说明是空闲的
            return reg_it->first;
        }
    }
    // 将rax中的值存起来
    auto it = reg_to_memory.find(rax);
    if(it->second != -1) {
        //说明存的不是常量
        x86Instruction("", "mov", "[rbp - " + std::to_string(it->second) + "]", reg_to_string(rax));
    }

    // 返回rax
    return rax;
}


std::string SPL_CodeGen::reg_to_string(x86_reg reg) {
    switch(reg) {
        case rax:
            return "rax";
        case rbx:
            return "rbx";
        case rcx:
            return "rcx";
        case rdx:
            return "rdx";
        case rbp:
            return "rbp";
        case rsp:
            return "rsp";
        case rsi:
            return "rsi";
        case rdi:
            return "rdi";
        case r1:
            return "r1";
        case r2:
            return "r2";
        case r3:
            return "r3";
        case r4:
            return "r4";
        case r5:
            return "r5";
        case r6:
            return "r6";
        case r7:
            return "r7";
        case r8:
            return "r8";
        case r9:
            return "r9";
        case r10:
            return "r10";
        case r11:
            return "r11";
        case r12:
            return "r12";
        case r13:
            return "r13";
        case r14:
            return "r14";
        case r15:
            return "r15";
        default:
            return "not_in";
    }
}


int SPL_CodeGen::fetchStackVariable(std::string& variable) {
    auto memory_offset_it = name_to_memory.find(variable);
    if(memory_offset_it == name_to_memory.end()) {
        throw invalid_argument{"debug info > should at load in memory " + variable};
    }
    return memory_offset_it->second;
}


x86_reg SPL_CodeGen::loadLiteralToReg(int i) {
    auto reg = get_x86_reg();
    // 装载字面量到寄存器中
    x86Instruction("", "mov", reg_to_string(reg), std::to_string(i));
    reg_to_memory.find(reg)->second = -1;
    return reg;
};


x86_reg SPL_CodeGen::bringToReg(Operand* operand) {
    x86_reg reg = not_in;
    if(checkOperandClass(operand, KNOWN)) {
        // 是常量
        reg = loadLiteralToReg(operand->value.valInt);
    } else if(checkOperandClass(operand, VAR)) {
        auto exist_in_reg = name_to_reg.find(operand->name);
        if(exist_in_reg == name_to_reg.end()) {
            throw invalid_argument{"debug info > should at least not_in " + operand->name};
        }
        if(exist_in_reg->second == not_in) {
            // 加载变量到寄存器中
            reg = get_x86_reg();
            auto offset = fetchStackVariable(operand->name);
            string s = "[rbp - " + std::to_string(offset) + "]";
            x86Instruction("", "mov", reg_to_string(reg), s);
            // 添加变量保存在寄存器的记录
            exist_in_reg->second = reg;
            // 添加寄存器到内存的映射
            reg_to_memory.find(reg)->second = offset;
        }
        else {
            reg = exist_in_reg->second;
        }
    } else {
        auto it = name_to_reg.find(operand->name);
        if(it == name_to_reg.end()) {
            throw invalid_argument{"temp variable should be in reg " + operand->name};
        }
        return it->second;
    }

    return reg;
}