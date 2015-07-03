#include <iostream>
using std::cout;
using std::endl;
#include <fstream>
using std::ifstream;
using std::getline;

#include <string>
using std::string;

#include "Parser_1MS.h"

int main(){
    //TODO add input filename to input of program, and destination directory /mainstream/ will create /mainstream/nodes /mainstream/programmes - listing the nodes and programmes as seperate files with the unique id as the file name
    string provider = "TasteMade";
    Parser_1MS parser(provider);
    ifstream input("/Users/fre04/Documents/NicheVideo/1mainstream/tm.xml");
    input >> parser;
    return 0;
}