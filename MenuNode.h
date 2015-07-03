//
//  MenuNode.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__MenuNode__
#define __BytePlay__MenuNode__
#include <string>
using std::string;

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

class HeadingNode{
protected:
    string t;
};

class Node{
public:
    string nodeId; //Mandatory
    string nodeType; //Mandatory
    string t; //Mandatory
    Node(string type) : nodeType(type){
        nodeId = "";
        t = "";
    }
    void display(){
        cout << "{nodeId: " << nodeId
        << ", nodeType: " << nodeType
        << ", title: " << t << "}"<< endl;
    }
};

class ProgrammeNode : public Node {
public:
        //nodeId == programmeId
        //t == title
    string renderhints;
    string sy;  //synopsis
    string uuid;
    string seasonuuid;
    string seriesuuid;
    string episodeNumber;
    string seasonNumber;
    string episodeTitle;
    int availEndTime;
    int broadcastTime;
    string provider;
    string cutv;
public:
    ProgrammeNode() : Node("PROGRAMME"){
        renderhints = "";
        sy = "";  //synopsis
        uuid = "";
        seasonuuid = "";
        seriesuuid = "";
        episodeNumber = "";
        seasonNumber = "";
        episodeTitle = "";
        availEndTime = 0;
        broadcastTime = 0;
        provider = "";
        cutv = "";
    }
    void display(){
        cout << "\tProgramme Node: " << endl;
        Node::display();
        cout << "\t{ renderhints: " << renderhints
        << ", sy: " << sy
        << ", uuid: " << uuid
        << ", seasonuuid: " << seasonuuid
        << ", seriesuuid: " << seriesuuid
        << ", episodeNumber: " << episodeNumber
        << ", seasonNumber: " << seasonNumber
        << ", episodeTitle: " << episodeTitle
        << ", availEndTime: " << availEndTime
        << ", broadcastTime: " << broadcastTime
        << ", provider: " << provider
        << ", cutv: " << cutv << "}"<< endl;
    }
    
};

class MenuChildNode : public Node{
protected:
    string renderhints; //Mandatory
    string sy;  //synopsis
    string uuid;
    string seasonuuid;
    string seriesuuid;
    string seasonNumber;
    string provider;
    string cutv;
    vector<ProgrammeNode *> programmeNode;
    
public:
    MenuChildNode(): Node("MENU"){
        renderhints = ""; //Mandatory
        sy = "";  //synopsis
        uuid = "";
        seasonuuid = "";
        seriesuuid = "";
        seasonNumber = "";
        provider = "";
        cutv = "";
    }
    void display(){
        cout << "\tMenuChildNode " << endl;
        Node::display();
        cout << "\t{renderhints: " << renderhints
        << ", sy: " << sy
        << ", uuid: " << uuid
        << ", seasonuuid: " << seasonuuid
        << ", seriesuuid: " << seriesuuid
        << ", seasonNumber: " << seasonNumber
        << ", provider: " << provider
        << ", cutv: " << cutv <<"}" << endl;
        cout << "\t\tProgramme children: " << endl;
        for (const auto & programme: programmeNode){
            programme->display();
        }
        
    }
};

class MenuNode : public Node{
public:
    string branduri;
    vector<MenuChildNode *> * childrenNodes;
    vector<ProgrammeNode *> * programmeNode;
public:
    MenuNode() : Node("MENU"){
        branduri = "";
        childrenNodes = nullptr;
        programmeNode = nullptr;
    }
    
    ~MenuNode(){
        if (programmeNode!=nullptr) delete programmeNode;
        if (childrenNodes!=nullptr) delete childrenNodes;
    }
    
    void setData(ProgrammeNode *data){
        if (data != nullptr){
            t = data->t;
            nodeId = data->nodeId;
        }
    }
    
    void addProgramme(ProgrammeNode * item){
        if (programmeNode == nullptr){
            programmeNode = new vector<ProgrammeNode *>;
        }
        programmeNode->push_back(item);
    }
    void setBrandUri(const string str){
        branduri = str;
    }
    
    void display(){
        //metadata
        cout << "MenuNode: " << endl;
        Node::display();
        cout << "brandUri: " << branduri << endl;
#if 0
        //child nodes
        if (childrenNodes!=nullptr){
            for (const auto & child : *childrenNodes){
                child->display();
            }
        }
        //programme nodes
        if (programmeNode!=nullptr){
            for (const auto & programme : *programmeNode){
                programme->display();
            }
        }
#endif
    }
};






#endif /* defined(__BytePlay__MenuNode__) */
