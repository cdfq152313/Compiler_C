#ifndef RE_H
#define RE_H

using namespace std;
#include "FSM.h"
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <stack>

class RE{
private:
    NFA *nfa;
    NFA* parse(string &Re, int i);
    int JumpToRightScope(string &Re, int i);//return the right scope position

public:
    void pattern(string Re);
};

#endif // RE_H
