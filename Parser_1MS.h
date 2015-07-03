//
//  Parser_1MS.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Parser_1MS__
#define __BytePlay__Parser_1MS__
#include <fstream>
using std::ifstream;
using std::ofstream;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "Item1MS.h"

class Parser_1MS{
    friend ifstream & operator>>(ifstream &input, Parser_1MS &parser);
public:
    string provider;
public:
    Parser_1MS(string p = "unknown") : provider(p){}
    void display(void){
        cout << "Here we go!" << endl;
    }
    bool isCategory(const string & str){
        if (str.empty()) return false;
        for (const auto & s : str){
            switch (s){
                case '>':
                case '<':
                case '=':
                    return false;
                default:
                    break;
            }
        }
        
        return true;
    }
    string getKeyValue(string & current, string key){
        string result = "";
        key += "=";
        
        size_t begin = current.find(key);
        if (begin != string::npos){
            begin = current.find("\"", begin);
            begin+=1;
            size_t end = current.find("\"", begin);
            if (end > begin){
                result = current.substr(begin, end-begin);
            }
        }
        return result;
    }
    
};

#endif /* defined(__BytePlay__Parser_1MS__) */
