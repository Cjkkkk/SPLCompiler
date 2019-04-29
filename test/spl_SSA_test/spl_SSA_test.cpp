#include "gtest/gtest.h"
#include "spl_IR.hpp"
#include "spl_SSA.hpp"


namespace {
// To use a test fixture, derive a class from testing::Test.
    class SPLSSATest : public testing::Test {
    protected:  // You should make the members protected s.t. they can be
        void SetUp() override {
            ir.addInstruction({"test", OP_NULL, nullptr, nullptr, nullptr});
            ir.addInstruction({"", OP_ASSIGN, new Operand(INT,"", CONST), nullptr ,new Operand(INT, "_t0", TEMP)});
            ir.addInstruction({"", OP_ASSIGN, new Operand(INT,"", CONST), nullptr ,new Operand(INT, "_t1", TEMP)});
            ir.addInstruction({"", OP_ASSIGN, new Operand(INT,"", CONST), nullptr ,new Operand(INT, "_t2", TEMP)});
            ir.addInstruction({"", MUL_, new Operand(INT,"_t1", TEMP), new Operand(INT, "_t2", TEMP) ,new Operand(INT, "_t3", TEMP)});
            ir.addInstruction({"", PLUS_, new Operand(INT,"_t0", TEMP), new Operand(INT, "_t3", TEMP) ,new Operand(INT, "_t4", TEMP)});
            ir.addInstruction({"", OP_ASSIGN, new Operand(INT,"_t4", TEMP), nullptr ,new Operand(INT, "0.ans.0", VAR)});
        }

        void TearDown() override {};
        SPL_IR ir;
        // Declares the variables your tests want to use.
    };

    TEST_F(SPLSSATest, DefaultConstructor) {
        // You can access data in the test fixture here.
        EXPECT_EQ(7, ir.IR.size());
    }
}  // namespace