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

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;


class HeadingNode{
protected:
    string t;
};

struct renderhints{
    string s_template;
    string s_imagetype;
};


class Node{
public:
    string nodeId; //Mandatory
    string nodeType; //Mandatory
    string t; //Mandatory
    renderhints hints; //Mandatory
    Node(const string & type, const string & title = "", const string & nId = "") : nodeType(type), nodeId(nId), t(title){}

    void display(){
        cout << "{nodeId: " << nodeId
        << ", nodeType: " << nodeType
        << ", title: " << t << "}"
        << ", render hints: " << "template: " << hints.s_template << ", imagetype: " << hints.s_imagetype << "}"
        << endl;
    }
};

class ProgrammeNode : public Node {
public:
        //nodeId == programmeId
        //t == title
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
    string thumbnail;
    string cutv;
public:
    ProgrammeNode() : Node("PROGRAMME"){
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
        thumbnail = "";
        hints.s_template = "";
        hints.s_imagetype = "";
    }
    void display(){
        cout << "\tProgramme Node: " << endl;
        Node::display();
        cout << ", sy: " << sy
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
public:
    string sy;  //synopsis
    string uuid;
    string seasonuuid;
    string seriesuuid;
    string seasonNumber;
    string provider;
    string cutv;
    string thumbnail;
    vector<ProgrammeNode *> programmeNode;
    
public:
    MenuChildNode(string p = "unknown"): Node("MENU"), provider(p){
        hints.s_template = ""; //Mandatory
        hints.s_imagetype = ""; //Mandatory
        sy = "";  //synopsis
        uuid = "";
        seasonuuid = "";
        seriesuuid = "";
        seasonNumber = "";
        cutv = "";
        thumbnail = "";
        hints.s_template = "3COL";
        hints.s_imagetype = "16-9";
    }
    
    void setNodeId(const string & str){
        nodeId = str;
    }
    
    void setTitle(const string & str){
        t = str;
    }
    
    void setData(MenuChildNode *data){
        if (data != nullptr){
            t = data->t;
            nodeId = data->nodeId;
            //renderhints = data->renderhints; //Mandatory
            sy = data->sy;  //synopsis
            uuid = data->uuid;
            seasonuuid = data->seasonuuid;
            seriesuuid = data->seriesuuid;
            seasonNumber = data->seasonNumber;
            provider = data->provider;
            cutv = data->cutv;
            thumbnail = data->thumbnail;
        }
    }
    
    void display(){
        cout << "\tMenuChildNode " << endl;
        Node::display();
        cout << ", sy: " << sy
        << ", uuid: " << uuid
        << ", seasonuuid: " << seasonuuid
        << ", seriesuuid: " << seriesuuid
        << ", seasonNumber: " << seasonNumber
        << ", provider: " << provider
        << ", cutv: " << cutv <<"}" << endl;
#if 0
        cout << "\t\tProgramme children: " << endl;
        for (const auto & programme: programmeNode){
            programme->display();
        }
#endif
    }
};

class MenuNode : public Node{
public:
    string branduri;
    vector<MenuNode *> * menuNodes;
    vector<MenuChildNode *> * childrenNodes;
    vector<ProgrammeNode *> * programmeNode;
    
public:
    friend ifstream & operator>>(ifstream &input, MenuNode &node);
    friend ostream & operator<<(ostream &output, MenuNode &node);
    MenuNode(const string & title = "", const string & nId = "") : Node("MENU", title, nId){
        branduri = "";
        menuNodes = nullptr;
        childrenNodes = nullptr;
        programmeNode = nullptr;
        hints.s_template = "MENUPANEL";
        hints.s_imagetype = "";
    }
    
    ~MenuNode(){
        if (programmeNode!=nullptr) delete programmeNode;
        if (childrenNodes!=nullptr) delete childrenNodes;
        if (menuNodes!=nullptr) delete menuNodes;
    }
    
    void setData(ProgrammeNode *data){
        if (data != nullptr){
            t = data->t;
            nodeId = data->nodeId;
        }
    }
    void addMenuNode(MenuNode * item){
        if (menuNodes == nullptr){
            menuNodes = new vector<MenuNode *>;
        }
        menuNodes->push_back(item);
    }
    void addChildNode(MenuChildNode * item){
        if (childrenNodes == nullptr){
            childrenNodes = new vector<MenuChildNode *>;
        }
        childrenNodes->push_back(item);
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
        //menu nodes
        if (menuNodes!=nullptr){
            for (const auto & menu : *menuNodes){
                menu->display();
            }
        }
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
    }
};






#endif /* defined(__BytePlay__MenuNode__) */
