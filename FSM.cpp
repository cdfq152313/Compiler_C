#include "FSM.h"

NFA::NFA(){
    Sinit = new NFAState();
    Sfin = Sinit;
    Sfin->accept = true;
}
void NFA::Clear(){
    Sinit = new NFAState();
    Sfin = Sinit;
    Sfin->accept = true;
}

void NFA::Construct_Char(char c){
    //Final state
    NFAState* FinalState = new NFAState();

    //this connect to final from c
    Sfin->accept = false;
    Sfin->Trans[c].push_back(FinalState);

    //change final state
    FinalState->accept = true;
    this->Sfin = FinalState;

}

void NFA::Construct_String(string s){
    NFAState* FinalState = new NFAState();

    Sfin->accept = false;
    for(unsigned int i = 0; i < s.size(); ++i){
        if(s[i] == '-'){
            for(char c = s[i-1]+1 ; c < s[i+1]; ++c){
                Sfin->Trans[c].push_back(FinalState);
            }
            //s[i+1] is already done
            ++i;
        }else{
            Sfin->Trans[s[i]].push_back(FinalState);
        }
    }
    //change final state
    FinalState->accept = true;
    Sfin = FinalState;
}

void NFA::Connect(NFA* N){
    NFAState* FinalState = new NFAState();

    //this final connect to N inital
    this->Sfin->Epsilon.push_back(N->Sinit);
    this->Sfin->accept = false;
    //N final connect to Final State
    N->Sfin->Epsilon.push_back(FinalState);
    N->Sfin->accept = false;

    //change final state
    FinalState->accept = true;
    this->Sfin = FinalState;
}

void NFA::ConnectStar(NFA* N){
    NFAState* FinalState = new NFAState();

    //this connect to N & FinalState
    this->Sfin->Epsilon.push_back(N->Sinit);
    this->Sfin->Epsilon.push_back(FinalState);
    this->Sfin->accept = false;

    //N connect to this(Epsilon)
    N->Sfin->Epsilon.push_back(this->Sfin);
    N->Sfin->accept = false;

    //change final state
    FinalState->accept = true;
    this->Sfin = FinalState;
}

void NFA::ConnectPlus(NFA* N){
    //final state
    NFAState* x = new NFAState();
    NFAState* FinalState = new NFAState();

    //this connect to N
    this->Sfin->Epsilon.push_back(N->Sinit);
    this->Sfin->accept = false;
    //N connect to x
    N->Sfin->Epsilon.push_back(x);
    N->Sfin->accept = false;
    //x connect to this & final state
    x->Epsilon.push_back( this->Sfin );
    x->Epsilon.push_back( FinalState );
    x->accept = false;

    //change final state
    FinalState->accept = true;
    Sfin = FinalState;
}

void NFA::ConnectOr(NFA* N){
    //init state
    NFAState* initState = new NFAState();

    initState->Epsilon.push_back(this->Sinit);
    initState->Epsilon.push_back(N->Sinit);

    //final state
    NFAState* finalState = new NFAState();
    finalState->accept = true;

    this->Sfin->Epsilon.push_back(finalState);
    this->Sfin->accept = false;
    N->Sfin->Epsilon.push_back(finalState);
    N->Sfin->accept = false;

    this->Sinit = initState;
    this->Sfin = finalState;
}

void NFA::PrintNumber(){
    number = 0;
    print_vector.clear();
    Number(Sinit);

    fp.open("NFA.txt");
    fp << boolalpha;
    Print();
    fp.close();
    fp.clear();
}
void NFA::Number(NFAState* N){
    if(N->state != 0)
        return;
    else{
        N->state = ++number;
        print_vector.push_back(N);
    }

    //Epsilon Traversal
    for(unsigned int i = 0; i < N->Epsilon.size(); ++i){
        Number(N->Epsilon[i]);
    }

    //Trans Traversal
    map<char, vector<NFAState*> >::iterator Trans_it;
    for(Trans_it = N->Trans.begin(); Trans_it!= N->Trans.end(); Trans_it++){
        for(unsigned int i = 0; i < Trans_it->second.size(); ++i){
            Number(Trans_it->second[i]);
        }
    }
}

void NFA::Print(){
    for(unsigned int i = 0; i < print_vector.size(); ++i){
        NFAState* N = print_vector[i];

        fp << N->state << " " << N->accept << endl;
        //print Epsilon
        if( !N->Epsilon.empty() ){
            fp << "Epsilon" << " ";
            for(unsigned int j = 0; j < N->Epsilon.size(); ++j){
                fp << N->Epsilon[j]->state << " ";
            }
            fp << endl;
        }


        //print Trans
        map<char, vector<NFAState*> >::iterator Trans_it;
        for(Trans_it = N->Trans.begin(); Trans_it!= N->Trans.end(); Trans_it++){
            fp << Trans_it->first << " ";
            for(unsigned int j = 0; j < Trans_it->second.size(); ++j){
                fp << Trans_it->second[j]->state << " ";
            }
            fp << endl;
        }
        fp << endl;
    }
}

