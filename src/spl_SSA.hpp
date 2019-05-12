//
// Created by DELL on 2019/4/22.
//

#ifndef SPLCOMPILER_SPL_SSA_H
#define SPLCOMPILER_SPL_SSA_H

#include <vector>
#include <list>
#include <set>
#include "spl_IR.hpp"
class SSANode {
public:
    SSANode():idom (-1) {};
    // 每一个CFG node中对应的IR
    std::list<Instruction*> instruSet;

    // 每一个CFG node dominate的节点列表
    std::vector<int> DSet;

    // 每一个CFG node对应的父节点列表
    std::vector<int> parentSet;

    // 每一个CFG node对应的 dominate frontier列表
    std::vector<int> DF;

    // 记录每一个CFG node control流向的node的label
    std::vector<std::string*> LabelSet;

    // 记录CFG本身的label
    std::string* label;

    // 记录CFG node对应的immediate dominate
    int idom;
};
class SPL_SSA {
public:
    SPL_SSA(vector<Instruction*>& ins_, SPL_IR* ir_): insSet(ins_), ir(ir_) {};
    ~SPL_SSA() {
        for(auto& node: nodeSet) {
            delete(node);
        }
    }

    vector<Instruction*>& insSet;
    SPL_IR* ir;
    // CFG中所有的node集合
    std::vector<SSANode*> nodeSet;

    // 每个label对应的node
    std::map<std::string, int> labelIndexMap;

    // 每个变量出现的block
    std::map<std::string, std::vector<int>> variableListBlock;

    // 记录所有Phi出现的block

    std::set<int> phiBlock;

    // 每一个定义对应的所有使用的位置ins*
    std::map<std::string, std::list<Instruction*>> nameUsageMap;

    // 每一个定义出现的位置ins*
    std::list<pair<std::string, Instruction*>> nameDefinitionMap;

    // 优化IR
    void OptimizeIR();

    void genCFGNode();

    // 生成CFG
    void generateCFG();

    // 计算Dominate frontier

    void generateDF();
    // 计算CFGd的每一个节点的immediate dominate
    void computeTreeIdom();
    void computeIdom(int index, std::vector<SSANode*>& nodeSet);

    // 在必要的位置插入phi函数
    void insertPhiFunction();
    void insertPhi(int nodeIndex, const string& variableName);

    // 重命名变量
    void renameVariable();


    void updateUsage(std::vector<map<std::string, int>>& def,
                    Operand* operand,
                    int& nodeIndex,
                    Instruction* ins);

    void updateDefinition(map<std::string, int>& currentDef,
                         std::vector<map<std::string, int>>& def,
                         Operand* operand,
                         int& nodeIndex,
                         Instruction* ins);

    void removeUnusedVariable();
    void copyPropagation();
    void constantPropagation();

    void outputInstruction(std::string filename);
    void outputDUChain();
    void outputIdom();
    void backToTAC(std::vector<Instruction*>& ins);

    void printIR(ostream& s);
};


#endif //SPLCOMPILER_SPL_SSA_H
