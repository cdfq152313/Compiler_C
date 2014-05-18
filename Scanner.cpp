#include "Scanner.h"

bool Scanner::open(string name){
    jfile.open(name.c_str());
    if(!jfile){
        cout << "no input .c file" << endl;
        return false;
    }

    return true;
}

void Scanner::print(int type){
    //error & Comment
    if(type == 8 || type == 9){
        if(type == 8)
            success = false;
        word = "";
        return;
    }
    else{
        output2 << "\t" << setw(13) << left << category[type] << ": " << word << endl;
        output2 << right;
        word = "";
    }
}

bool Scanner::PrinttoParser(string name){
    ofstream output;
    output.open(name.c_str());
    if(!output)
        return false;
    for(unsigned int i = 0; i < tokens.size(); ++i){
        //number
        if(tokens[i].type == 10){
            output <<  "num " << tokens[i].token << endl;
        }
        //character
        else if(tokens[i].type == 4){
            output <<  "char " << tokens[i].token << endl;
        }
        //Identifier
        else if(tokens[i].type == 3){
            output <<  "id " << tokens[i].token << endl;
        }
        else{
            output <<  tokens[i].token << endl;
        }
    }
    return true;
}

void Scanner::print_line(){
    output << "Line "
        << setw(4) << cur_line
        << setw(2) << scope.top()
        << " " + line
        << endl;
}


void Scanner::close(){
    while (!scope.empty())
    {
        scope.pop();
    }
    output.close();
    output.clear();
    output2.close();
    output2.clear();
    jfile.close();
    jfile.clear();
}


bool Scanner::start(string name){
    //開啟輸出檔
    cout << "start scanner" << endl;
    output2.open(name.c_str());
    if(!output2)
        return false;
    //initalize
    cur_line = 0;
    scope_num = 0;
    scope.push(0);
    comment = false;
    success = true;

    //start
    while(jfile.good()){
        cur_line++;
        //取出一行
        getline(jfile, line);
        //印出一行
        output2 << "Line" << setw(3) << cur_line << " :" << endl;

        //放進string buffer裡
        line_buf.clear();
        line_buf.str(line);
        //忽略行前空白
        line_buf >> ws;

        while(line_buf.good()){
            //分析一個字
            analyzer();

            if(comment){
                //單行註解
                comment = false;
                break;
            }

            //忽略中間空白
            line_buf >> ws;
        }
    }

    close();
    if(!success){
        cout << "lexical error" << endl;
    }
    return success;
}

