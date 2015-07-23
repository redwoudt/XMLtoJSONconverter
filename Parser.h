//
//  Parser.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 23/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Parser__
#define __BytePlay__Parser__

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;
#include "Item1MS.h"
#include "MenuNode.h"

class Parser{
private:
    MenuNode * root;
public:
    friend ifstream & operator>>(ifstream &input, Parser &parser);
    friend ostream & operator<<(ostream &output, Parser &parser);
    Parser():root(nullptr){
    }
    ~Parser(){
        if (root!=nullptr){
            delete root;
        }
    }
    void display(){
        if (root!=nullptr){
            root->display();
        }
    }
    
    
};

#endif /* defined(__BytePlay__Parser__) */
