#ifndef PARSERGENERATOR_H_INCLUDED
#define PARSERGENERATOR_H_INCLUDED

using namespace std;
#include "Struct.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <cctype>




class ParserGenerator{
public:
    ParserGenerator();
    bool ReadGrammar(string name); //input read grammar
    bool ConstructNullFirstFollow(string name); //construct nullable first follow & output
    void PrintGrammar(); // print grammar on console
    bool GenerateLLTable(string name); //consturct LL(1) parsing table & output


private:
    map<string, Nonterminal> nonterminals; //map
    string epsilon; // epsilon string
    map<string, map<string, vector<string> > > LLtable; //LL(1)parsing Table
    vector<string> nonorder;

    bool PrintNullFirstFollow(string name);//pirnt nullable first

    //nullable test
    void nullable_test();
    bool null_iter(map<string, Nonterminal>::iterator it);
    //first test
    void first_test();
    bool first_iter(map<string, Nonterminal>::iterator it);
    //follow test
    void follow_test();
    void follow_add_first();

    //LL(1) parsing table
    bool create_table();//create LL(1) parsing table
    bool print_table(string name);// output LL(1) parsing table

    bool subset(set<string> & s1,set<string> & s2);//s2 belongs to s1
};


#endif // PARSERGENERATOR_H_INCLUDED
