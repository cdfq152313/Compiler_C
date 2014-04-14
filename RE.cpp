#include "RE.h"
//記得在Re最後加上空白

void RE::pattern(string Re){
    nfa = parse(Re, 0);
    nfa->PrintNumber();
}

NFA* RE::parse(string &Re, int i){
    NFA* N1 = new NFA();
    for(unsigned int j = i; j < Re.size()-1; ++j){
        NFA* N2 = NULL;
        if(Re[j] == '('){
            N2 = parse(Re,j+1);
            j = JumpToRightScope(Re, j);
        }

        if(Re[j] == ')'){
            break;
        }

        if(Re[j] == '['){
            //construct a substring []
            int subend = JumpToRightScope(Re, j);
            string s = Re.substr(j+1, subend-1);
            N2 = new NFA();
            N2->Construct_String(s);
            j = subend;
        }

        //connect or
        if(Re[j] == '|'){
            N2 = parse(Re, j+1);
            N1->ConnectOr(N2);
            delete N2;
            break;
        }
        //connect star
        if(Re[j+1] == '*'){
            if(N2 == NULL){
                N2 = new NFA();
                N2->Construct_Char(Re[j]);
                N1->ConnectStar(N2);
                delete N2;
            }
            else{
                N1->ConnectStar(N2);
                delete N2;
            }
            //next char "*" already done
            j++;
        }
        //connect plus
        else if(Re[j+1] == '+'){
            if(N2 == NULL){
                N2 = new NFA();
                N2->Construct_Char(Re[j]);
                N1->ConnectPlus(N2);
                delete N2;
            }
            else{
                N1->ConnectPlus(N2);
                delete N2;
            }
            //next char "+" already done
            j++;
        }
        //connect
        else{
            if(N2 == NULL){
                N1->Construct_Char(Re[j]);
            }
            else{
                N1->Connect(N2);
                delete N2;
            }
        }
    }
    return N1;
}

int RE::JumpToRightScope(string &Re, int i){
    stack<int> scope;
    for(unsigned int j = i; j < Re.size(); ++j){
        if(Re[j] == '('){
            scope.push(j);
        }
        if(Re[j] == ')'){
            scope.pop();
        }
        if(Re[j] == '['){
            scope.push(j);
        }
        if(Re[j] == ']'){
            scope.pop();
        }
        if(scope.empty()){
            return j;
        }
    }
    //error
    return 0;
}
