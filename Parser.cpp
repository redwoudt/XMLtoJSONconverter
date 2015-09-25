//
//  Parser.cpp
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 23/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "Parser.h"
#include "json.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

#ifndef TEST
#define CATALOGUE_DIR "./catalognode/"
#define CONTENT_DETAILS_DIR "./contentdetails/"
#define BASE_DATA_DIR "./data/"
#else
#define BASE_DATA_DIR "/Users/fre04/Sandpit/Reformat1MS/"
#define CATALOGUE_DIR "/Users/fre04/Sandpit/Reformat1MS/catalognode/"
#define CONTENT_DETAILS_DIR "/Users/fre04/Sandpit/Reformat1MS/contentdetails/"
#endif


ifstream & operator>>(ifstream &input, Parser &parser){
    Json::Value data;
    Json::Reader reader;
        
    bool parsingSuccessful = reader.parse( input, data );
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse configuration\n"
        << reader.getFormattedErrorMessages();
        return input;
    }
    std::string platform = data.get("platform", "undefined" ).asString();
   // cout << platform << endl;
    
    if (platform != "undefined" && parser.root == nullptr){ //root menu
        parser.root = new DataTree(platform);
        ifstream *inputFile = nullptr;
        
        const Json::Value providers = data["contentProviders"];
        for ( int index = 0; index < providers.size(); ++index )  {
            string value(providers[index]["provider"].asString());
            DataNode * node = parser.root->AddNode(value);
            
            //cout << providers[index]["dataLink"].asString() << endl;
            inputFile = new ifstream(providers[index]["dataLink"].asString());
            
            (*inputFile) >> (*node);
            //parser.root->addMenuNode(child);
            delete inputFile;
        }
        parser.root->sortLeaves();
        parser.root->addId();
    }
    
    return input;
}

ostream & operator<<(ostream &output, Parser &parser){
    
    //traverse through data tree: convert children to childmenus and items to programmenodes
    //TODO: work through email notes (David N and from Adam meeting)
    output << "Starting with output..." << endl;
    
    //parser.root
    {
        DIR *channelDir = opendir(CATALOGUE_DIR);
    
        if (channelDir){
            struct dirent * next_file;
            char filepath[256];
            while((next_file = readdir(channelDir))!=nullptr){
                // build the full path for each file in the folder
                sprintf(filepath, "%s/%s", CATALOGUE_DIR, next_file->d_name);
                remove(filepath);
            }
        }
        //create channel files:
        mkdir(CATALOGUE_DIR, 0777);
        
        //first create bookmark nodes:
        cout << "."<<std::flush;
        //push out channel metadata:
        string path (BASE_DATA_DIR);
        {
            ofstream channelFile (path + "catalognode/" + HPCATCHUP + ".json");
            if (channelFile.is_open()){
                parser.createBookmark(channelFile, HPCATCHUP);
            }
            channelFile.close();
        }
        {
            ofstream channelFile (path + "catalognode/" + NICHEVIDEO + ".json");
            if (channelFile.is_open()){
                parser.createBookmark(channelFile, NICHEVIDEO);
            }
            channelFile.close();
        }
        //next create menu nodes
        //use something like: parser.root->createNode(directory)
        // this will then all each data tree and item in tree and create menu nodes - might as well do contentdetails at the same time
        parser.root->createNode(path);
                               
    }
    output << "...Complete!" << endl;
    return output;
}

void Parser::createBookmark(ofstream & file, const string & bookmark){
    if (bookmark.compare(HPCATCHUP)==0){
        Json::Value root;
        root["nodeid"] = "BOOKMARK";
        root["nodetype"] = "MENU";
        root["t"] = HPCATCHUP;
        Json::Value child;
        child.clear();
        child["nodetype"] = "MENU";
        child["nodeid"] = HPCATCHUP_NODEID;
        child["t"] = "Catch Up";
        child["renderhints"] = "{ \"template\": \"MENUPANEL\" }";
        child["sy"] = "Catch up on favourite shows from the last 7 days.";
        root["childnodes"].append(child);
        file << root;
        //cout << root;
    }
    else if (bookmark.compare(NICHEVIDEO)==0){
        
        Json::Value root;
        root["nodeid"] = "BOOKMARK";
        root["nodetype"] = "MENU";
        root["t"] = NICHEVIDEO;
        Json::Value child;
        child.clear();
        child["nodetype"] = "MENU";
        child["nodeid"] = NICHEVIDEO_NODEID;
        child["t"] = "OTT Niche Video";
        child["renderhints"] = "{ \"template\": \"MENUPANEL\" }";
        child["sy"] = "Find all the best Niche Video here.";
        root["childnodes"].append(child);
        file << root;
       // cout << root;
    }
    
}
