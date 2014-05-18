#include "CodeGenerator.h"
CodeGenerator::CodeGenerator(){
    root = NULL;
    epsilon = "epsilon";

    vType[0] = "int";
    vType[1] = "char";

    vType2[0] = "function";
    vType2[1] = "variable";
    vType2[2] = "array";

    temp[0] = "t0";
    temp[1] = "t1";
    temp[2] = "t2";
    temp[3] = "t3";
    temp[4] = "t4";
    temp[5] = "t5";
    temp[6] = "t6";
    temp[7] = "t7";

    Op[0] = "NO_OP" ;
    Op[1] = "=" ;
    Op[2] = "+" ;
    Op[3] = "-" ;
    Op[4] = "*" ;
    Op[5] = "/" ;
    Op[6] = ">" ;
    Op[7] = "<" ;
    Op[8] = ">=" ;
    Op[9] = "<=" ;
    Op[10] = "==" ;
    Op[11] = "!=" ;
    Op[12] = "||" ;
    Op[13] = "&&" ;
}

CodeGenerator::~CodeGenerator(){
    if(root != NULL)
        destroy(root);
}
void CodeGenerator::destroy(Node* node){
    for(unsigned int i = 0; i < node->pbody.size(); i++){
        destroy(node->pbody[i]);
    }
    delete node;
}



bool CodeGenerator::ReadParsingtree(string name){
    cout << "Read Parsing tree" << endl;

    ifstream input;
    input.open(name.c_str());
    if(!input){
        cout << "Read Parsing tree failed." << endl;
        return false;
    }

    int depth; //current depth
    int maxScope = 0; // max scope
    stack<int> stackScope; //scope stack
    SymbolTable globaltable(0,-1);
    string symbol;

    stackScope.push(maxScope);
    symboltable.push_back(globaltable);

    //create root node
    input >> depth >> symbol;
    root = new Node(symbol, depth, stackScope.top() );

    //create node recursive
    input >> depth;
    TreeRecu(input, root, depth, stackScope, maxScope);

    input.close();
    return true;
}

int CodeGenerator::TreeRecu(ifstream &input, Node* head, int depth, stack<int>& stackScope,int &maxScope){
    int nextDepth;
    string symbol;

    while(true){
        input >> symbol;
        if(symbol == "{"){
            ++maxScope;
            //new symbol table
            SymbolTable x(maxScope, stackScope.top() );
            symboltable.push_back(x);
            //stack
            stackScope.push(maxScope);
        }

        head->body.push_back(symbol);
        Node* cur = new Node(symbol, depth, stackScope.top());
        head->pbody.push_back(cur);

        if(symbol == "}"){
            stackScope.pop();
        }

        //end of file
        input >> ws;
        if(!input.good())
            return 0;

        input >> nextDepth;
        if(nextDepth > depth){
            nextDepth = TreeRecu(input, cur, nextDepth, stackScope, maxScope);
        }
        if(nextDepth == depth){
            continue;
        }
        if(nextDepth < depth){
            return nextDepth;
        }
    }
}


bool CodeGenerator::CreateSymbolTable(string name){
    cout << "Create Symbol Table" << endl;
    bool pass;
    pass = CreateSymbolTableRecu(root);

    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    for(unsigned int i= 0; i < symboltable.size(); ++i){
        map<string , Entry>::iterator it;
        for(it = symboltable[i].entry.begin(); it != symboltable[i].entry.end(); ++it){
            output << it->second.symbol <<"\t"<< vType2[it->second.type2] <<"\t" << vType[it->second.type] <<"\t" << it->second.scope << endl;
        }
    }
    output.close();
    if(!pass){
        cout << "Symbol double declare" << endl;
    }
    return pass;
}

bool CodeGenerator::CreateSymbolTable2(string name){
    cout << "Create Symbol Table for tiny machine code generator" << endl;

    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    int start = 1;
    for(unsigned int i= 0; i < symboltable.size(); ++i){
        map<string , Entry>::iterator it;
        for(it = symboltable[i].entry.begin(); it != symboltable[i].entry.end(); ++it){
            if(it->second.type2){
                string symbol;
                symbol = RenameId(it->second.symbol, it->second.scope);
                output << symbol << " " << start << endl;
                if(it->second.type2 == 1){
                    start++;
                }
                else if(it->second.type2 == 2){
                    stringstream ss;
                    int size;
                    ss << it->second.arraySize;
                    ss >> size;
                    start += size;
                }
            }
        }
    }
    output.close();
    return true;
}


