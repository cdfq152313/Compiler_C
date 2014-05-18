#include <iostream>
#include <string>
#include "ParserGenerator.h"
#include "Parser.h"
#include "Scanner.h"
#include "CodeGenerator.h"
#include "MCodeGenerator.h"
using namespace std;


int main()
{
    Scanner scanner;
    ParserGenerator pg;
    Parser pa;
    CodeGenerator cg;
    MCodeGenerator mcg;
    bool checksc = false;
    bool checkpg = false;
    bool checkpa = false;

    //scanner


    scanner.open("./input/main.c");
    checksc = scanner.start("./output/token.txt");
    if(checksc){
        scanner.PrinttoParser("./output/ParserToken.txt");
    }

    //ParserGenerator
    pg.ReadGrammar("./input/grammar.txt");
    checkpg = pg.ConstructNullFirstFollow("./output/set.txt");
    checkpg = pg.GenerateLLTable("./output/LLtable.txt");

    if(checkpg && checksc){
        pa.ReadLLTable("./output/LLtable.txt");
        pa.ReadToken("./output/ParserToken.txt");
        checkpa = pa.LLParser();
        if(checkpa){
            pa.PrintTree("./output/tree.txt");
        }
    }
    //intermediate code
    cg.ReadParsingtree("./output/tree.txt");
    cg.CreateSymbolTable("./output/symbol.txt");
    cg.CreateSymbolTable2("./output/symbol2.txt");
    cg.SemanticRule("./output/quadruples.txt");

    //machine code
    mcg.ReadSymbolTable("./output/symbol2.txt");
    mcg.ReadIntermediateCode("./output/quadruples.txt");
    mcg.MachineCodeGenerate("./output/code.tm");

    //Parser
    cout << "press ENTER to exit" << endl;
    getchar();
    return 0;
}

