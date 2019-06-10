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
        outfile << ";";
        ins->output(outfile);
        switch(ins->op) {
            case PLUS_:
            case MINUS_:
                generatePlusAndMinus(ins);
                break;
            case MUL_:
            case DIV_:
                generateMulAndDivide(ins);
                break;
            case GT_:
            case GE_:
            case LE_:
            case LT_:
            case EQUAL_:
            case UNEQUAL_:
                generateCompare(ins);
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
            case OP_PARAM:
                generateParam(ins);
                break;
            case OP_CALL:
                generateCall(ins);
                break;
            default:
                break;
        }
    }
}


void SPL_CodeGen::generateCompare(Instruction* ins) {
    // 加载两个比较的操作数
    auto reg1 = bringToReg(ins->arg1);
    auto reg2 = bringToReg(ins->arg2);
    x86Instruction(ins->label, "cmp", reg_to_string(reg1), reg_to_string(reg2));

    // 释放reg因为不再需要了
    freeReg(reg1);
    freeReg(reg2);

    switch(ins->op) {
        case LE_:
            x86Instruction("", "setle", "al", "");
            break;
        default:
            break;
    }
    auto offset = fetchStackVariable(ins->res->name);
    x86Instruction("", "mov", "[ rbp - " + std::to_string(offset) + " ]", "al");
}

void SPL_CodeGen::generateIf(Instruction* ins) {
    auto reg = bringToReg(ins->arg1);
    if(checkInstructionOp(ins, OP_IF)) {
        x86Instruction(ins->label, "cmp", reg_to_string(reg), "0x1");
        x86Instruction("", "je", ins->res->name, "");
    } else {
        x86Instruction(ins->label, "cmp", reg_to_string(reg), "0x0");
        x86Instruction("", "je", ins->res->name, "");
    }
    freeReg(reg);
}


void SPL_CodeGen::generatePlusAndMinus(Instruction *ins) {
    //检查参数是否在寄存器中了
    x86_reg reg1 = bringToReg(ins->arg1);
    x86_reg reg2 = bringToReg(ins->arg2);
    if(ins->op == PLUS_)
        x86Instruction(ins->label, "add", reg_to_string(reg1), reg_to_string(reg2));
    if(ins->op == MINUS_)
        x86Instruction(ins->label, "sub", reg_to_string(reg1), reg_to_string(reg2));
    auto& reg_status = reg_memory_mapping.find(reg1)->second;
    reg_status.status = STACK;
    reg_status.offset = fetchStackVariable(ins->res->name);
    freeReg(reg1, true);
    freeReg(reg2);
}


void SPL_CodeGen::generateMulAndDivide(Instruction *ins) {
    // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    auto reg1 = bringToReg(ins->arg1, rax);
    auto reg2 = bringToReg(ins->arg2);
    if(ins->op == DIV_) {
        // rdx 应该为0
        loadLiteralToReg(0, rdx);
    }
    x86Instruction(ins->label, opTox86Ins(ins->op), reg_to_string(reg2), "");
    auto& reg_status = reg_memory_mapping.find(reg1)->second;
    reg_status.status = STACK;
    reg_status.offset = fetchStackVariable(ins->res->name);
    freeReg(reg1, true);
    freeReg(reg2);
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
        freeReg(reg);
    }
}

void SPL_CodeGen::generateLogic(Instruction* ins) {

}