void CodeGenerator::ChangeScope(Node* node){
    for(unsigned int i = 0; i < node->pbody.size(); ++i){
        node->pbody[i]->scope = node->scope;
        ChangeScope(node->pbody[i]);
    }
}

bool CodeGenerator::CreateSymbolTableRecu(Node* node){
    bool pass = true;
    //FunDecl => ( ParamDeclList ) Block
    //Block => { VarDeclList StmtList }
    if(node->head == "FunDecl"){
        //ParamDeclList scope = block scope
        node->pbody[1]->scope = node->pbody[3]->pbody[0]->scope;
        ChangeScope(node->pbody[1]);
    }


    for(unsigned int i = 0; i < node->pbody.size(); ++i){
        bool p;
        p = CreateSymbolTableRecu(node->pbody[i]);
        if(p == false)
            pass = false;
    }

    if(node->head == "DeclList'"){
        //DeclList' => Type id Decl
        //create symbol entry
        Entry x;
        x.token = "id";
        x.type = node->pbody[0]->type;
        x.symbol = node->pbody[1]->id;
        x.type2 = node->pbody[2]->type;
        x.scope = node->scope;
        x.arraySize = node->pbody[2]->temp;
        if( symboltable[x.scope].entry.count(x.symbol) ){
            cout << x.symbol << " already declare" << endl;
            pass = false;
        }
        else{
            symboltable[x.scope].entry[x.symbol] = x;
        }
    }
    else if(node->head == "Type"){
        //Type => int
        //Type => char
        node->type = node->pbody[0]->type;
    }
    else if(node->head == "int"){
        node->type = 0;
    }
    else if(node->head == "char"){
        node->type = 1;
    }
    else if(node->head == "id"){
        node->id = node->body[0];
    }
    else if(node->head == "Decl"){
        //Decl => VarDecl'
        //Decl => FunDecl
        node->type = node->pbody[0]->type;
        node->temp = node->pbody[0]->temp;
    }
    else if(node->head == "VarDecl'"){
        //VarDecl' => ;
        if(node->body[0] == ";"){
            node->type = 1; //variable
        }
        //VarDecl' => [ num ] ;
        else{
            node->type = 2; //array
            node->temp = node->pbody[1]->body[0];
        }
    }
    else if(node->head == "VarDecl"){
        //VarDecl => Type id VarDecl'
        Entry x;
        x.token = "id";
        x.type = node->pbody[0]->type;
        x.symbol = node->pbody[1]->id;
        x.type2 = node->pbody[2]->type;
        x.scope = node->scope;
        x.arraySize = node->pbody[2]->temp;
        if( symboltable[x.scope].entry.count(x.symbol) ){
            cout << x.symbol << " already declare" << endl;
            pass = false;
        }
        else{
            symboltable[x.scope].entry[x.symbol] = x;
        }
    }
    else if(node->head == "FunDecl"){
        //FunDecl => ( ParamDeclList ) Block
        node->type = 0;
    }
    else if(node->head == "ParamDecl"){
        //ParamDecl => Type id ParamDecl'
        Entry x;
        x.token = "id";
        x.type = node->pbody[0]->type;
        x.symbol = node->pbody[1]->id;
        x.type2 = node->pbody[2]->type;
        x.scope = node->scope;
        if( symboltable[x.scope].entry.count(x.symbol) ){
            cout << x.symbol << " already declare" << endl;
            pass = false;
        }
        else{
            symboltable[x.scope].entry[x.symbol] = x;
        }
    }
    else if(node->head == "ParamDecl'"){
        //ParamDecl' => [ ]
        //ParamDecl' => epsilon
        if(node->body[0] == "[")
            node->type = 2;
        else if(node->body[0] == epsilon)
            node->type = 1;
    }

    return pass;
}

void CodeGenerator::PrintTree(){
    cout << "print parsing tree" << endl;
    PrintTree_recu(root);
}

void CodeGenerator::PrintTree_recu(Node* cur){
    cout << setw(cur->scope*2-1) << cur->scope << " " << cur->head << endl;
    for(unsigned int i = 0; i < cur->pbody.size(); ++i){
        PrintTree_recu(cur->pbody[i]);
    }
}

bool CodeGenerator::SemanticRule(string name){
    cout << "Semantic Rule" << endl;
    bool pass;
    pass = Stmt(root);

    if(!pass){
        cout << "semantic Rule failed" << endl;
        return false;
    }
    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    vector<Quadruple>::iterator it;

    for(it= quadruples.begin(); it!=quadruples.end(); it++){
        output << it->op << " "<< it->result<< " "<< it->arg1 << " " << it->arg2 << endl;
    }
    return pass;
}

