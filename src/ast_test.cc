#include "spl_ast.hh"

void test(void){
    int i=0, j=1;
    class SPL::AST_Imm* i1 = new SPL::AST_Imm(0, &i);
    class SPL::AST_Imm* i2 = new SPL::AST_Imm(0, &j);
    class SPL::AST_Math* m1 = new SPL::AST_Math(0, i1, i2);
    
}