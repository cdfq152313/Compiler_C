#ifndef FSM_H
#define FSM_H

using namespace std;

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>


class NFAState{
public:
    int state;
    bool accept;
    map<char,vector<NFAState*> > Trans;
    vector <NFAState *> Epsilon;
    NFAState(){
        state = 0;
        accept = false;
    }
};

class DFAState{
    int state;
    bool accept;
    map<char, DFAState*> Trans;
};

class NFA{
public:
    NFAState* Sinit;//inital state
    NFAState* Sfin;//final State

    NFA();
    void Construct_String(string s);
    void Construct_Char(char c);
    void Clear();
    void Connect(NFA* N);
    void ConnectStar(NFA* N);
    void ConnectPlus(NFA* N);
    void ConnectOr(NFA* N);
    void PrintNumber();


private:
    void Number(NFAState* N);
    void Print();
    int number;
    ofstream fp;
    vector <NFAState*> print_vector;
};

#endif // FSM_H
