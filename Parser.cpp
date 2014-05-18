#include "Parser.h"


bool Parser::ReadLLTable(string name){
    cout << "Read LL(1) Parsing table." << endl;

    ifstream LLfile;
    LLfile.open(name.c_str());
    if(!LLfile){
        cout << "Read LL(1) Parsing table failed." << endl;
        return false;
    }

    //read grammar
    string line;
    string nonterminal;
    string terminal;
    string word;
    istringstream line_buf;

    //start symbol
    LLfile >> startsymbol >> epsilon >> ws;
    while(LLfile.good()){
        getline(LLfile, line);  //從input取得一行
        //文件尾端空白行
        bool exit = true;
        for(unsigned int i = 0; i < line.size(); ++i){
            if( !isblank(line[i]) ){
                exit = false;
                break;
            }
        }
        if(exit) break;

        //head
        line_buf.str(line);
        line_buf >> nonterminal >> terminal;
        while(line_buf.good()){
            line_buf >> word;//取得一字
            LLtable[nonterminal][terminal].push_back(word);//放進grammar裡
            line_buf >> ws;//清除行尾空白
        }

        line_buf.clear();
    }

    LLfile.close();
    return true;
}

bool Parser::ReadToken(string name){
    ifstream Tokenfile;

    cout << "Read Token." << endl;

    Tokenfile.open(name.c_str());
    if(!Tokenfile){
        cout << "Read Token failed." << endl;
        return false;
    }


    //read grammar
    string line;
    istringstream line_buf;

    //current production iterator
    while(Tokenfile.good()){
        Token token;
        //read token
        Tokenfile >> token.token;
        //read data
        if(token.token == "num" || token.token == "id" ){
            Tokenfile >> token.data;
        }
        input.push_back(token);
        Tokenfile >> ws;
    }
    Token token;
    token.token = "$";
    input.push_back(token);

    Tokenfile.close();
    return true;
}

bool Parser::LLParser(){
    bool success;
    unsigned int i = 0;//input position

    cout << "LL(1) parsing" << endl;
    root = new Node(startsymbol, 1);
    success = LLParser_recu(i, root);

    if(!success){
        cout << "LL(1) parsing error" << endl;
        cleartree(root);
        root = NULL;
    }
    return success;
}

bool Parser::LLParser_recu(unsigned int &i, Node* cur){
    //cout << cur->depth << " " << cur-> head << endl;

    //epsilon no derivation
    if(cur->head == epsilon){
        return true;
    }
    if(i >= input.size() ){
        return false;
    }
    else if(cur->head == input[i].token){
        if( (cur->head == "id") || (cur->head == "num") ){
            cur->body.push_back( input[i].data );
            Node* x = new Node(input[i].data , cur->depth+1);
            cur->pbody.push_back(x);
        }
        ++i;
        return true;
    }


    map<string, map<string, vector<string> > >::iterator isnonterminal;
    map<string, vector<string> >::iterator production;

    //terminal and can't not parse , error
    isnonterminal = LLtable.find(cur->head);
    if(isnonterminal == LLtable.end() ){
        return false;
    }
    //this field is empty, error
    production = isnonterminal->second.find(input[i].token);
    if(production == LLtable[cur->head].end()){
        return false;
    }

    //parsing tree
    for(unsigned int j = 0; j < production->second.size(); ++j){
        cur->body.push_back(production->second[j]);
        Node* x = new Node(production->second[j] , cur->depth+1);
        cur->pbody.push_back(x);
        if( !LLParser_recu(i, x) ){
            return false;
        }
    }
    return true;
}

void Parser::cleartree(Node* cur){
    for(unsigned int i = 0; i < cur->pbody.size(); ++i){
        cleartree(cur->pbody[i]);
    }
    delete cur;
}

bool Parser::PrintStep(string name){
    cout << "print parsing procedure" << endl;
    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    PrintStep_recu(output, root);

    output.close();
    return true;
}

void Parser::PrintStep_recu(ofstream &output, Node* cur){
    if(cur->pbody.empty())
        return;

    output << cur->head << " ->";
    for(unsigned int i = 0; i < cur->body.size(); ++i){
        output << " " << cur->body[i];
    }
    output << endl;

    for(unsigned int i = 0; i < cur->pbody.size(); ++i){
        PrintStep_recu(output, cur->pbody[i]);
    }
}

bool Parser::PrintTree(string name){
    cout << "print parsing tree" << endl;

    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    PrintTree_recu(output, root);

    output.close();
    return true;
}

void Parser::PrintTree_recu(ofstream &output, Node* cur){
    output << setw(cur->depth*2-1)<<cur->depth << " " << cur->head << endl;
    for(unsigned int i = 0; i < cur->pbody.size(); ++i){
        PrintTree_recu(output, cur->pbody[i]);
    }
}
