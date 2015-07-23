#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;

#include <string>
using std::string;

#include "Parser.h"
#define TEST 1

int main(int argc, char* argv[]){
    string provider = "";
    ifstream *input = nullptr;
    input = new ifstream(string("/Users/fre04/Sandpit/Reformat1MS/index.json"));
#ifndef TEST
    for (int i = 0; i < argc; ++i){
        cout << argv[i] << endl;
        if (string(argv[i]) == "-i"){
            if ( i + 1 < argc){
                input = new ifstream(string(argv[++i]));
                cout << string(argv[i]) <<endl;
            } else{
                std::cerr << "-i option requires one argument." << endl;
            }
        }
    }
#endif
    if (input != nullptr){
        Parser parser;
        (*input) >> parser;
        parser.display();
        
    }
    
#if 0
    if (input != nullptr){
        Parser_1MS parser(provider);
        (*input) >> parser;
        cout << parser;
    }
#endif
    
    return 0;
}