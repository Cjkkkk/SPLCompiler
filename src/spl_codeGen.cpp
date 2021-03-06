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
    writeDirectives("global", "main");
    writeDirectives("section", ".text");
    writeDirectives("extern", "printf");
    writeDirectives("extern", "scanf");
    // 遍历所有函数
    for(auto index = ir->getIRSetSize() - 1; index >= 0 ; index--) {
        ir->setCurrent(index);
        current_function_name = ir->getCurrentIR()[0]->label;
        std::cout << current_function_name << "\n" ;
        writeSectionTextSubroutine();
        param.clear();
        nth_param = 1;
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
            case AND_:
            case OR_:
                generatePlusAndMinus(ins);
                break;
            case MUL_:
            case DIV_:
            case MOD_:
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
                std::cout << "count 1\n";
                generateCall(ins);
                break;
            case OP_FUNC_PARAM:
                generateGetParam(ins);
            default:
                break;
        }
    }
}


void SPL_CodeGen::generateGetParam(Instruction* ins) {
    if(nth_param <= 6) {
        auto offset = fetchStackVariable(ins->arg1->name);
        x86Instruction(ins->label, "mov", "[ rbp - " + std::to_string(offset) + " ]", reg_to_string(parm_reg_mapping[nth_param-1]));
    } else {

    }
    nth_param += 1;
}

