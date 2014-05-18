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
    //�}�ҿ�X��
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
        //���X�@��
        getline(jfile, line);
        //�L�X�@��
        output2 << "Line" << setw(3) << cur_line << " :" << endl;

        //��istring buffer��
        line_buf.clear();
        line_buf.str(line);
        //������e�ť�
        line_buf >> ws;

        while(line_buf.good()){
            //���R�@�Ӧr
            analyzer();

            if(comment){
                //������
                comment = false;
                break;
            }

            //���������ť�
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
    char temp;//�x�s�@�Ӧr��
    Token token;

    //���լO�_��char �M string �M����
    temp = line_buf.peek();

    //string
    if(temp == '"'){
        //�N�Ĥ@�����޸��s�J
        word = line_buf.get();
        //�L���������r��
        escape = false;

        //���ɮש|������
        while((temp = line_buf.get()) != EOF){
            //�s�J
            word.push_back(temp);
            //�Y�J�����޸����� �B�e�@�r�D����r���h����
            if(temp == '"' && !escape ){
                token.token = word;
                token.line = cur_line;
                token.type = 5;
                tokens.push_back(token);
                print(token.type);
                return ;
            }

            //��������r���v�T
            if(escape){
                escape = false;
            }
            //��������r��
            else if(temp == '\\'){
                escape = true;
            }
        }
        //�Y�i��즹�B�N��S�����޸����� ���~
        token.token = word;
        token.line = cur_line;
        token.type = 8;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //char
    if(temp == '\''){
        //�N�Ĥ@�ӳ�޸��s�J
        word = line_buf.get();
        //�L���������r��
        escape = false;

        //���ɮש|������
        while((temp = line_buf.get()) != EOF){
            //�s�J
            word.push_back(temp);

            //�Y�J���޸����� �B�e�@�r�D����r���h�}�l�P�_
            if(temp == '\'' && !escape ){
                //�p�G�W�L�|�Ӧr�� ���X�k
                if(word.size() > 4){
                    //error
                    token.token = word;
                    token.line = cur_line;
                    token.type = 8;
                    tokens.push_back(token);
                    print(token.type);
                }

                //�T�Ӧr��
                else if(word.size() == 3){
                    //�u����޸� ���X�k
                    if(word[1] == '\''){
                        //error
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        tokens.push_back(token);
                        print(token.type);
                    }

                    //�ѤU���ҦX�k
                    else{
                        token.token = word;
                        token.line = cur_line;
                        token.type = 4;
                        tokens.push_back(token);
                        print(token.type);
                    }
                }

                //�|�Ӧr��
                else if(word.size() == 4){
                    //�X�k����r�� \\ \' \"
                    if(word[1] == '\\' && (word[2]== '\\' || word[2] == '\'' || word[2] == '"') ){
                        token.token = word;
                        token.line = cur_line;
                        token.type = 4;
                        tokens.push_back(token);
                        print(token.type);
                    }
                    else{
                    //��L���X�k
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        tokens.push_back(token);
                        print(token.type);
                    }

                }
                return ;
            }

            //��������r���v�T
            if(escape){
                escape = false;
            }
            //��������r��
            else if(temp == '\\'){
                escape = true;
            }
        }
        //�Y�i��즹�B�N��S����޸����� ���~
        token.token = word;
        token.line = cur_line;
        token.type = 8;
        tokens.push_back(token);
        print(token.type);
        return;
    }

    //���Ѫ������� (�h����ѩ|���ק�)
    if(temp == '/'){
        word = line_buf.get();
        temp = line_buf.peek();

        //�i������
        if(temp == '*'){
            //�N�P���[�J
            temp = line_buf.get();
            word.push_back(temp);
            int first_line = cur_line;

            while(true){
                //�p�G����Ū���٨S������ �hŪ�U�@��
                if((temp = line_buf.get()) == EOF){
                    //�Y�L�ɮץiŪ�F
                    if(!jfile.good()){
                        //�L�X���q
                        token.token = word;
                        token.line = cur_line;
                        token.type = 8;
                        print(token.type);
                        return;
                    }
                    else{
                        //��X�[�W�@��M�ť�
                        word += "\n                             :";
                        //�U�@��
                        cur_line++;
                        //���X�@��
                        getline(jfile, line);
                        print_line();
                        //��ibuffer
                        line_buf.clear();
                        line_buf.str(line);
                        //�A�]�@���j��
                        continue;
                    }

                }

                //�W�[�r��
                word.push_back(temp);

                //��J��/�� �ˬd�e�@�Ӧr���O�_��*
                //�ѩ�/*/ �O���~������ �ҥH�Y������h���o�֩�T�Ӧr
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
        //������
        else if(temp == '/'){
            comment = true;
        }
        //���X�k���e
        else {
            //�h�^�@�r��
            line_buf.unget();
            //���oword
            line_buf >> word;

            //���k
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

    //�Y��{
    if(temp == '{'){
        //scope�s���h1
        scope_num++;
        //���stack
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
    //�Y��}
    if(temp == '}'){
        //����r��
        word = line_buf.get();
        //special symbol
        token.token = word;
        token.line = cur_line;
        token.type = 2;
        tokens.push_back(token);
        print(token.type);
        //�����@�hscope
        scope.pop();
        if(scope.empty()){
            //�S���������A�� ���~
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

    //identifer �}�Y���r��, _ , $
    if(isalpha(word[0]) || word[0]=='_' || word[0] == '$'  ){
        for(unsigned int i = 1; i < word.size(); ++i){
            //�Y�O�r��,�Ʀr, _ , $ ,�h��identifer
            if(isalnum(word[i])|| word[0]=='_' || word[0] == '$'){
                continue;
            }
            //�Y��.�h���o���̫�@�Ӧr�� �B�U�Ӷ}�Y���r��, _ , $
            if(word[i] == '.' && i != word.size()-1 && (isalpha(word[i+1]) || word[i+1]=='_' || word[i+1] == '$') ){
                continue;
            }
            //�Y����즹��N��error
            token.token = word;
            token.line = cur_line;
            token.type = 8;
            tokens.push_back(token);
            print(token.type);
            return;
        }
        //�Y�W�z���q�L�h�X�k identifer
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
            //�Ĥ@�Ӧr�i�H���Ʀr, +- , .
            if( isdigit(word[0]) || word[0] == '-' || word[0] == '+' ||  word[0] == '.'){
                //�Y�X�{.�h�]��ture
                if(word[0] == '.')
                    dot = true;
                //�v�r����
                for(unsigned int i = 1; i < word.size(); ++i){
                    //�p�G���p���I
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
                    //�p�G�L�p���I
                    else{
                        if( !isdigit(word[i])){
                            //�Y�p���I�h���T
                            if( word[i] == '.'){
                                dot = true;
                                continue;
                            }
                            //�Y�����Ʀr�]�����p���I ���~
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
                //���p���I float �Ȯɧ�g��number
                if(dot){
                    token.token = word;
                    token.line = cur_line;
                    token.type = 10;
                    tokens.push_back(token);
                    print(token.type);
                }
                //�S�p���I int �Ȯɧ�g��number
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
            //�Y�u���@�ӼƦr�hint �Ȯɧ�g��number
            if(isdigit(word[0])){
                token.token = word;
                token.line = cur_line;
                token.type = 10;
                tokens.push_back(token);
                print(token.type);
                return;
            }
        }
        //�Y�����O�herror
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
