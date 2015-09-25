//
//  MenuNode.cpp
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "MenuNode.h"
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <cstring>
#include <string.h>

//TODO - clean this up
ifstream & operator>>(ifstream &input, MenuNode &node){
    return input;
}
ostream & operator<<(ostream &output, MenuNode &node){
    return output;
}


