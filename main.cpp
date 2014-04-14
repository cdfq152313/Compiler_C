#include <iostream>
#include <string>
#include "RE.h"
using namespace std;

void test(string &x){
    cout<< x << endl;
    x = "fucking yaaa";
}

int main()
{
    RE re;
    re.pattern("apple|banana");
    return 0;
}


