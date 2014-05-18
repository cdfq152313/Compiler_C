#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

using namespace std;
#include "Struct.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <set>
#include <cctype>
#include <cstdio>
#include "Scanner.h"



//Token::token
//Token::data


class Parser{
    public:
        Parser(){
            root = NULL;
        }
        ~Parser(){
            if(root != NULL)
                cleartree(root);
        }
        bool ReadLLTable(string name);
        bool ReadToken(string name);
        bool LLParser();
        bool PrintStep(string name);
        bool PrintTree(string name);

    private:
        //output2 output3
        //想法為二維陣列 值為iterator 指向production
        map<string, map<string, vector<string> > > LLtable;
        vector <Token> input;
        string startsymbol;
        string epsilon;
        Node *root;

        bool LLParser_recu(unsigned &i, Node* cur);
        void PrintStep_recu(ofstream &output, Node* cur);
        void PrintTree_recu(ofstream &output, Node* cur);
        void cleartree(Node* cur);

};

#endif // PARSER_H_INCLUDED