void SPL_CodeGen::generateParam(Instruction* ins) {
    auto reg = getNextArgReg();
    if(reg == not_in) {
        // 参数寄存器不够用了
        throw invalid_argument{"reg for argument is not available now..."};
    }
    get_x86_reg(reg);
    bringToReg(ins->arg1, reg);
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
    x86Instruction(ins->label, "call", "printf", "");
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
                if(offset % 8 != 0) offset = (offset / 8 + 1) * 8;
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

int SPL_CodeGen::fetchStackVariable(std::string& variable) {
    auto memory_offset_it = name_to_stack.find(variable);
    if(memory_offset_it == name_to_stack.end()) {
        throw invalid_argument{"debug info > should at load in memory " + variable};
    }
    return memory_offset_it->second;
}


x86_reg SPL_CodeGen::getNextArgReg() {
    for(auto& reg: reg_arg) {
        if(reg.status) {
            // 可以使用这个寄存器作为参数传递
            reg.status = false;
            return reg.name;
        }
    }
    return not_in;
}


x86_reg SPL_CodeGen::loadLiteralToReg(int i, x86_reg reg) {
    if(reg == not_in) {
        reg = get_x86_reg();
    } else {
        freeReg(reg);
    }
    x86Instruction("", "mov", reg_to_string(reg), std::to_string(i));
    return reg;
}


x86_reg SPL_CodeGen::bringToReg(Operand* operand, x86_reg reg) {
    if(reg == not_in) reg = get_x86_reg(); // 需要一个空闲的寄存器
    else {
        freeReg(reg); // 需要指定的寄存器
    }
    if(checkOperandClass(operand, KNOWN)) {
        // 是常量
        if(checkOperandType(operand, INT)) {
            // 装载字面量到寄存器中
            x86Instruction("", "mov", reg_to_string(reg), std::to_string(operand->value.valInt));
        }
        if(checkOperandType(operand, STRING)) {
            auto temp_s = getTempStringLable();
            x86Instruction("", "mov", reg_to_string(reg), temp_s);
            string_literals.insert({temp_s, "db \"" +  *operand->value.valString +"\", 10, 0"});
        }
        // 记录该寄存器中存放的是常数
        reg_memory_mapping.find(reg)->second.status = LITERAL;
    } else if(checkOperandClass(operand, VAR)) {
        // 加载变量到寄存器中
        // 变量默认是全局的
        x86Instruction("", "mov", reg_to_string(reg), "[ " + operand->name + " ]");
        // 添加寄存器到内存的映射
        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = GLOBAL;
        reg_status.global = operand->name;
    } else {
        auto offset = fetchStackVariable(operand->name);
        x86Instruction("", "mov", reg_to_string(reg), "[ rbp - " + std::to_string(offset) + " ]");
        // 添加寄存器到内存的映射
        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = STACK;
        reg_status.offset = offset;
    }
    return reg;
}

/**
 * 获取寄存器
 * @param reg [x86_reg] 寄存器
 */

void SPL_CodeGen::get_x86_reg(x86_reg reg) {
    auto& reg_status = reg_memory_mapping.find(reg)->second;
    if (reg_status.status == GLOBAL || reg_status.status == STACK) {
        freeReg(reg);
    }
}

/**
 * 获取任意空闲的寄存器
 * @return reg [x86_reg] 空闲寄存器
 */

x86_reg SPL_CodeGen::get_x86_reg() {
    for(const auto& reg: reg_order) {
        auto& reg_status = reg_memory_mapping.find(reg.name)->second;
        if(reg_status.status == FREE) {
            // 说明是空闲的
            return reg.name;
        }
    }
    // 没有找到空闲的，那么开始轮班
    for(auto& reg: reg_order) {
        if(reg.counter) {
            // counter为true，征用这个寄存器，同时设为false，下次不会重复使用
            reg.counter = false;
            freeReg(reg.name);
            return reg.name;
        } else {
            reg.counter = true;
        }
    }
    return not_in;
}
/**
 * 释放寄存器
 * @param reg [x86_reg] 寄存器
 * @param write_back [bool] 是否写回堆栈或者全局变量
 */

void SPL_CodeGen::freeReg(x86_reg reg, bool write_back) {
    auto& reg_status = reg_memory_mapping.find(reg)->second;
    if(!write_back) {
        reg_status.status = FREE;
        return;
    }
    if(reg_status.status == STACK) {
        auto offset = reg_status.offset;
        x86Instruction("", "mov", "[ rbp - " + std::to_string(offset) + " ]", reg_to_string(reg));
    } else if(reg_status.status == GLOBAL) {
        auto name = reg_status.global;
        x86Instruction("", "mov", "[ " + name + " ]", reg_to_string(reg));
    } else if (reg_status.status == LITERAL) {

    } else {
        throw invalid_argument{"debug info> already free" + reg_to_string(reg)};
    }
    reg_status.status = FREE;
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

std::string SPL_CodeGen::getTempStringLable() {
    return "string_literal_" + std::to_string(temp_count++);
}