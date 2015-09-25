//
//  Parser.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 23/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Parser__
#define __BytePlay__Parser__

#define TEST 1

#include <iostream>
using std::cout;
using std::endl;

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;

#include "DataTree.h"
#include "json.h"

const string HPCATCHUP = "HPCATCHUP";
const string HPCATCHUP_NODEID = "BOOKMARK001";
const string NICHEVIDEO = "NICHEVIDEO";
const string NICHEVIDEO_NODEID = "BOOKMARK002";

class Parser{
private:
    DataTree * root;
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
    void createBookmark(ofstream & file, const string & bookmark);
    
    void display(){
        if (root!=nullptr){
            root->display();
        }
    }
    
    
};

#endif /* defined(__BytePlay__Parser__) */
