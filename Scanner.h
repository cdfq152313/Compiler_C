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
        Scanner(string name);//��l���ܼƨè̷ӫ��w�ɦW�}��
        Scanner();//��l���ܼ�(map)
        bool open(string name);//�}�ҫ��w�ɮ�
        void close();//����
        bool start(string name);//�}�l����
        bool PrinttoParser(string name);
    private:
        ifstream jfile;//��J�ɮ�
        ofstream output;//��X�ɮ�
        ofstream output2;
        string line;//�@��r��
        int cur_line;//�{�b���
        istringstream line_buf;//�r��y
        string word;//�x�s�@��word���ѧO
        int scope_num;//scope�s��
        stack<int> scope;//���F�p��scope��stack
        bool escape;//�p�����r�����v�T
        bool comment;
        bool success;
        string category[11];//����
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
        map<string, int> token_map;//����keyword, operator, special symbol

        void analyzer();//��r��yline_buf�����e�� �Φ��̧Ƕi��token lexer
        void print(int type);//��analyzer()�ե� �L�Xword�����e �õ���token�����O
        void print_line();//��start()�Manalyzer()�ե� �L�X�@�檺���e
};



#endif // SCANNER_H_INCLUDED
