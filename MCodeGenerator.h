#ifndef MCODEGENERATOR_H_INCLUDED
#define MCODEGENERATOR_H_INCLUDED

using namespace std;
#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<sstream>
#include<cctype>
#include"Struct.h"

class MCodeGenerator{
    public:
        bool ReadSymbolTable(string name);//read parsing tree & create empty symbol table
        bool ReadIntermediateCode(string name);
        bool MachineCodeGenerate(string name);
        MCodeGenerator(){
            tempNum = 8;
            temp[0] = "t0";
            temp[1] = "t1";
            temp[2] = "t2";
            temp[3] = "t3";
            temp[4] = "t4";
            temp[5] = "t5";
            temp[6] = "t6";
            temp[7] = "t7";

            op["="] = "LDC";
            op["l="] = "LD";
            op["s="] = "ST";
            op["+"] = "ADD";
            op["-"] = "SUB";
            op["*"] = "MUL";
            op["/"] = "DIV";
            op["<"] = "LT";
            op["<="] = "LE";
            op[">"] = "GT";
            op[">="] = "GE";
            op["=="] = "EQ";
            op["!="] = "NE";
            op["&&"] = "AND";
            op["||"] = "OR";
            op["!"] = "NOT";
            op["JEQ"] = "JEQ";

            opType["="] = 0;
            opType["l="] = 1;
            opType["s="] = 1;
            opType["+"] = 0;
            opType["-"] = 0;
            opType["*"] = 0;
            opType["/"] = 0;
            opType["<"] = 0;
            opType["<="] = 0;
            opType[">"] = 0;
            opType[">="] = 0;
            opType["=="] = 0;
            opType["!="] = 0;
            opType["&&"] = 0;
            opType["||"] = 0;
            opType["!"] = 0;
            opType["JEQ"] = 1;
        }
    private:
        string Translate(string s);
        map<string, string> symbolTable;
        vector<Quadruple> iCode;
        map<string, string> op;
        map<string, int> opType;
        string temp[8];
        int tempNum;
};

#endif // MCODEGENERATOR_H_INCLUDED
