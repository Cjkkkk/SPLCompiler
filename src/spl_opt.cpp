//
// Created by DELL on 2019/5/9.
//
#include "spl_SSA.hpp"
#include <stack>

void copyByValue(Operand* res, Operand* source) {
    *res = *source;
}

// 沿着DU链如果有指令使用了目标变量就替换成指定对应变量
// copy propagation中 v1 = v2
// 则沿着DU链，如果有变量使用v1 则替换成v2
// const propagation v1 = v2(const) 或者 v1 = v2(const op const)
// 则沿着DU链，如果有变量使用了v1 则替换成v2

// 同时根据v2是否为const决定是否v2的duchain
// 虽然v1被替换，但是不需要删除v1中的对应使用记录，vector删除操作耗时


// propagateAlongDuchain
// @param usage : 需要遍历的变量的所有usage
// @param res : v2
// @param target : v1的名字
// @param copy_usage : 如果有需要则更新v2的usage
// @return


void propagateAlongDuchain(std::list<Instruction*>& usage,
                           Operand* res, const string& target,
                           std::list<Instruction*>* copy_usage = nullptr) {

    for(auto use = usage.begin() ; use != usage.end() ; use ++) {
        bool use_or_not = false;
        if(checkInstructionOp(*use, OP_PHI)) {
            for(auto var : *((*use)->getVariable())) {
                if(var.first->name == target) {
                    copyByValue(var.first, res);
                    use_or_not = true;
                }
            }
        }
        if((*use)->arg1 && (*use)->arg1->name == target) {
            copyByValue((*use)->arg1, res);
            use_or_not = true;
        }
        if((*use)->arg2 && (*use)->arg2->name == target) {
            copyByValue((*use)->arg2, res);
            use_or_not = true;
        }

        if(use_or_not) {
            if( ! checkOperandClass(res, KNOWN) ) {
                copy_usage->push_back(*use);
            }
            // 从DU hain中删除使用记录
            use = usage.erase(use);
            -- use;
        }
    }
}


void eraseAlongDuChain(
        std::map<std::string, std::list<Instruction*>>& nameUsageMap,
        stack<string>& unusable,
        std::string& name, unsigned id) {

    auto du_it = nameUsageMap.find(name);
    list<Instruction*>& use_list = du_it->second;
    for(auto it = use_list.begin() ; it != use_list.end(); it ++) {
        if((*it)->unique_id == id) {
            // 删除
            it = use_list.erase(it);
            break;
        }
    }

    // 检查长度是否还是大于0
    if(use_list.empty()) {
        unusable.push(name);
        nameUsageMap.erase(du_it);
    }

}


// const propagation 常量传播
// loop through all instruction
// var1 = var2 或者 var1 = var2 op var3 propagation through DU chain

void SPL_SSA::constantPropagation() {
    map<std::string, Operand*> constMap;
    for(auto& node: nodeSet) {
        for(auto ins_it = node->instruSet.begin() ; ins_it != node->instruSet.end(); ins_it ++) {
            if(isCalculateOp((*ins_it)->op)) {
                if(checkOperandClass((*ins_it)->arg1,KNOWN)
                   && checkOperandClass((*ins_it)->arg2,KNOWN)) {
                    SPL_OP op = (*ins_it)->op;

                    auto it = nameUsageMap.find((*ins_it)->res->name);

                    auto o = evaluate(op,(*ins_it)->arg1, (*ins_it)->arg2);

                    delete((*ins_it)->arg1);
                    (*ins_it)->arg1 = nullptr;
                    delete((*ins_it)->arg2);
                    (*ins_it)->arg2 = nullptr;


                    (*ins_it)->op = OP_ASSIGN;
                    (*ins_it)->arg1 = o;
                    propagateAlongDuchain(it->second, (*ins_it)->arg1, it->first);
//                    ins_it = node->instruSet.erase(ins_it);
//
//                    --ins_it;
                }
                continue;
            }
            if (checkInstructionOp(*ins_it, OP_ASSIGN)) {
                if(checkOperandClass((*ins_it)->arg1, KNOWN)
                   && checkOperandType((*ins_it)->arg1, INT)) {
                    auto it = nameUsageMap.find((*ins_it)->res->name);

                    propagateAlongDuchain(it->second, (*ins_it)->arg1, it->first);

                }
                continue;
            }


            if (checkInstructionOp(*ins_it, OP_PHI)) {
                Operand* o = nullptr;
                bool res = true;
                for(auto var: *(*ins_it)->getVariable()) {
                    if (!checkOperandClass(var.first, KNOWN)) {
                        // 存在不为const的变量
                        res = false;
                        break;
                    } else if (!o) {
                        o = var.first;
                    } else if(!compareValue(var.first, o)){
                        // phi中变量值不一致
                        res = false;
                        break;
                    }
                }
                if(res) {
                    auto it = nameUsageMap.find((*ins_it)->res->name);
                    (*ins_it)->op = OP_ASSIGN;

                    (*ins_it)->arg1 = new Operand;
                    copyByValue((*ins_it)->arg1, o);

                    propagateAlongDuchain(it->second, (*ins_it)->arg1, it->first);
                }
            }
        }
    }
}


