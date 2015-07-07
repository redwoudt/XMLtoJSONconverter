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
using std::ostream;
#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "Item1MS.h"
#include "MenuNode.h"

static const string SERIES = "series";
static const string EPISODE = "episode";
static const int lineSpace = 3;


class Parser_1MS{
    friend ifstream & operator>>(ifstream &input, Parser_1MS &parser);
    friend ostream & operator<<(ostream &output, Parser_1MS &parser);
public:
    string provider;
    vector<MenuNode *> channels;
    vector<Content *> contentItems;
public:
    Parser_1MS(string p = "unknown") : provider(p){}
    void display(void){
       // cout << "Parsing file..." << endl;
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
    bool isChannelHeader(Item1MS * item){
        if (item == nullptr) return false;
        return (item->sourceId == "" && item->label=="" && item->playbackUrl=="" && item->categories->empty());
        
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
    
    void insertLineNoQuote(ofstream & file, string type, bool value, int numberOfSpace = 0, bool last=false){
        if (file.is_open()){
            while (numberOfSpace--){
                file << " ";
            }
            if (last){
                file << type << ": " << boolalpha << value << endl;
            }
            else {
                file << type << ": " << boolalpha << value << "," << endl;
            }
            
        }
    }
    
    void insertLineNoQuote(ofstream & file, string type, int value, int numberOfSpace = 0, bool last=false){
        if (file.is_open()){
            while (numberOfSpace--){
                file << " ";
            }
            if (last){
                file << type << ": " << value << endl;
            }
            else {
                file << type << ": " << value << "," << endl;
            }
            
        }
    }
    
    void insertLineNoQuote(ofstream & file, string type, string value, int numberOfSpace = 0, bool last=false){
        if (file.is_open()){
            while (numberOfSpace--){
                file << " ";
            }
            if (last){
                file << type << ": \"" << value << "\"" << endl;
            }
            else {
                file << type << ": \"" << value << "\"," << endl;
            }
        }
    }
    
    void insertLine(ofstream & file, string type, string value, int numberOfSpace = 0, bool last=false){
        if (file.is_open()){
            while (numberOfSpace--){
                file << " ";
            }
            if (last){
                file << "\"" << type << "\": \"" << value << "\"" << endl;
            }
            else {
                file << "\"" << type << "\": \"" << value << "\"," << endl;
            }
            
        }
    }
    
    void addContentMetadata(ofstream & file, Content * item){
        if (item == nullptr) return;
        if (file.is_open()){
            file << "{" << endl;
            
            string pad(lineSpace, ' ');
            file << pad + "\"childnodes\": [" <<endl;
            if (item->formats){
                for (const auto & format : *item->formats){
                    file << pad + pad + "{" << endl;
                    insertLineNoQuote(file, "at", format->at, lineSpace*3);
                    insertLineNoQuote(file, "playbackLink", format->playbackLink, lineSpace*3);
                    insertLineNoQuote(file, "oigid", format->oigid, lineSpace*3);
                    insertLineNoQuote(file, "d", format->d, lineSpace*3);
                    insertLineNoQuote(file, "hd", format->hd, lineSpace*3);
                    insertLineNoQuote(file, "provider", format->provider, lineSpace*3, true);
                    if (format == item->formats->back()){
                        file << pad + pad + "}" << endl;
                    }
                    else {
                        file << pad + pad + "}," << endl;
                    }
                    
                }
            }
            file << pad + "],"<< endl;
            file << endl;
            
            insertLine(file, "uuid", item->uuid, lineSpace);
            insertLine(file, "seasonuuid", item->seasonUuid, lineSpace);
            insertLine(file, "seriesuuid", item->seriesUuid, lineSpace);
            insertLine(file, "sy", item->sy, lineSpace);
            insertLine(file, "t", item->t, lineSpace, true);
            file << "}" << endl;
        }
    }
    
    void addChildMetadata(ofstream & file, MenuChildNode * node){
        if (node == nullptr) return;
        if (file.is_open()){
            file << "{" << endl;
            insertLine(file, "nodeid", node->nodeId, lineSpace);
            insertLine(file, "nodetype", node->nodeType, lineSpace);
            insertLine(file, "t", node->t, lineSpace);
            //insertLine(file, "branduri", node->branduri);
            file << endl;
            string pad(lineSpace, ' ');
            file << pad + "\"childnodes\": [" <<endl;
            if (!node->programmeNode.empty()){
                for (const auto & programme : node->programmeNode){
                    file << pad + "{" << endl;
                    insertLine(file, "nodetype", programme->nodeType, lineSpace*2);
                    insertLine(file, "nodeid", programme->nodeId, lineSpace*2);
                    insertLine(file, "t", programme->t, lineSpace*2);
                    insertLine(file, "renderhints", programme->renderhints, lineSpace*2);
                    insertLine(file, "sy", programme->sy, lineSpace*2, true);
                    //insertLine(file, "uuid", programme->uuid, lineSpace*2);
                    if (programme == node->programmeNode.back()){
                        file << pad + "}" << endl;
                    }
                    else {
                        file << pad + "}," << endl;
                    }
                    
                }
            }
            file << pad + "]"<< endl;
            file << "}" << endl;
        }
    }
    
    void addChannelMetadata(ofstream & file, MenuNode * channel){
        if (channel == nullptr) return;
        if (file.is_open()){
            file << "{" << endl;
            insertLine(file, "nodeid", channel->nodeId, lineSpace);
            insertLine(file, "nodetype", channel->nodeType, lineSpace);
            insertLine(file, "t", channel->t, lineSpace);
            //insertLine(file, "branduri", channel->branduri);
            file << endl;
            string pad(lineSpace, ' ');
            file << pad + "\"childnodes\": [" <<endl;
            if (channel->childrenNodes){
                for (const auto & node : *channel->childrenNodes){
                    file << pad + "{" << endl;
                    insertLine(file, "nodetype", node->nodeType, lineSpace*2);
                    insertLine(file, "nodeid", node->nodeId, lineSpace*2);
                    insertLine(file, "t", node->t, lineSpace*2);
                    insertLine(file, "renderhints", node->renderhints, lineSpace*2);
                    insertLine(file, "sy", node->sy, lineSpace*2, true);
                    //insertLine(file, "uuid", node->uuid, lineSpace*2);
                    if (node == channel->childrenNodes->back()){
                        file << pad + "}" << endl;
                    }
                    else {
                        file << pad + "}," << endl;
                    }
                    
                }
            }
            file << pad + "]"<< endl;
            file << "}" << endl;
        }
    }
    
    
};

#endif /* defined(__BytePlay__Parser_1MS__) */
