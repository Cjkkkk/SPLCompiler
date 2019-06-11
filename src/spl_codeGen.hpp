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
#include "spl_compiler.hpp"
typedef enum reg_status {
    FREE,
    STACK,
    GLOBAL,
    LITERAL
} reg_status;


struct reg_memory {
    reg_status status;
    int offset;
    string global;
};

struct reg_order {
    reg name;
    bool counter;
};

struct reg_arg {
    reg name;
    bool status;
};

class SPL_CodeGen {
public:
    explicit SPL_CodeGen( SPL_IR* ir_): ir(ir_){
        outfile.open("assem/hello.asm",std::ios::out);
        callee_saved_registers = {
                rbp, ebx, r12d, r13d, r14d, r15d
        };

        reg_order = {
                {r10d, true} ,{r11d, true} ,{r12d, true} ,{r13d, true}, {r14d, true}, {r15d, true},
                {eax, true}, {ebx, true}, {ecx, true}, {edx, true},
                {esi, true}, {edi, true}, {r8d, true}, {r9d, true}
        };

        reg_arg = {
                {edi, true}, {esi, true}, {edx, true}, {ecx, true}, {r8d, true}, {r9d, true}
        };

        parm_reg_mapping = {
                edi, esi, edx, ecx, r8d, r9d
        };
        // 初始化mapping
        for(const auto& reg : reg_order) {
            reg_memory_mapping.insert({reg.name, {FREE, 0, ""}});
        }

        temp_count = 0;
        nth_param = 1;
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
    void generateParam(Instruction* ins);
    void generateCompare(Instruction* ins);
    void generateGetParam(Instruction* ins);

    int fetchStackVariable(std::string& variable);
    x86_reg getNextArgReg();
    void free_arg();
    bool isParam(Operand*);
    // 把全局变量/堆栈/常量加载到寄存器中
    x86_reg bringToReg(Operand* operand, x86_reg=not_in);
    x86_reg loadLiteralToReg(int, x86_reg=not_in);
    void freeReg(x86_reg, bool=false);
    // 获取某一个寄存器的使用权
    x86_reg get_x86_reg();
    void get_x86_reg(x86_reg);

    // 保存上一个函数的活动记录
    void push_rbp();
    void pop_rbp();

    void collect_bss_data(Instruction*);
    // 提前分配好堆栈
    void allocateStack();
    void deallocateStack();

    std::string getTempStringLable();
    // 将ir写入文件
    void x86Instruction(const string& label, const string& ins, const string& op1, const string& op2);
    // 必须16字节对齐
    int stack_size;
    // 中间代码
    SPL_IR* ir;

    // 输出的文件
    std::ofstream outfile;

    // 字符串字面量
    // std::map<std::string, Operand*> string_literals;

    std::map<std::string, std::string> string_literals;
    // 记录寄存器是否可用
    std::map<x86_reg, reg_memory> reg_memory_mapping;
    // 查找空闲寄存器的顺序
    std::vector<reg_order> reg_order;

    std::vector<reg_arg> reg_arg;
    // 变量是否在寄存器当中
    std::map<std::string, x86_reg> name_to_reg;
    // 变量相对rbp的offset
    std::map<std::string, int> name_to_stack;
    // callee saved
    std::set<x86_reg> callee_saved_registers;

    std::map<std::string, pair<x86_size, unsigned int>> bss_data;

    std::set<std::string> param;

    std::vector<x86_reg> parm_reg_mapping;
    unsigned int temp_count;

    unsigned int nth_param;

    std::string current_function_name;
};


#endif //SPLCOMPILER_SPL_CODEGEN_HPP
