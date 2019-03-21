//
// Created by DELL on 2019/3/21.
//

#ifndef SPLCOMPILER_AST_H
#define SPLCOMPILER_AST_H

#include <string>

class AST {

};
class Stmt: public AST{

};

class IfStmt: public Stmt{

};

class WhileStmt:public Stmt{

};

class Expression: public AST{
public:
    virtual int Evalute();
};
class BinaryExpr: public Expression {
public:
    BinaryExpr(Expression* l, Expression* r, int o):LExpr(l), REXpr(r), op(o){}
    int Evalute(){
        if(op == 1)return LExpr->Evalute() + REXpr->Evalute();
        else if(op == 2)return LExpr->Evalute() - REXpr->Evalute();
        else if(op == 3)return LExpr->Evalute() * REXpr->Evalute();
        else if(op == 4)return LExpr->Evalute() / REXpr->Evalute();
        return -1000;
    }
    Expression* LExpr;
    Expression* REXpr;
    int op;
};

class UnaryExpr: public Expression{
public:
    int Op;
    Expression* Expr;
};

class Primary: public Expression{
public:
    explicit Primary(int v):IValue(v){}
    explicit Primary(std::string v):SValue(v){}
    explicit Primary(char v):CValue(v){}
    explicit Primary(double v):DValue(v){}
    int Evalute(){ return IValue;}
    int IValue;
    std::string SValue;
    char CValue;
    double DValue;
};
#endif //SPLCOMPILER_AST_H
