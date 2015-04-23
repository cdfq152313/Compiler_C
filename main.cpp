#include <iostream>
#include <string>
#include "ParserGenerator.h"
using namespace std;

int main()
{
    bool checkpg;
    ParserGenerator pg;
    pg.ReadGrammar("grammar.txt");
    checkpg = pg.ConstructNullFirstFollow("set.txt");
    checkpg = pg.GenerateLLTable("LLtable.txt");
    
    return 0;
}

