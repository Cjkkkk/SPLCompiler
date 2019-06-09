//
// Created by DELL on 2019/4/30.
//

#ifndef SPLCOMPILER_SPL_CODEGEN_HPP
#define SPLCOMPILER_SPL_CODEGEN_HPP

#include <map>
#include <list>
#include <set>
#include <fstream>
#include "spl_IR.hpp"

typedef enum x86_reg {
    rax, rbx, rcx, rdx, rbp, rsp,
    rsi, rdi,
    r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15,
    not_in
} reg;

class SPL_CodeGen {
public:
    explicit SPL_CodeGen( SPL_IR* ir_): ir(ir_){
        outfile.open("assem/hello.asm",std::ios::out);
        callee_saved_registers = {
                rbp, rbx, r12, r13, r14, r15
        };
        reg_to_memory = {
                {rax, 0}, {rbx, 0}, {rcx, 0}, {rdx, 0}, {rsi, 0}, {rdi, 0},
                {r1, 0}, {r2, 0}, {r3, 0}, {r4, 0} , {r5, 0}, {r6, 0}, {r7, 0}, {r8, 0},
                {r9, 0}, {r10, 0}, {r11, 0}, {r12, 0}, {r13, 0}, {r14, 0} ,{r15, 0}
        };
        reg_order = {
                rax, rbx, rcx, rdx,
                rsi, rdi,
                r1, r2, r3, r4, r5, r6, r7, r8, r9, r10, r11, r12, r13, r14, r15
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

    void generatePlusAndMinus(Instruction* ins);
    void generateMulAndDivide(Instruction* ins);
    void generateAssign(Instruction* ins);
    void generateLogic(Instruction* ins);
    void generateIf(Instruction* ins);
    void generateNull(Instruction* ins);
    void generateRet(Instruction* ins);
    void generateGoto(Instruction* ins);
    void generateCall(Instruction* ins);
    int fetchStackVariable(std::string& variable);
    x86_reg loadLiteralToReg(int);
    std::string reg_to_string(x86_reg);
    x86_reg get_x86_reg();
    x86_reg bringToReg(Operand* operand);
    // 保存上一个函数的活动记录
    void push_rbp();
    void pop_rbp();

    // 提前分配好堆栈
    void allocateStack();
    void deallocateStack();

    int stack_size;

    // 将ir写入文件
    void x86Instruction(const string& label, const string& ins, const string& op1, const string& op2);

    // 中间代码
    SPL_IR* ir;

    // 输出的文件
    std::ofstream outfile;

    // 字符串字面量
    // std::map<std::string, Operand*> string_literals;

    std::map<std::string, std::string> string_literals;
    // 变量相对rbp的offset
    std::map<std::string, int> name_to_memory;

    // 变量是否在寄存器当中
    std::map<std::string, x86_reg> name_to_reg;

    // 寄存器对应的内存位置
    std::map<x86_reg, int> reg_to_memory;

    // 查找空闲寄存器的顺序
    std::vector<x86_reg> reg_order;
    // callee saved
    std::set<x86_reg> callee_saved_registers;
};


#endif //SPLCOMPILER_SPL_CODEGEN_HPP