bool CodeGenerator::Stmt(Node* node){
    bool pass = true;
    //Traversal
    if(node->head == "Expr"){
        pass = Expr(node);
        return pass;
    }
    else if(node->head == "Stmt"){
        tempC.UnlockAllTemp();


        //Stmt => break ;
        //Stmt => if ( Expr ) Stmt else Stmt
        if(node->body[0] == "if"){
            pass = Expr(node->pbody[2]);

            string t1, t2;
            stringstream ss;
            int jumpLine, ifEnd, elseEnd;
            //if else choose
            t1 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            PushQuadruple("JEQ", node->pbody[2]->temp, "", t1);
            jumpLine = quadruples.size()-1;

            //if
            pass = Stmt(node->pbody[4]);
            //if end
            t1 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            t2 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            PushQuadruple("JEQ", t2, "", t1);
            ifEnd = quadruples.size()-1;

            //else
            pass = Stmt(node->pbody[6]);
            //else end
            elseEnd = quadruples.size()-1;
            //jump
            ss << ifEnd+1;
            quadruples[jumpLine].arg1 = ss.str();
            ss.str("");
            ss << elseEnd+1;
            quadruples[ifEnd].arg1 = ss.str();
        }
        //Stmt => while ( Expr ) Stmt
        else if(node->body[0] == "while"){
            string t1, t2;
            stringstream ss;
            int whileInit, jumpLine;

            whileInit = quadruples.size();
            pass = Expr(node->pbody[2]);

            t1 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            PushQuadruple("JEQ", node->pbody[2]->temp, "", t1);
            jumpLine = quadruples.size() -1;

            pass = Stmt(node->pbody[4]);
            t1 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            t2 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            ss << whileInit;
            PushQuadruple("JEQ", t2, ss.str(), t1);

            ss.str("");
            ss << quadruples.size();
            quadruples[jumpLine].arg1 = ss.str();
        }
        //Stmt => return Expr ;

        //Stmt => Block
        //Stmt => ;
        //Stmt => Expr ;
        else {
            for(unsigned int i = 0; i < node->pbody.size(); ++i){
                pass = Stmt(node->pbody[i]);
            }
        }
        return pass;
    }

    for(unsigned int i = 0; i < node->pbody.size(); ++i){
        pass = Stmt(node->pbody[i]);
    }
    return pass;
}

