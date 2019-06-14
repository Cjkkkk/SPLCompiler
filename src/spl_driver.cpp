/*
 * @file        spl_driver.cpp
 * @brief       Compiler's frontend.
 * @details     .
 * @author      Jiang Xiaochong
 * @date        3/13/2019
 * @version     1.0.0
 * @par         Copyright(c): Zuiqiang Xiaozu(Best Group)
 */

#include <cassert>
#include <cctype>
#include <fstream>
#include <set>

#include "spl_driver.hpp"

SPL::SPL_Driver::~SPL_Driver()
{
    delete (scanner);
    scanner = nullptr;
    delete (parser);
    parser = nullptr;
}

void SPL::SPL_Driver::parse(const char *const filename)
{
    assert(filename != nullptr);
    std::ifstream in_file(filename);
    if (!in_file.good())
    {
        exit(EXIT_FAILURE);
    }
    parse_helper(in_file);
    return;
}

void SPL::SPL_Driver::parse(std::istream &stream)
{
    if (!stream.good() && stream.eof())
    {
        return;
    }
    // else
    parse_helper(stream);
    return;
}

void SPL::SPL_Driver::parse_helper(std::istream &stream)
{

    delete (scanner);
    try
    {
        scanner = new SPL::SPL_Scanner(&stream);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate scanner: (" << ba.what()
                  << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }

    delete (parser);
    try
    {
        parser =
            new SPL::SPL_Parser((*scanner) /* scanner */, (*this) /* driver */);
    }
    catch (std::bad_alloc &ba)
    {
        std::cerr << "Failed to allocate parser: (" << ba.what()
                  << "), exiting!!\n";
        exit(EXIT_FAILURE);
    }
    const int accept(0);
    if (parser->parse() != accept)
    {
        // std::cerr << "Parse failed!!\n";
        exit(EXIT_FAILURE);
    }
    return;
}


std::ostream &SPL::SPL_Driver::print(std::ostream &stream)
{
    return (stream);
}


void SPL::SPL_Driver::emitIR() {
    // 为每一个函数留一个位置存放IR
    ir.getIRSet().resize(astmng.functions->size());

    // 输出没有优化的中间码
    std::ofstream outfile;
    outfile.open("byte_code/unopt.bc", std::ios::out);
    // 遍历每一个函数的语法树生成IR
    for(auto index = 0 ; index < astmng.functions->size() ;index ++) {
        // 设定当前处理的哪个函数

        ir.setCurrent(index);

        AST* func = astmng.functions->at(index);

        // 设置当前函数的作用域
        unsigned int scopeIndex = astmng.scopes->at(index);
        // 获取该函数定义在那个作用域之中
        unsigned int PreScopeIndex = astmng.defined_scopes->at(index);

        ir.symbolTable->setCurrentScopeIndex(scopeIndex);

        // 代码生成
        auto func_name = symtab.getFunctionNameByIndex(scopeIndex);
        ir.addInstruction(new Instruction{
            func_name + "." + std::to_string(PreScopeIndex),
            OP_NULL,
            nullptr,
            nullptr,
            nullptr});

        // 获取参数列表
        auto f = symtab.lookupFunction(func_name.c_str());

        if( f ) {
            auto argument_list = f->subSymbolList;
            for(auto& argument: *argument_list) {
                ir.addInstruction(new Instruction{
                        "",
                        OP_FUNC_PARAM,
                        new Operand{argument->elementType, argument->name + "." + std::to_string(argument->scopeIndex), PARAM, argument},
                        nullptr,
                        nullptr
                });
            }

            if(f->symbolType != UNKNOWN) {
                // 函数有返回值
                auto ret = symtab.lookupVariable(func_name.c_str());
                ir.addInstruction(new Instruction{
                        "",
                        OP_FUNC_RET,
                        new Operand{f->elementType, func_name + "." + std::to_string(PreScopeIndex), RET, ret},
                        nullptr,
                        nullptr
                });
            }
        }
        func->emit(&ir);
        ir.addInstruction(new Instruction{"", OP_RET, nullptr, nullptr, nullptr});
        ir.outputInstruction(outfile);
    }
    outfile.close();
}


// 代码优化
void SPL::SPL_Driver::optimizeIR() {
    std::ofstream outfile;
    outfile.open("byte_code/opt.bc", std::ios::out);
    for(auto index = 0 ; index < ir.getIRSetSize() ; index ++) {
        // 设定当前处理的函数的IR的index
        ir.setCurrent(index);
        unsigned int scopeIndex = astmng.scopes->at(index);

        // 设置作用域
        ir.symbolTable->setCurrentScopeIndex(scopeIndex);

        // 生成IR优化对象
        SPL_SSA ssa_ir(ir.getCurrentIR(), &ir);
        ssa_ir.OptimizeIR();
        ssa_ir.printIR(outfile);
    }
    outfile.close();
}


// 目标代码生成
void SPL::SPL_Driver::codeGen() {
    code_gen.GenerateMachineCode();
}