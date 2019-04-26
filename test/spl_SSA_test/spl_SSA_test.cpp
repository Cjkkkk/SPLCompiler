#include "gtest/gtest.h"
#include "spl_SSA.hpp"

namespace {
    class TestClass:public testing::Test{
    public:
        SPL_SSA splSSAObj;
    };
}

//TEST_F(TestClass,Test_ReturnZeroMethod){
//    ASSERT_EQ(test_IamClass_obj.return_Zero(),0);
//}