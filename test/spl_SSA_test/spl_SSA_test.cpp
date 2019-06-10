#include "gtest/gtest.h"
#include "spl_IR.hpp"
#include "spl_SSA.hpp"


namespace {
// To use a test fixture, derive a class from testing::Test.
    class SPLSSATest : public testing::Test {
    public:
        SPLSSATest():ir(nullptr),s(ir.getCurrentIR(), &ir) {}
    protected:  // You should make the members protected s.t. they can be
        void SetUp() override {
            ir.addInstruction(new Instruction{"test", OP_NULL, nullptr, nullptr, nullptr});
            ir.addInstruction(new Instruction{"", OP_ASSIGN, new Operand(INT,"", KNOWN), nullptr ,new Operand(INT, "_t0", TEMP)});
            ir.addInstruction(new Instruction{"", OP_ASSIGN, new Operand(INT,"", KNOWN), nullptr ,new Operand(INT, "_t1", TEMP)});
            ir.addInstruction(new Instruction{"", OP_ASSIGN, new Operand(INT,"", KNOWN), nullptr ,new Operand(INT, "_t2", TEMP)});
            ir.addInstruction(new Instruction{"", MUL_, new Operand(INT,"_t1", TEMP), new Operand(INT, "_t2", TEMP) ,new Operand(INT, "_t3", TEMP)});
            ir.addInstruction(new Instruction{"", PLUS_, new Operand(INT,"_t0", TEMP), new Operand(INT, "_t3", TEMP) ,new Operand(INT, "_t4", TEMP)});
            ir.addInstruction(new Instruction{"", OP_ASSIGN, new Operand(INT,"_t4", TEMP), nullptr ,new Operand(INT, "0.ans.0", VAR)});

            //s.genCFGNode();
           // s.generateCFG();
            //s.computeTreeIdom();
            // compute DF
           // s.generateDF();
        }

        void TearDown() override {};
        SPL_IR ir;
        SPL_SSA s;
        // Declares the variables your tests want to use.
    };

    TEST_F(SPLSSATest, DefaultConstructor) {
        // You can access data in the test fixture here.
        //EXPECT_EQ(7, ir.IR.size());
    }

    TEST_F(SPLSSATest, RemoveUnused) {
        // You can access data in the test fixture here.
//        s.removeUnusedVariable();
//
//        EXPECT_EQ(0, s.nodeSet[0]->instruSet.size());
    }
}  // namespace