#ifndef CODEGENERATOR_H_INCLUDED
#define CODEGENERATOR_H_INCLUDED
using namespace std;

#include "Struct.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <cctype>
#include <cstdio>


//parsing tree node


class CodeGenerator{
    public:
        bool ReadParsingtree(string name);//read parsing tree & create empty symbol table

        bool CreateSymbolTable(string name);
        bool CreateSymbolTable2(string name);
        bool SemanticRule(string name);
        void PrintTree();

        CodeGenerator();
        ~CodeGenerator();
    private:
        int TreeRecu(ifstream &input, Node* head, int depth, stack<int>& stackScope,int &maxScope);//將parsing tree的節點給加入
        void ChangeScope(Node* node);
        bool CreateSymbolTableRecu(Node* node);
        void PrintTree_recu(Node* cur);
        void destroy(Node* node);//destructor使用 以銷毀樹狀結構
        bool Stmt(Node* node);
        bool Expr(Node* node);
        string PushQuadruple(string op, string result, string arg1, string arg2);
        bool ExprArrayTail(Node* node);
        string RenameId(string id, int scope);

        Node* root;
        vector<SymbolTable> symboltable;
        vector<Quadruple> quadruples;
        Quadruple qInst;//Quadruple instruction
        TempCreate tempC;
        string epsilon;

        set<string> nonterminal;
        string Op[15];
        string temp[8];
        string vType[3];
        string vType2[3];
        enum OP{
            NO_OP,
            ASSIGN,
            ADD,
            MINUS,
            MULTIPLY,
            DIVIDE,
            GREATER,
            LESS,
            INCREMENTSUFFIX,
            INCREMENTPREFIX,
            JFALSE,
            JMP
        };
};


#endif // CODEGENERATOR_H_INCLUDED
