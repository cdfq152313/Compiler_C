#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#include "Struct.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <stack>
#include <map>
#include <vector>
#include <cctype>
using namespace std;



class Scanner{
    public:
        Scanner(string name);//初始化變數並依照指定檔名開檔
        Scanner();//初始化變數(map)
        bool open(string name);//開啟指定檔案
        void close();//關檔
        bool start(string name);//開始執行
        bool PrinttoParser(string name);
    private:
        ifstream jfile;//輸入檔案
        ofstream output;//輸出檔案
        ofstream output2;
        string line;//一行字串
        int cur_line;//現在行數
        istringstream line_buf;//字串流
        string word;//儲存一個word來識別
        int scope_num;//scope編號
        stack<int> scope;//為了計算scope的stack
        bool escape;//計算跳脫字元的影響
        bool comment;
        bool success;
        string category[11];//分類
        /* category
        0  <Keywords>
        1  <Operator>
        2  <Special Symbol>
        3  <Identifier>
        4  <Char>
        5  <String>
        6  <Int>
        7  <Float>
        8  <Error>
        9  <Comment>
		10 <Number>
        */
        vector <Token> tokens;
        map<string, int> token_map;//紀錄keyword, operator, special symbol

        void analyzer();//當字串流line_buf有內容時 用此依序進行token lexer
        void print(int type);//由analyzer()調用 印出word的內容 並給予token的類別
        void print_line();//由start()和analyzer()調用 印出一行的內容
};



#endif // SCANNER_H_INCLUDED