void Scanner::analyzer(){
    char temp;//儲存一個字元
    Token token;

    //測試是否為char 和 string 和註解
    temp = line_buf.peek();

    //string
    if(temp == '"'){
        //將第一個雙引號存入
        word = line_buf.get();
        //無偵測到跳脫字元
        escape = false;

        //當檔案尚未結尾
        while((temp = line_buf.get()) != EOF){
            //存入
            word.push_back(temp);
            //若遇到雙引號結尾 且前一字非跳脫字元則中止
            if(temp == '"' && !escape ){
                token.token = word;
                token.line = cur_line;
                token.type = 5;
                tokens.push_back(token);
                print(token.type);
                return ;
            }

            //消除跳脫字元影響
            if(escape){
                escape = false;
            }
            //偵測跳脫字元
            else if(temp == '\\'){
                escape = true;
            }
        }
        //若進行到此處代表沒有雙引號結尾 錯誤
        token.token = word;
        token.line = cur_line;
        token.type = 8;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //char
    if(temp == '\''){
        //將第一個單引號存入
        word = line_buf.get();
        //無偵測到跳脫字元
        escape = false;

        //當檔案尚未結尾
        while((temp = line_buf.get()) != EOF){
            //存入
            word.push_back(temp);

            //若遇到單引號結尾 且前一字非跳脫字元則開始判斷
            if(temp == '\'' && !escape ){
                //如果超過四個字元 不合法
                if(word.size() > 4){
                    //error
                    token.token = word;
                    token.line = cur_line;
                    token.type = 8;
                    tokens.push_back(token);
                    print(token.type);
                }

                //三個字元
                else if(word.size() == 3){
                    //只有單引號 不合法
                    if(word[1] == '\''){
                        //error
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        tokens.push_back(token);
                        print(token.type);
                    }

                    //剩下的皆合法
                    else{
                        token.token = word;
                        token.line = cur_line;
                        token.type = 4;
                        tokens.push_back(token);
                        print(token.type);
                    }
                }

                //四個字元
                else if(word.size() == 4){
                    //合法跳脫字元 \\ \' \"
                    if(word[1] == '\\' && (word[2]== '\\' || word[2] == '\'' || word[2] == '"') ){
                        token.token = word;
                        token.line = cur_line;
                        token.type = 4;
                        tokens.push_back(token);
                        print(token.type);
                    }
                    else{
                    //其他不合法
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        tokens.push_back(token);
                        print(token.type);
                    }

                }
                return ;
            }

            //消除跳脫字元影響
            if(escape){
                escape = false;
            }
            //偵測跳脫字元
            else if(temp == '\\'){
                escape = true;
            }
        }
        //若進行到此處代表沒有單引號結尾 錯誤
        token.token = word;
        token.line = cur_line;
        token.type = 8;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //註解直接消除 (多行註解尚未修改)
    if(temp == '/'){
        word = line_buf.get();
        temp = line_buf.peek();

        //可跨行註解
        if(temp == '*'){
            //將星號加入
            temp = line_buf.get();
            word.push_back(temp);
            int first_line = cur_line;

            while(true){
                //如果此行讀完還沒有結束 則讀下一行
                if((temp = line_buf.get()) == EOF){
                    //若無檔案可讀了
                    if(!jfile.good()){
                        //印出此段
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        print(token.type);
                        return;
                    }
                    else{
                        //輸出加上一行和空白
                        word += "\n                             :";
                        //下一行
                        cur_line++;
                        //取出一行
                        getline(jfile, line);
                        print_line();
                        //放進buffer
                        line_buf.clear();
                        line_buf.str(line);
                        //再跑一次迴圈
                        continue;
                    }

                }

                //增加字元
                word.push_back(temp);

                //當遇到/時 檢查前一個字元是否為*
                //由於/*/ 是錯誤的註解 所以若為首行則不得少於三個字
                if(temp == '/' && word.size() > 1 && word[word.size()-2] == '*'  && (first_line != cur_line || word.size()>3)){
                    //comment
                    token.token = word;
                    token.line = cur_line;
                    token.type = 9;
                    print(token.type);
                    return;
                }

            }
        }
        //單行註解
        else if(temp == '/'){
            comment = true;
        }
        //不合法內容
        else {
            //退回一字元
            line_buf.unget();
            //取得word
            line_buf >> word;

            //除法
            if(word == "/"){
                token.token = word;
                token.line = cur_line;
                token.type = token_map[word];
                tokens.push_back(token);
                print(token.type);
                return;
            }
            //error
            token.token = word;
            token.line = cur_line;
            token.type = 8;
            tokens.push_back(token);
            print(token.type);
        }
        return;
    }

    //若為{
    if(temp == '{'){
        //scope編號多1
        scope_num++;
        //放到stack
        scope.push(scope_num);
        //word
        word = line_buf.get();
        //special symbol
        token.token = word;
        token.line = cur_line;
        token.type = 2;
        tokens.push_back(token);
        print(token.type);
        return;
    }
    //若為}
    if(temp == '}'){
        //獲取字元
        word = line_buf.get();
        //special symbol
        token.token = word;
        token.line = cur_line;
        token.type = 2;
        tokens.push_back(token);
        print(token.type);
        //結束一層scope
        scope.pop();
        if(scope.empty()){
            //沒有相應的括號 錯誤
            scope.push(-1);
        }
        return;
    }


    //others
    line_buf >> word;
    map<string, int>::iterator it = token_map.find(word);
    //keyword, operator, special symbol
    if(it != token_map.end()){
        token.token = word;
        token.line = cur_line;
        token.type = it->second;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //identifer 開頭為字母, _ , $
    if(isalpha(word[0]) || word[0]=='_' || word[0] == '$'  ){
        for(unsigned int i = 1; i < word.size(); ++i){
            //若是字母,數字, _ , $ ,則為identifer
            if(isalnum(word[i])|| word[0]=='_' || word[0] == '$'){
                continue;
            }
            //若有.則不得為最後一個字元 且下個開頭為字母, _ , $
            if(word[i] == '.' && i != word.size()-1 && (isalpha(word[i+1]) || word[i+1]=='_' || word[i+1] == '$') ){
                continue;
            }
            //若執行到此行代表error
            token.token = word;
            token.line = cur_line;
            token.type = 8;
            tokens.push_back(token);
            print(token.type);
            return;
        }
        //若上述都通過則合法 identifer
        token.token = word;
        token.line = cur_line;
        token.type = 3;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //int, float
    else{
        if(word.size() > 1){
            bool dot = false;
            //第一個字可以為數字, +- , .
            if( isdigit(word[0]) || word[0] == '-' || word[0] == '+' ||  word[0] == '.'){
                //若出現.則設為ture
                if(word[0] == '.')
                    dot = true;
                //逐字測試
                for(unsigned int i = 1; i < word.size(); ++i){
                    //如果有小數點
                    if(dot){
                        if( !isdigit(word[i]) ){
                            token.token = word;
                            token.line = cur_line;
                            token.type = 8;
                            tokens.push_back(token);
                            print(token.type);
                            return;
                        }
                    }
                    //如果無小數點
                    else{
                        if( !isdigit(word[i])){
                            //若小數點則正確
                            if( word[i] == '.'){
                                dot = true;
                                continue;
                            }
                            //若不為數字也不為小數點 錯誤
                            else{
                                token.token = word;
                                token.line = cur_line;
                                token.type = 8;
                                tokens.push_back(token);
                                print(token.type);
                                return;
                            }
                        }
                    }
                }
                //有小數點 float 暫時改寫為number
                if(dot){
                    token.token = word;
                    token.line = cur_line;
                    token.type = 10;
                    tokens.push_back(token);
                    print(token.type);
                }
                //沒小數點 int 暫時改寫為number
                else{
                    token.token = word;
                    token.line = cur_line;
                    token.type = 10;
                    tokens.push_back(token);
                    print(token.type);
                }
                return;
            }
            else{
                token.token = word;
                token.line = cur_line;
                token.type = 8;
                tokens.push_back(token);
                print(token.type);
                return;
            }
        }

        else{
            //若只有一個數字則int 暫時改寫為number
            if(isdigit(word[0])){
                token.token = word;
                token.line = cur_line;
                token.type = 10;
                tokens.push_back(token);
                print(token.type);
                return;
            }
        }
        //若都不是則error
        token.token = word;
        token.line = cur_line;
        token.type = 8;
        tokens.push_back(token);
        print(token.type);
        return;
    }

}



Scanner::Scanner(string name){
    open(name);
    Scanner();
}

Scanner::Scanner(){

    category[0] = "<Keywords>";
    category[1] = "<Operator>";
    category[2] = "<Special>";
    category[3] = "<Identifier>";
    category[4] = "<Char>";
    category[5] = "<String>";
    category[6] = "<Int>";
    category[7] = "<Float>";
    category[8] = "<Error>";
    category[9] = "<Comment>";
	category[10] = "<Number>";

    //keyword
    token_map["auto"] = 0;
    token_map["break"] = 0;
    token_map["case"] = 0;
    token_map["char"] = 0;
    token_map["const"] = 0;
    token_map["continue"] = 0;
    token_map["default"] = 0;
    token_map["do"] = 0;
    token_map["double"] = 0;
    token_map["else"] = 0;
    token_map["enum"] = 0;
    token_map["extern"] = 0;
    token_map["float"] = 0;
    token_map["for"] = 0;
    token_map["goto"] = 0;
    token_map["if"] = 0;
    token_map["int"] = 0;
    token_map["long"] = 0;
    token_map["register"] = 0;
    token_map["return"] = 0;
    token_map["short"] = 0;
    token_map["signed"] = 0;
    token_map["sizeof"] = 0;
    token_map["static"] = 0;
    token_map["struct"] = 0;
    token_map["switch"] = 0;
    token_map["typedef"] = 0;
    token_map["union"] = 0;
    token_map["unsigned"] = 0;
    token_map["void"] = 0;
    token_map["volatile"] = 0;
    token_map["while"] = 0;

    //operator
    token_map["++"] = 1;
    token_map["--"] = 1;
    token_map["+"] = 1;
    token_map["-"] = 1;
    token_map["~"] = 1;
    token_map["!"] = 1;
    token_map["*"] = 1;
    token_map["/"] = 1;
    token_map["%"] = 1;
    token_map["+"] = 1;
    token_map["-"] = 1;
    token_map["<<"] = 1;
    token_map[">>"] = 1;
    token_map[">>>"] = 1;
    token_map["<"] = 1;
    token_map[">"] = 1;
    token_map["<="] = 1;
    token_map[">="] = 1;
    token_map["=="] = 1;
    token_map["!="] = 1;
    token_map["&"] = 1;
    token_map["^"] = 1;
    token_map["|"] = 1;
    token_map["&&"] = 1;
    token_map["||"] = 1;
    token_map["?"] = 1;
    token_map[":"] = 1;
    token_map["="] = 1;
    token_map["+="] = 1;
    token_map["-="] = 1;
    token_map["*="] = 1;
    token_map["/="] = 1;
    token_map["%="] = 1;
    token_map["&="] = 1;
    token_map["^="] = 1;
    token_map["|="] = 1;
    token_map["<<="] = 1;
    token_map[">>="] = 1;

    //special symbol
    token_map["{"] = 2;
    token_map["}"] = 2;
    token_map["["] = 2;
    token_map["]"] = 2;
    token_map["("] = 2;
    token_map[")"] = 2;
    token_map[";"] = 2;
    token_map[","] = 2;

}
