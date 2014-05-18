#include "ParserGenerator.h"

ParserGenerator::ParserGenerator(){
    epsilon = "epsilon";
}

bool ParserGenerator::ReadGrammar(string name){
    cout <<"Reading Grammar" << endl;
    //open grammar file
    ifstream grammar;//grammar
    grammar.open(name.c_str());
    if(!grammar){
        cout << "no grammar" << endl;
        return false;
    }

    //read grammar
    string line;
    string head;
    string word;
    istringstream line_buf;

    //current production iterator
    map<string, Nonterminal>::iterator nonterminal;

    while(grammar.good()){
        getline(grammar, line);  //從input取得一行
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
        if(!isblank(line[0])){
            line_buf.str(line);    //輸入buffer
            line_buf >> head;//獲取nonterminal
            nonorder.push_back(head);
            nonterminals[head].head = head;
            nonterminal = nonterminals.find(head);
            line_buf.clear();     //清除狀態

            //initalize map
            nonterminal->second.nullable = false;
            nonterminal->second.visit = false;
        }
        //body
        else{
            line_buf.str(line);
            vector<string> production;
            set<string> first;
            //get production
            while(line_buf.good()){
                line_buf >> word;//取得一字
                production.push_back(word);//放進grammar裡
                line_buf >> ws;//清除行尾空白
            }
            //production
            nonterminal->second.body.push_back(production);
            //nullable
            if(production[0] == epsilon){
                nonterminal->second.bodynullable.push_back(true);
                nonterminal->second.nullable = true;
            }else{
                nonterminal->second.bodynullable.push_back(false);
            }
            //first
            nonterminal->second.bodyfirst.push_back(first);
            //clear buffer
            line_buf.clear();
        }
    }
    grammar.close();
    return true;
}

void ParserGenerator::PrintGrammar(){
    map<string, Nonterminal>::iterator nonterminal;

    cout << boolalpha;
    for(nonterminal = nonterminals.begin(); nonterminal!= nonterminals.end(); ++ nonterminal){
        cout << nonterminal->first << "\t" <<nonterminal->second.nullable <<endl;
        for(unsigned int i = 0; i < nonterminal->second.body.size(); ++i){
                for(unsigned int j = 0; j < nonterminal->second.body[i].size(); ++j){
                    cout << "\t" <<nonterminal->second.body[i][j];
                }
                cout << "\t| " <<nonterminal->second.bodynullable[i] <<endl;
        }
    }
    cout << noboolalpha;
}

bool ParserGenerator::PrintNullFirstFollow(string name){
    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    output << boolalpha;
    map<string, Nonterminal>::iterator nonterminal;
    set<string>::iterator it_string;

    //nullable
    for(nonterminal = nonterminals.begin(); nonterminal!= nonterminals.end(); ++ nonterminal){
        output << nonterminal->first << "\t" <<nonterminal->second.nullable <<endl;
    }
    output << endl;
    output << noboolalpha;

    //first
    for(nonterminal = nonterminals.begin(); nonterminal!= nonterminals.end(); ++ nonterminal){
        output << nonterminal->first << "\t";
        for(it_string= nonterminal->second.first.begin() ; it_string != nonterminal->second.first.end(); ++it_string)
            output <<  *it_string <<  " ";
        output << endl;
    }
    output << endl;

    //follow
    for(nonterminal = nonterminals.begin(); nonterminal!= nonterminals.end(); ++ nonterminal){

        output << nonterminal->first << "\t";
        for(it_string= nonterminal->second.follow.begin() ; it_string != nonterminal->second.follow.end(); ++it_string)
            output <<  *it_string <<  " ";
        output << endl;
    }

    output.close();
    return true;
}

bool ParserGenerator::ConstructNullFirstFollow(string name){
    cout << "Construct Nullable"<<endl;
    nullable_test();
    cout << "Construct First"<<endl;
    first_test();
    cout << "Construct Follow"<<endl;
    follow_test();
    bool success = PrintNullFirstFollow(name);
    if(!success)
        cout << "Construct Nullable First Follow set failed" << endl;
    return success;
}

