#ifndef STRUCT_H_INCLUDED
#define STRUCT_H_INCLUDED
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

class Token{
public:
    string token;
    string data;
    int type;
    int line;
    /* type
        0  <Keywords>
        1  <Operator>
        2  <Special Symbol>
        3  <Identifier>
        4  <Char>
        5  <String>
        6  <Int>
        7  <Float>
        8  <Error>
        9  <Comment>
		10 <Number>
    */
};

class Node{
public:
    string head;
    vector<string> body;
    vector<Node*> pbody;
    string id;
    string temp;
    string op;//node operate
    bool expr;
    int scope;
    int line;
    int depth;
    int type;
    /**
    type1:
        0. int
        1. char
    type2:
        0. function
        1. variable
        2. array
    **/

    Node(string head, int depth);
    Node(string head, int depth, int scope);
};


class Nonterminal{
public:
    string head;
    vector< vector<string> > body;
    vector<bool> bodynullable;
    vector< set<string> > bodyfirst;
    bool nullable;
    set<string> first;

    //follow teminal(first(B) ÄÝ©ó follow(A))
    set<string> follow;
    //follow nonterminal(follow(C) ÄÝ©ó follow(A))
    set<string> follow_non;
    bool visit;
};

class Quadruple{
public:
    string op;
    string arg1;
    string arg2;
    string result;
    Quadruple(string op, string result, string arg1, string arg2);
    Quadruple();
    void NewQuadruple(string op, string result, string arg1, string arg2);
};

//symbol table的entry
class Entry{
public:
    string symbol;
    string token;
    string arraySize;
    int type;
    /**
    type1:
        0. int
        1. char
    **/
    int type2;
    /**
    type2:
        0. function
        1. variable
        2. array
    **/
    int scope;//此ID在第幾scope
    bool variable;//是否為變數(若false則為function)
};

class SymbolTable{
public:
    int scope;
    int pScope;//parent scope
    map<string , Entry> entry;//symbol , name
    SymbolTable(int scope, int pScope);
};
class TempCreate{
public:
    //eight bit temp
    bool temp[8];
    string tempS[8];
    TempCreate();
    bool IsTemp(string t);
    string FindTemp();
    void UnlockTemp(string t);
    void UnlockAllTemp();
};
#endif // STRUCT_H_INCLUDED
