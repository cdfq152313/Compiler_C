#include"MCodeGenerator.h"

bool MCodeGenerator::ReadSymbolTable(string name){
    cout << "Read Symbol Table" << endl;

    ifstream input;
    input.open(name.c_str());
    if(!input){
        cout << "Read Symbol Table failed." << endl;
        return false;
    }
    string symbol, locate;
    while(input.good()){
        input >> symbol >> locate;
        symbolTable[symbol] = locate;
        input >> ws;
    }
    input.close();
    return true;
}

bool MCodeGenerator::ReadIntermediateCode(string name){
    cout << "Read Intermediate Code" << endl;

    ifstream input;
    input.open(name.c_str());
    if(!input){
        cout << "Read Intermediate Code failed." << endl;
        return false;
    }
    string line;
    Quadruple q;
    while(input.good()){
        stringstream line_buf;
        getline(input, line);

        line_buf.str(line);
        line_buf >> q.op >> q.result >> q.arg1 >> ws;
        if(line_buf.good()){
            line_buf >> q.arg2;
            if(q.op == "=")
                q.op = "l=";
        }else{
            q.arg2 ="0";
        }
        iCode.push_back(q);

        input >> ws;
    }
    input.close();
    return true;
}

bool MCodeGenerator::MachineCodeGenerate(string name){
    ofstream output;

    cout << "Output Machine Code" << endl;
    output.open(name.c_str());
    if(!output){
        cout << "Output Machine Code failed." << endl;
        return false;
    }

    unsigned int i;
    for(i = 0; i < iCode.size(); ++i){
        switch( opType[iCode[i].op] ){
        case 0:
            output << i << ": " << op[ iCode[i].op ] << " "
                << Translate(iCode[i].result) << ","
                << Translate(iCode[i].arg1) << ","
                << Translate(iCode[i].arg2) << endl ;
            break;
        case 1:
            output << i << ": " << op[ iCode[i].op ] << " "
                << Translate(iCode[i].result) << ","
                << Translate(iCode[i].arg1) << "("
                << Translate(iCode[i].arg2) << ")" <<endl ;
            break;
        }
    }
    output << i << ": OUT " << Translate(iCode[i-1].result) << ",0,0" <<endl;
    i++;
    output << i << ": HALT 1,0,0" << endl;
    return true;
}
string MCodeGenerator::Translate(string s){
    //digit
    bool pass = true;
    for(unsigned int i = 0; i < s.size(); ++i){
        if( !isdigit(s[i]) ){
            pass = false;
            break;
        }
    }
    if(pass){
        return s;
    }
    //register
    for(int i = 0; i < tempNum; ++i){
        if(s == temp[i]){
            stringstream ss;
            ss << i;
            return ss.str();
        }
    }
    //variable
    return symbolTable[s];
}