bool CodeGenerator::Expr(Node* node){
    bool pass = true;
    if(node->head == "Expr"){
        //Expr => UnaryOp Expr
        if(node->body[0] == "UnaryOp"){
            for(unsigned int i = 0; i < node->pbody.size(); ++i){
                pass = Expr(node->pbody[i]);
                if(!pass)return false;
            }

            //insert instruction
            if(node->pbody[0]->op == "-"){
                string t;
                t = PushQuadruple("=", tempC.FindTemp(), "0", "");
                node->temp = PushQuadruple(node->pbody[0]->op , tempC.FindTemp(), t, node->pbody[1]->temp);
            }
            else{
                node->temp = PushQuadruple(node->pbody[0]->op , tempC.FindTemp(), node->pbody[1]->temp, "");
            }

        }
        //Expr => num Expr'
        else if(node->body[0] == "num"){
            node->pbody[0]->expr = true;
            pass = Expr(node->pbody[0]);//num
            if(!pass)return false;
            pass = Expr(node->pbody[1]);//Expr'
            if(!pass)return false;


            //insert instruction
            node->temp = PushQuadruple(node->pbody[1]->op, tempC.FindTemp(), node->pbody[0]->temp, node->pbody[1]->temp);
        }
        //Expr => ( Expr ) Expr'
        else if(node->body[0] == "("){
            pass = Expr(node->pbody[1]);//Expr
            if(!pass)return false;
            pass = Expr(node->pbody[3]);//Expr'
            if(!pass)return false;

            //insert instruction
            node->temp = PushQuadruple(node->pbody[3]->op, tempC.FindTemp(), node->pbody[1]->temp,  node->pbody[3]->temp);
        }
        //Expr => id ExprIdTail
        else if(node->body[0] == "id"){
            node->pbody[0]->expr = true;
            pass = Expr(node->pbody[0]);
            if(!pass)return false;

            node->pbody[1]->id = node->pbody[0]->id;
            node->pbody[1]->type = node->pbody[0]->type;
            pass = Expr(node->pbody[1]);
            if(!pass)return false;
            node->temp = node->pbody[1]->temp;
        }
        return pass;
    }
    else if(node->head == "ExprIdTail"){
        //ExprIdTail => Expr'
        if(node->body[0] == "Expr'"){

            //not variable type
            if(node->type != 1){
                cout << node->id <<" is not a variable." << endl;
                return false;
            }

            //recursive
            pass = Expr(node->pbody[0]);
            if(!pass)return false;

            //load memory
            string  t1, t2;
            t1 = PushQuadruple("=", tempC.FindTemp(), "0", "");
            t2 = PushQuadruple("=", tempC.FindTemp(), node->id, t1);

            //instruction
            node->temp = PushQuadruple(node->pbody[0]->op, tempC.FindTemp(), t2, node->pbody[0]->temp);
        }
        //ExprIdTail => = Expr
        else if(node->body[0] == "="){
            if(node->type != 1){
                cout << node->id <<" is not a variable." << endl;
                return false;
            }
            //recursive
            pass = Expr(node->pbody[1]);
            if(!pass)return false;

            //zero register
            string t;
            t = PushQuadruple("=", tempC.FindTemp(), "0", "");
            //save memory
            node->temp = PushQuadruple("s=", node->pbody[1]->temp, node->id, t);
        }

        //ExprIdTail => [ Expr ] ExprArrayTail
        else if(node->body[0] == "["){
            if(node->type != 2){
                cout << node->id <<" is not an array." << endl;
                return false;//not array type
            }

            pass = Expr(node->pbody[1]);//recursive
            if(!pass)return false;
            pass = Expr(node->pbody[3]);//recursive
            if(!pass)return false;

            //save memory
            if(node->pbody[3]->op == "="){
                node->temp = PushQuadruple("s=" , node->pbody[3]->temp, node->id, node->pbody[1]->temp);
            }
            else{
                //read array
                string t;
                t = PushQuadruple("=", tempC.FindTemp(), node->id, node->pbody[1]->temp);

                //operate
                node->temp = PushQuadruple(node->pbody[3]->op , tempC.FindTemp(), t, node->pbody[3]->temp);
            }
        }
        return pass;
    }
    else if(node->head == "ExprArrayTail"){
        //ExprArrayTail => Expr'
        if(node->body[0] == "Expr'"){
            //recursive
            pass = Expr(node->pbody[0]);
            if(!pass)return false;

            node->op = node->pbody[0]->op;
            node->temp = node->pbody[0]->temp;
        }
        //ExprArrayTail => = Expr
        else if(node->body[0] == "="){
            //recursive
            pass = Expr(node->pbody[1]);
            if(!pass)return false;

            node->op = "=";
            node->temp = node->pbody[1]->temp;
        }
        return pass;
    }
    else if(node->head == "Expr'"){
        //Expr' => BinOp Expr
        if(node->body[0] == "BinOp"){
            node->op = node->pbody[0]->body[0];
            pass = Expr(node->pbody[1]);
            node->temp = node->pbody[1]->temp;
        }
        //Expr' => epsilon
        if(node->body[0] == epsilon){
            node->op = "NO_OP";
        }
        return pass;
    }
    else if( node->head == "id"){
        if(!node->expr)return true;

        map<string , Entry>::iterator it;
        for(int scope = node->scope; scope!=-1 ;  scope = symboltable[scope].pScope){
            it = symboltable[scope].entry.find( node->body[0] );//find symbol
            if( it != symboltable[scope].entry.end() ){
                node->type = it->second.type2;
                node->id = RenameId(node->body[0] , scope);
                return true;
            }
        }
        cout << "Cannot find " << node->body[0] << "in scope "<< node->scope << endl;
        return false;
    }
    else if( node->head == "num"){
        if(!node->expr){
            return true;
        }
        else{
            node->temp = PushQuadruple("=", tempC.FindTemp(), node->body[0], "");
            return true;
        }
    }
}


string CodeGenerator::RenameId(string id, int scope){
    stringstream ss;
    string output;
    ss << "s" << scope << "_" << id;
    ss >> output;
    return output;
}

string CodeGenerator::PushQuadruple(string op, string result, string arg1, string arg2){
    if(op == "NO_OP"){
        tempC.UnlockTemp(result);
        tempC.UnlockTemp(arg2);
        return arg1;
    }

    //push Quadruple
    qInst.NewQuadruple(op , result, arg1, arg2);
    quadruples.push_back(qInst);

    if(tempC.IsTemp(result)){
        tempC.UnlockTemp(arg1);
        tempC.UnlockTemp(arg2);
        return result;
    }
    else{
        tempC.UnlockTemp(arg1);
        tempC.UnlockTemp(result);
        return arg2;
    }

}