void ParserGenerator::nullable_test(){
    //it->first = nonterminal, it->second = Nonterminal 節點
    //current nonterminal
    map<string, Nonterminal>::iterator nonterminal;

    //每個元素走訪

    bool change = true;
    while(change){
        change = false;
        for(nonterminal = nonterminals.begin(); nonterminal != nonterminals.end(); ++nonterminal){
            if(null_iter(nonterminal))
                change = true;
        }
    }

}

bool ParserGenerator::null_iter(map<string, Nonterminal>::iterator nonterminal){

    map<string, Nonterminal>::iterator body;
    //每個元素走訪
    bool change = false;
    for(unsigned int i = 0; i < nonterminal->second.body.size(); ++i){
        //already nullable
        if(nonterminal->second.bodynullable[i]){
            continue;
        }
        //test nullable
        bool pass = true;
        for(unsigned int j = 0; j < nonterminal->second.body[i].size(); ++j){
            body = nonterminals.find(nonterminal->second.body[i][j]);
            //terminal (no epsilon condition)
            if( body == nonterminals.end() ){
                pass = false;
                break;
            }
            //nonterminal
            else{
                if(!body->second.nullable){
                    pass = false;
                    break;
                }
            }
        }
        //如果全部pass, nullable
        if(pass){
            nonterminal->second.nullable = true;
            nonterminal->second.bodynullable[i] = true;
            change = true;
        }
    }
    return change;
}


void ParserGenerator::first_test(){
    map<string, Nonterminal>::iterator nonterminal;

    //每個元素走訪
    bool change = true;
    while(change){
        change = false;
        for(nonterminal = nonterminals.begin(); nonterminal != nonterminals.end(); ++nonterminal){
            if(first_iter(nonterminal))
                change = true;
        }
    }
}


bool ParserGenerator::first_iter(map<string, Nonterminal>::iterator nonterminal){
    map<string, Nonterminal>::iterator body_nonterminal;
    //每個元素走訪
    bool change = false;
    for(unsigned int i = 0; i < nonterminal->second.body.size(); ++i){
        //no epsilon
        if(nonterminal->second.body[i][0] == epsilon)
            continue;

        //insert first
        for(unsigned int j = 0; j < nonterminal->second.body[i].size(); ++j){
            string word = nonterminal->second.body[i][j];
            body_nonterminal = nonterminals.find(word);

            //terminal
            if( body_nonterminal == nonterminals.end() ){
                //if word not insert, insert it
                if( nonterminal->second.bodyfirst[i].count( word ) == 0 ){
                    nonterminal->second.bodyfirst[i].insert( word );
                    nonterminal->second.first.insert( word );
                    change = true;
                }
                break;
            }
            //nonterminal
            else{
                if( !subset( nonterminal->second.bodyfirst[i] ,  body_nonterminal->second.first)  ){
                    nonterminal->second.bodyfirst[i].insert( body_nonterminal->second.first.begin(), body_nonterminal->second.first.end() );
                    nonterminal->second.first.insert( body_nonterminal->second.first.begin(), body_nonterminal->second.first.end() );
                    change = true;
                }
                //if word isn't nullable, break
                if(!body_nonterminal->second.nullable){
                    break;
                }
            }
        }

    }
    return change;
}

bool ParserGenerator::subset(set<string> & s1,set<string> & s2){
    //empty subset is anyone's subset
    if(s2.empty()){
        return true;
    }

    set<string>::iterator it_s2 = s2.begin();
    for (;it_s2!=s2.end(); ++it_s2){
        if(s1.count(*it_s2) == 0){
            return false;
        }
    }
    return true;
}



void ParserGenerator::follow_test(){
    //add first
    follow_add_first();

    map<string, Nonterminal>::iterator head;
    set<string>::iterator followsymbol;
    map<string, Nonterminal>::iterator follow;
    set<string>::iterator insertsymbol;

    //每個元素走訪
    bool change = true;
    while(change){
        change = false;
        for(head = nonterminals.begin(); head != nonterminals.end(); ++head){
            //add nonterminal's terminal follow set into head terminal follow set
            for(followsymbol = head->second.follow_non.begin();followsymbol != head->second.follow_non.end() ;++followsymbol){
                follow = nonterminals.find(*followsymbol);
                for(insertsymbol = follow->second.follow.begin(); insertsymbol!= follow->second.follow.end(); ++ insertsymbol){
                    if( !head->second.follow.count(*insertsymbol) ){
                        head->second.follow.insert(*insertsymbol);
                        change = true;
                    }
                }
            }
        }
    }
}


