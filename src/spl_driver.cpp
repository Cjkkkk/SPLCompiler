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
    for(auto index = 0 ; index < astmng.functions->size() ;index ++) {
        AST* func = astmng.functions->at(index);
        unsigned int scopeIndex = astmng.scopes->at(index);
        // 设置作用域
        ir.symbolTable->setCurrentScopeIndex(scopeIndex);
        ir.IR.emplace_back(symtab.getFunctionNameByIndex(scopeIndex), OP_NULL, "", "", "");
        func->emit(&ir);

        ir.IR.emplace_back( "", OP_RET, "", "", "");
    }
}

void SPL::SPL_Driver::printIR() {
    std::ofstream outfile;
    outfile.open("out.bc", std::ios::out);
    for(const auto & ir : ir.IR) {
        std::cout << ir.label << "\t" << SPL_OPToString(ir.op) << "\t" << ir.arg1 << "\t" << ir.arg2 << "\t" << ir.result << "\n";
        outfile << ir.label << "\t" << SPL_OPToString(ir.op) << "\t" << ir.arg1 << "\t" << ir.arg2 << "\t" << ir.result << "\n";
    }
    outfile.close();
}


void SPL::SPL_Driver::genSSATree() {
    std::vector<SSANode*> nodeSet;
    SSANode* current;
    std::map<std::string, int> labelIndexMap;
    for(auto ins : ir.IR){
        if(ins.label != "") {
            // start target
            SSANode* newNode = new SSANode();
            nodeSet.push_back(newNode);
            current = newNode;
            current->label = &ins.label;
            labelIndexMap.insert({*current->label, nodeSet.size() - 1});
        } else if (ins.op == OP_IF || ins.op == OP_IF_Z || ins.op == OP_GOTO) {
            // 添加子节点
            current->LabelSet.push_back(&ins.result);
        } else {
            // 添加指针
            current->instruSet.push_back(&ins);
        }
    }
    
    // generate CFG
    for(auto node: nodeSet) {
        auto currentNodeOffset = &node - &nodeSet[0];
        for(auto label : node->LabelSet) {
            auto offset = labelIndexMap.find(*label)->second; // 找到子女的label对应的offset
            node->childSet.push_back(offset); // child set
            nodeSet[offset]->parentSet.push_back(currentNodeOffset); // parent set
        }
    }

    // compute SD
    for(auto node : nodeSet) {
        if(node->parentSet.size() != 0) {

        }
    }
    
}