void SPL_CodeGen::generateCompare(Instruction* ins) {
    // 加载两个比较的操作数
    x86_reg reg1, reg2;
    is_param_ref(ins->arg1->name) ? reg1 = bringToReg(ins->arg1, not_in, true, false) : reg1 = bringToReg(ins->arg1, not_in, false, false);
    is_param_ref(ins->arg2->name) ? reg2 = bringToReg(ins->arg2, not_in, true, false) : reg2 = bringToReg(ins->arg2, not_in, false, false);
    x86Instruction(ins->label, "cmp", reg_to_string(reg1), reg_to_string(reg2));

    // 释放reg因为不再需要了
    freeReg(reg1, false, false);
    freeReg(reg2, false, false);

    auto offset = fetchStackVariable(ins->res->name);
    switch(ins->op) {
        case LE_:
            x86Instruction("", "setle", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        case LT_:
            x86Instruction("", "setl", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        case GE_:
            x86Instruction("", "setge", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        case GT_:
            x86Instruction("", "setg", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        case EQUAL_:
            x86Instruction("", "sete", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        case UNEQUAL_:
            x86Instruction("", "setne", "[ rbp - " + std::to_string(offset) + " ]", "");
            break;
        default:
            break;
    }
}

void SPL_CodeGen::generateIf(Instruction* ins) {
    x86_reg reg;
    is_param_ref(ins->arg1->name) ? reg = bringToReg(ins->arg1, not_in, true, false) : reg = bringToReg(ins->arg1, not_in, false, false);

    if(checkInstructionOp(ins, OP_IF)) {
        x86Instruction(ins->label, "cmp", reg_to_string(reg), "0x1");
        x86Instruction("", "je", ins->res->name, "");
    } else {
        x86Instruction(ins->label, "cmp", reg_to_string(reg), "0x0");
        x86Instruction("", "je", ins->res->name, "");
    }
    freeReg(reg, false, false);
}


void SPL_CodeGen::generatePlusAndMinus(Instruction *ins) {
    //检查参数是否在寄存器中了
    x86_reg reg1, reg2;
    is_param_ref(ins->arg1->name) ? reg1 = bringToReg(ins->arg1, not_in, true, false) : reg1 = bringToReg(ins->arg1, not_in, false, false);
    is_param_ref(ins->arg2->name) ? reg2 = bringToReg(ins->arg2, not_in, true, false) : reg2 = bringToReg(ins->arg2, not_in, false, false);

    x86Instruction(ins->label, opTox86Ins(ins->op), reg_to_string(reg1), reg_to_string(reg2));

    auto& reg_status = reg_memory_mapping.find(reg1)->second;
    reg_status.status = STACK;
    reg_status.offset = fetchStackVariable(ins->res->name);
    reg_status.operand = ins->res;

    freeReg(reg1, true, false); // load back to memory
    freeReg(reg2, false, false);
}


void SPL_CodeGen::generateMulAndDivide(Instruction *ins) {
    // mul的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    // div的一个乘数在eax中， 只接受一个参数， 结果放在eax中
    x86_reg reg1, reg2;
    is_param_ref(ins->arg1->name) ? reg1 = bringToReg(ins->arg1, rax, true, false) : reg1 = bringToReg(ins->arg1, rax, false, false);
    is_param_ref(ins->arg2->name) ? reg2 = bringToReg(ins->arg2, not_in, true, false) : reg2 = bringToReg(ins->arg2, not_in, false, false);

    if(ins->op == DIV_ || ins->op == MOD_) {
        // edx 应该为0
        loadLiteralToReg(0, rdx);
    }
    x86Instruction(ins->label, opTox86Ins(ins->op), reg_to_string(reg2), "");

    if(ins->op == MOD_) {
        // 余数保存在edx寄存器中
        freeReg(rax, false, false);
        reg1 = rdx;
    }

    auto& reg_status = reg_memory_mapping.find(reg1)->second;
    reg_status.status = STACK;
    reg_status.offset = fetchStackVariable(ins->res->name);
    reg_status.operand = ins->res;

    freeReg(reg1, true, false);

    freeReg(reg2, false, false);
}

/**
 *
 * @param ins [Instruction* ] 中间码
 * 赋值操作的右边是立即数的时候，直接移动立即数到内存
 * 赋值操作的右边是变量的时候，先加载变量到寄存器中
 */
void SPL_CodeGen::generateAssign(Instruction *ins) {
    // x = 3 或者 x = y
    x86_reg reg;
    if(isReturnVariable(ins->res)) {
        // 说明是函数返回值
        is_param_ref(ins->arg1->name) ? reg = bringToReg(ins->arg1, rax, true, false) : reg = bringToReg(ins->arg1, rax, false, false);

        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = STACK;
        reg_status.offset = fetchStackVariable(ins->res->name);
        reg_status.operand = ins->res;
        freeReg(reg, true, false);
    } else if(isParam(ins->res)) {

        // 说明是函数的参数
        is_param_ref(ins->arg1->name) ? reg = bringToReg(ins->arg1, not_in, true, false) : reg = bringToReg(ins->arg1, not_in, false, false);

        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = STACK;
        reg_status.offset = fetchStackVariable(ins->res->name);
        reg_status.operand = ins->res;
        if(is_param_ref(ins->res->name)) {
            freeReg(reg, true, true);
        } else {
            freeReg(reg, false, false);
        }

    } else {
        // 说明是全局变量
        reg = bringToReg(ins->arg1);
        // 是全局变量
        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = GLOBAL;
        reg_status.global = ins->res->name;
        reg_status.operand = ins->res;
        freeReg(reg, true, false);
    }
}

void SPL_CodeGen::generateLogic(Instruction* ins) {

}

void SPL_CodeGen::generateParam(Instruction* ins) {
    param_stack.push_back(ins->arg1);
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
    auto arg_list = ins->arg1->symbol == nullptr ? nullptr : ins->arg1->symbol->subSymbolList;

    for(auto index = 0 ; index < param_stack.size() ; index ++ ) {
        auto reg = getNextArgReg();
        if(reg == not_in) {
            // 参数寄存器不够用了
            throw invalid_argument{"reg for argument is not available now..."};
        } else {
            // 参数是引用
            if(arg_list && arg_list->at(index)->paraType == REFER) {
                // 需要引用类型
                reg = is_param_ref(param_stack[index]->name) ? bringToReg(param_stack[index], reg, true, true) : bringToReg(param_stack[index], reg,
                                                                                                                            false,true);
            } else {
                // 不需要引用类型
                reg = is_param_ref(param_stack[index]->name) ? bringToReg(param_stack[index], reg, true, false) : bringToReg(param_stack[index], reg,
                                                                                                                             false, false);
            }
        }
    }
    if(ins->arg1->name == "write.0") x86Instruction(ins->label, "call", "printf", "");
    else if (ins->arg1->name == "read.0") {
        x86Instruction(ins->label, "call", "scanf", "");
    }
    else {
        x86Instruction(ins->label, "call", ins->arg1->name, "");
    }
    if(ins->res) {
        // 有返回值
        auto& reg_status = reg_memory_mapping.find(rax)->second;
        reg_status.status = STACK;
        reg_status.offset = fetchStackVariable(ins->res->name);
        reg_status.operand = ins->res;
        is_param_ref(ins->res->name) ? freeReg(rax, true, true) :  freeReg(rax, true, false);
    }
    // 释放所有参数寄存器的使用权
    free_arg();

    // 清空所有参数
    param_stack.clear();
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
        // 临时变量放在栈上
        if(isCalculateOp(ins->op) || (checkInstructionOp(ins, OP_CALL) && ins->res)) {
            // 现在栈上把临时变量的位置留出来
            auto it = name_to_stack.find(ins->res->name);
            if(it == name_to_stack.end()) {
                offset += 8;
                std::cout << ins->res->name << " " << offset << "\n";
                // 初始化变量在堆栈上的位置
                name_to_stack.insert({ins->res->name, offset});
            }
        }

        // 函数参数/返回值也放在栈上
        if(checkInstructionOp(ins, OP_FUNC_PARAM) || checkInstructionOp(ins, OP_FUNC_RET)) {
            if(checkInstructionOp(ins, OP_FUNC_PARAM)) {
                std::cout << "collect " << ins->arg1->name << "\n";
                param.insert({ins->arg1->name, ins->arg1});
            }
            // 现在栈上把临时变量的位置留出来
            auto it = name_to_stack.find(ins->arg1->name);
            if(it == name_to_stack.end()) {
                offset += 8;
                // 初始化变量在堆栈上的位置
                name_to_stack.insert({ins->arg1->name, offset});
            }
        }

        collect_bss_data(ins->arg1);
        collect_bss_data(ins->arg2);
        collect_bss_data(ins->res);
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


x86_reg SPL_CodeGen::loadLiteralToReg(int i, x86_reg reg) {
    if(reg == not_in) {
        reg = get_x86_reg();
    } else {
        freeReg(reg);
    }
    x86Instruction("", "mov", reg_to_string(reg), std::to_string(i));
    return reg;
}


x86_reg SPL_CodeGen::bringToReg(Operand* operand, x86_reg reg, bool is_ref, bool want_ref) {
    if(reg == not_in) reg = get_x86_reg(); // 需要一个空闲的寄存器
    else {
        get_x86_reg(reg); // 需要指定的寄存器
    }
    if(checkOperandClass(operand, KNOWN)) {
        // 是常量
        if(checkOperandType(operand, INT)) {
            // 装载字面量到寄存器中
            x86Instruction("", "mov", reg_to_string(reg), std::to_string(operand->value.valInt));
        }
        if(checkOperandType(operand, BOOL)) {
            int b = operand->value.valBool ? 1 : 0;
            x86Instruction("", "mov", reg_to_string(reg), std::to_string(b));
        }
        if(checkOperandType(operand, CHAR)) {
            x86Instruction("", "mov", reg_to_string(reg), std::to_string(operand->value.valChar));
        }
        if(checkOperandType(operand, STRING)) {
            auto temp_s = getTempStringLable();
            x86Instruction("", "mov", reg_to_string(reg), temp_s);
            string_literals.insert({temp_s, "db \"" +  *operand->value.valString +"\", 10, 0"});
        }
        // 记录该寄存器中存放的是常数
        reg_memory_mapping.find(reg)->second.status = LITERAL;
    } else if(checkOperandClass(operand, VAR) && !isParam(operand) && !isReturnVariable(operand)) {
        // 加载变量到寄存器中
        // 变量默认是全局的
        // 提供的变量是引用， 加载这个引用的地址
        if (!is_ref && want_ref)  {
            // 提供的变量是值， 加载引用
            x86Instruction("", "lea", reg_to_string(reg), " [ " + operand->name + " ]");
        } else if (is_ref && ! want_ref)  {
            // 提供的是引用， 加载值
            // 暂时锁住之前用的寄存器
            auto& reg_status = reg_memory_mapping.find(reg)->second;
            reg_status.status = STACK;
            auto r = get_x86_reg();

            x86Instruction("", "mov", reg_to_string(r), " [ " + operand->name + " ]");

            x86Instruction("", "mov", reg_to_string(reg), " [ " + reg_to_string(r) + " ]");
        } else{
            // 提供的是值， 加载值

            if(operand->type == ARRAY) {
                if(operand->offset->cl == KNOWN) {
                    x86Instruction(
                            "",
                            "mov",
                            reg_to_string(reg),
                            " [ " + operand->name + " + 8 * " + std::to_string(operand->offset->value.valInt) + " ]");
                } else {
                    // 先锁住当前持有的寄存器
                    auto& reg_status = reg_memory_mapping.find(reg)->second;
                    reg_status.status = STACK;


                    auto r = get_x86_reg();
                    bringToReg(operand->offset, r);
                    x86Instruction(
                            "",
                            "mov",
                            reg_to_string(reg),
                            " [ " + operand->name + " + 8 * " + reg_to_string(r) + " ]"
                            );
                    freeReg(r, false, false);
                }
            } else if(operand->type == RECORD) {
                // 先锁住当前持有的寄存器
                auto members = operand->symbol->subSymbolList;
                int offset = 0;
                for(auto i = 0 ; i < members->size() ; i ++ ) {
                    if(members->at(i)->name == operand->offset->name) offset = i;
                }
                x86Instruction(
                        "",
                        "mov",
                        reg_to_string(reg),
                        " [ " + operand->name + " + 8 * " + std::to_string(offset) + " ]"
                );
            }
            else {
                x86Instruction("", "mov", reg_to_string(reg), " [ " + operand->name + " ]");
            }
        }

        // 添加寄存器到内存的映射
        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = GLOBAL;
        reg_status.global = operand->name;
        reg_status.operand = operand;
    } else {
        // 参数和局部变量都放这里
        auto offset = fetchStackVariable(operand->name);

        if( !is_ref && want_ref){
            // 不是引用，加载引用
            x86Instruction("", "lea", reg_to_string(reg), " [ rbp - " + std::to_string(offset) + " ]");
        } else if( is_ref && ! want_ref){
            // 是引用， 但是加载值
            // 暂时锁住之前用的寄存器
            auto& reg_status = reg_memory_mapping.find(reg)->second;
            reg_status.status = STACK;

            auto r = get_x86_reg();
            x86Instruction("", "mov", reg_to_string(r), " [ rbp - " + std::to_string(offset) + " ]");

            x86Instruction("", "mov", reg_to_string(reg), "[ " + reg_to_string(r) + " ]");
        } else {
            // 两者相同
            x86_size size;
            string mov;
            if (!is_ref && ! want_ref) {
                size = operand->getSize();
                switch(size) {
                    case qword:
                        mov = "mov";
                        break;
                    case dword:
                        mov = "movsx";
                        break;
                    default:
                        mov = "movzx";
                }
                x86Instruction("", mov, reg_to_string(reg), x86SizeToString(size) + " [ rbp - " + std::to_string(offset) + " ]");
            } else {
                x86Instruction("", "mov", reg_to_string(reg), " [ rbp - " + std::to_string(offset) + " ]");
            }
        }
        // 添加寄存器到内存的映射
        auto& reg_status = reg_memory_mapping.find(reg)->second;
        reg_status.status = STACK;
        reg_status.offset = offset;
        reg_status.operand = operand;
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
        throw invalid_argument{"debug info: it's impossible to have register occupied. " + reg_to_string(reg)};
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
    // 理论上一定可以在通用寄存器中找到空闲的
    throw invalid_argument{"debug info: can not find a free general register"};
    // 没有找到空闲的，那么开始轮班
//    for(auto& reg: reg_order) {
//        if(reg.counter) {
//            // counter为true，征用这个寄存器，同时设为false，下次不会重复使用
//            reg.counter = false;
//            freeReg(reg.name);
//            return reg.name;
//        } else {
//            reg.counter = true;
//        }
//    }
//    return not_in;
}
/**
 * 释放寄存器
 * @param reg [x86_reg] 寄存器
 * @param write_back [bool] 是否写回堆栈或者全局变量
 */

void SPL_CodeGen::freeReg(x86_reg reg, bool write_back, bool write_reference) {
    auto& reg_status = reg_memory_mapping.find(reg)->second;
    // 单纯释放
    if(!write_back) {
        reg_status.status = FREE;
        return;
    }

    // 需要写回内存
    if(reg_status.status == STACK) {
        auto offset = reg_status.offset;
        if( write_reference ) {
            auto r = get_x86_reg();
            // 加载地址到寄存器
            x86Instruction("", "mov", reg_to_string(r), "[ rbp - " + std::to_string(offset) + " ]");
            // 按照地址写回
            x86Instruction("", "mov", "[ " + reg_to_string(r) + " ]", reg_to_string(reg));
            freeReg(r);
        } else {
            x86Instruction("", "mov", "[ rbp - " + std::to_string(offset) + " ]", reg_to_string(reg));
        }
    } else if(reg_status.status == GLOBAL) {
        auto name = reg_status.global;
        if( write_reference ) {
            auto r = get_x86_reg();
            x86Instruction("", "mov", reg_to_string(r), "[ " + name + " ]");
            x86Instruction("", "mov", "[ " + reg_to_string(r) + " ]", reg_to_string(reg));
            freeReg(r);
        } else {
            if(reg_status.operand->type == ARRAY) {
                if(reg_status.operand->offset->cl == KNOWN) {
                    x86Instruction(
                            "",
                            "mov",
                            " [ " + reg_status.operand->name + " + 8 * " + std::to_string(reg_status.operand->offset->value.valInt) + " ]",
                            reg_to_string(reg));
                } else {
                    auto r = get_x86_reg();
                    bringToReg(reg_status.operand->offset, r);
                    x86Instruction(
                            "",
                            "mov",
                            " [ " + reg_status.operand->name + " + 8 * " + reg_to_string(r) + " ]",
                            reg_to_string(reg));
                    freeReg(r, false, false);
                }
            } else if(reg_status.operand->type == RECORD) {
                auto members = reg_status.operand->symbol->subSymbolList;
                int offset = 0;
                for(auto i = 0 ; i < members->size() ; i ++ ) {
                    if(members->at(i)->name == reg_status.operand->offset->name) offset = i;
                }
                x86Instruction(
                        "",
                        "mov",
                        " [ " + reg_status.operand->name + " + 8 * " +std::to_string(offset) + " ]",
                        reg_to_string(reg));
            }
            else {
                x86Instruction("", "mov", "[ " + name + " ]", reg_to_string(reg));
            }
        }
    } else if (reg_status.status == LITERAL) {

    } else {
        throw invalid_argument{"debug info> already free" + reg_to_string(reg)};
    }
    reg_status.status = FREE;
}


void SPL_CodeGen::collect_bss_data(Operand* op) {
    if(op && checkOperandClass(op, VAR) && !isParam(op) && !isReturnVariable(op)) {
        auto it = bss_data.find(op->name);
        if(it == bss_data.end()) {
            if(op->type == ARRAY) {
                bss_data.insert({op->name, {qword, op->symbol->scalarSize}});
            } else if(op->type == RECORD) {
                bss_data.insert({op->name, {qword, op->symbol->subSymbolList->size()}});
            } else {
                bss_data.insert({op->name, {qword, 1}});
            }
        }
    }
}


void SPL_CodeGen::collect_ronly_data(Instruction* ins) {
    if(ins->arg1 && checkOperandClass(ins->arg1, CONST)) {
        auto it = ronly_data.find(ins->arg1->name);
        if(it == ronly_data.end()) ronly_data.insert({ins->arg1->name, {ins->arg1->getSize(), 1}});
    }
    if(ins->arg2 && checkOperandClass(ins->arg2, CONST)) {
        auto it = ronly_data.find(ins->arg2->name);
        if(it == ronly_data.end()) ronly_data.insert({ins->arg2->name, {ins->arg2->getSize(),1}});
    }
    if(ins->res && checkOperandClass(ins->res, CONST)) {
        auto it = ronly_data.find(ins->res->name);
        if(it == ronly_data.end()) ronly_data.insert({ins->res->name, {ins->res->getSize(), 1}});
    }
}


std::string SPL_CodeGen::getTempStringLable() {
    return "string_literal_" + std::to_string(temp_count++);
}


x86_reg SPL_CodeGen::getNextArgReg() {
    for(auto& reg: reg_arg) {
        if(reg.status) {
            // 可以使用这个寄存器作为参数传递
            reg.status = false; // 这个寄存器被占用了
            return reg.name;
        }
    }
    return not_in;
}


void SPL_CodeGen::free_arg() {
    for(auto& arg: reg_arg) {
        arg.status = true;
        freeReg(arg.name);
    }
}


bool SPL_CodeGen::isParam(Operand* operand) {
    return param.find(operand->name) != param.end();
}


bool SPL_CodeGen::isReturnVariable(Operand* operand) {
    return current_function_name == operand->name;
}

bool SPL_CodeGen::is_param_ref(string& name) {
    auto it = param.find(name);
    return it != param.end() && it->second->symbol->paraType == REFER;
}

/*
 * 在寄存器之间移动
 */

void SPL_CodeGen::moveBetweenReg(x86_reg reg1, x86_reg reg2) {
    x86Instruction("", "mov", reg_to_string(reg1), reg_to_string(reg2));
}