void ParserGenerator::follow_add_first(){
    //it->first = nonterminal, it->second = Nonterminal 節點
    map<string, Nonterminal>::iterator head;
    map<string, Nonterminal>::iterator abody;
    map<string, Nonterminal>::iterator bbody;
    bool check;

    //head ->.. abody bbody
    //每個nonterminal走訪
    for(head = nonterminals.begin(); head != nonterminals.end(); ++head){
        //每個production走訪
        for(unsigned int i = 0; i < head->second.body.size(); ++i){
            //同一production的每個字走訪
            for(unsigned int j = 0; j < head->second.body[i].size(); ++j){
                abody = nonterminals.find( head->second.body[i][j] );
                //若為nonterminal 才繼續
                if(abody != nonterminals.end()){
                    check = true;
                    //若後面還有symbol 加入此symbol 的first
                    for(unsigned int k = j+1; k < head->second.body[i].size(); ++k){
                        //測試為terminal或nonterminal
                        bbody = nonterminals.find( head->second.body[i][k] );
                        //若後面為nonterminal
                        if(bbody !=  nonterminals.end() ){
                            //加入first
                            abody->second.follow.insert( bbody->second.first.begin(), bbody->second.first.end() );
                            //非nullable跳出迴圈
                            if(bbody->second.nullable == false){
                                check = false;
                                break;
                            }
                        }
                        //後面為terminal
                        else{
                            abody->second.follow.insert(head->second.body[i][k]);
                            check = false;
                            break;
                        }
                    }
                    //若此nontermianl的後面皆為nullable, follow(it) 屬於follow(it2)
                    if(check){
                        abody->second.follow_non.insert(head->first);
                    }
                }
            }
        }
    }
}

bool ParserGenerator::GenerateLLTable(string name){
    bool success;
    success = create_table();
    print_table(name);

    if(!success){
        cout << "Grammar Conflict, maybe left recursion or left factorying or other problem." << endl;
    }
    return success;
}

bool ParserGenerator::create_table(){
    map<string, Nonterminal>::iterator head;
    set<string>::iterator terminal;
    vector<string>::iterator nonterminal;
    bool conflict = false;
    //elements traversal
    for(nonterminal = nonorder.begin(); nonterminal != nonorder.end(); ++nonterminal){
        head = nonterminals.find(*nonterminal);
        //add nonterminal's terminal follow set into head terminal follow set
        for(unsigned int i = 0; i < head->second.body.size(); ++i){
            //add first
            for(terminal = head->second.bodyfirst[i].begin(); terminal!= head->second.bodyfirst[i].end(); ++terminal){
                //conflict
                if(LLtable[head->first].count(*terminal)){
                    cout << "conflict:"<<head->first <<" " << *terminal << endl;
                    conflict = true;
                }
                else{
                    for(unsigned int j = 0; j < head->second.body[i].size(); ++j)
                        LLtable[head->first][*terminal].push_back(head->second.body[i][j]);
                }
            }
            //nullable, add follow
            if(head->second.bodynullable[i]){
                for(terminal = head->second.follow.begin(); terminal!= head->second.follow.end(); ++terminal){
                    //conflict
                    if(LLtable[head->first].count(*terminal)){
                        cout << "conflict:"<<head->first <<" " << *terminal << endl;
                        conflict = true;
                    }
                    else{
                        for(unsigned int j = 0; j < head->second.body[i].size(); ++j)
                            LLtable[head->first][*terminal].push_back(head->second.body[i][j]);
                    }
                }
            }
        }
    }
    return !conflict;
}

bool ParserGenerator::print_table(string name){
    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;

    map<string, map<string, vector<string> > >::iterator nonterminal;
    map<string, vector<string> >::iterator terminal;

    output << left <<nonorder[0] << " " << epsilon << endl;
    for(nonterminal = LLtable.begin(); nonterminal!= LLtable.end(); ++ nonterminal){
        for(terminal = nonterminal->second.begin(); terminal != nonterminal->second.end(); ++terminal){
            output << setw(20) <<nonterminal->first << setw(10) << terminal->first;
            for(unsigned int i = 0; i < terminal->second.size(); ++i){
                output << " " << terminal->second[i];
            }
            output << endl;
        }
    }


    output.close();
    return true;
}
