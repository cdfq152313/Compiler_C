#include "Struct.h"

Node::Node(string head, int depth){
        this->head = head;
        this->depth = depth;
        expr = false;
    }
Node::Node(string head, int depth, int scope){
    this->head = head;
    this->depth = depth;
    this->scope = scope;
    expr = false;
}

Quadruple::Quadruple(string op, string result, string arg1, string arg2){
    NewQuadruple(op, result, arg1, arg2);
}
Quadruple::Quadruple(){
}
void Quadruple::NewQuadruple(string op, string result, string arg1, string arg2){
    this->op = op;
    this->result = result;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

SymbolTable::SymbolTable(int scope, int pScope){
    this->scope = scope;
    this->pScope = pScope;
}

TempCreate::TempCreate(){
    for(int i = 0; i < 8; ++i){
        temp[i] = false;
    }
    tempS[0] = "t0";
    tempS[1] = "t1";
    tempS[2] = "t2";
    tempS[3] = "t3";
    tempS[4] = "t4";
    tempS[5] = "t5";
    tempS[6] = "t6";
    tempS[7] = "t7";
}
bool TempCreate::IsTemp(string t){
    for(int i = 0; i < 8; ++i){
        if(tempS[i] == t){
            return true;
        }
    }
    return false;
}
string TempCreate::FindTemp(){
    //only 1~7 can use
    for(int i = 0; i < 7; ++i){
        if(!temp[i]){
            temp[i] = true;
            return tempS[i];
        }
    }
}
void TempCreate::UnlockTemp(string t){
    for(int i = 0; i < 8; ++i){
        if(tempS[i] == t){
            temp[i] = false;
            return;
        }
    }
}
void TempCreate::UnlockAllTemp(){
    for(int i = 0; i < 8; ++i){
        temp[i] = false;
    }
}