// copy propagation 拷贝传播
// loop through all definition from top to bottom
// propagate var1 = var2 到所有使用var1的ins中

void SPL_SSA::copyPropagation() {
    for(auto& var : nameDefinitionMap ) {
        auto ins = var.second;

        if(!checkInstructionOp(ins, OP_ASSIGN)) continue;

        auto& usage = nameUsageMap.find(ins->res->name)->second;

        auto& copy_usage = nameUsageMap.find(ins->arg1->name)->second;

        propagateAlongDuchain(usage, ins->arg1, ins->res->name, &copy_usage);
    }
}


// 遍历所有du chain计算所有变量的使用次数 如果为o则添加到待删除的集合中
// 遍历待删除的集合 找到这些变量的定义处 删除定义
// 遍历定义使用到的变量 将这些变量中的使用记录去除 如果这些变量的使用次数也减为0则添加到待删除的集合

void SPL_SSA::removeUnusedVariable() {
    map<string, Instruction*> m;
    for(auto& def: nameDefinitionMap) {
        m.insert(def);
    }

    // 初始化所有没有使用过的变量的集合
    stack<string> unusable;
    for(auto& usage: nameUsageMap) {
        if(usage.second.empty()) {
            unusable.push(usage.first); // 没有被使用过
        }
    }

    set<unsigned int> collect_id;

    while(!unusable.empty()) {
        // 找到这些没有使用过的变量的定义
        string& current = unusable.top();
        unusable.pop();

        auto it = m.find(current);
        if(it == m.end()) {
            // 不在该作用域的变量没有nameDefinitionMap 所以自然也无法删除了
            continue;
        }
        Instruction* def = it->second;

        collect_id.insert(def->unique_id);

        if(checkInstructionOp(def, OP_PHI)) {
            for(auto& var: *(def->getVariable())) {
                if(checkOperandClass(var.first, KNOWN)) continue;
                eraseAlongDuChain(nameUsageMap, unusable, var.first->name, def->unique_id);
            }
            continue;
        }

        if(def->arg1 && !checkOperandClass(def->arg1, KNOWN)) {
            // 在du chain中删除对应的使用
            eraseAlongDuChain(nameUsageMap, unusable, def->arg1->name, def->unique_id);
        }

        if(def->arg2 && !checkOperandClass(def->arg2, KNOWN)) {
            // 在du chain中删除对应的使用
            eraseAlongDuChain(nameUsageMap, unusable, def->arg2->name, def->unique_id);
        }

    }


    // 删除nameDefinitonMap中没有使用的项
    for(auto it = nameDefinitionMap.begin() ; it != nameDefinitionMap.end() ; it ++) {
        if(collect_id.find((*it).second->unique_id) != collect_id.end()) {
            it = nameDefinitionMap.erase(it);
            it --;
        }
    }

    // 删除所有没有使用的变量
    for(auto node: nodeSet) {
        for(auto it = node->instruSet.begin(); it != node->instruSet.end() ; it ++) {
            if(collect_id.find((*it)->unique_id) != collect_id.end()) {
                it = node->instruSet.erase(it);
                it --;
            }
        }
    }
}
