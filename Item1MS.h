//
//  Item1MS.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Item1MS__
#define __BytePlay__Item1MS__
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
#include <unordered_map>
using std::unordered_map;
#include <set>
using std::set;

#include "MenuNode.h"
#include "Content.h"

class Item1MS{
public:
    string uniqueId;
    string pubDate;
    string title;
    string description;
    int duration;
    string playbackUrl;
    string thumbnailUrl;
    string rating;
    string category;
    string secondaryTitle;
    string priceType;
    string sourceId;
    double episodeNumber;
    string label;
    set<string> * categories;
    string provider;
public:
    Item1MS(string p = "unknown", string id = ""): provider(p), uniqueId(id){
        pubDate = "";
        title = "";
        description = "";
        duration = 0;
        playbackUrl = "";
        thumbnailUrl = "";
        rating = "";
        category = "";
        secondaryTitle = "";
        priceType = "";
        sourceId = "";
        episodeNumber = 0.0;
        label = "";
        categories = new set<string>;
    }
    ~Item1MS(){
        delete categories;
    }
    
    void display() const{
        cout << " For Item(" << uniqueId <<") " << endl
         << "pubDate: " << pubDate << endl
        << "title: " << title << endl
        << "description: " << description << endl
        << "duration: " << duration << endl
        << "playbackUrl: " << playbackUrl << endl
        << "thumbnailUrl: " << thumbnailUrl  << endl
        << "rating: " << rating << endl
        << "category: " << category << endl
        << "secondaryTitle: " << secondaryTitle << endl
        << "priceType: " << priceType << endl
        << "sourceId: " << sourceId << endl
        << "provider: " << provider<< endl
        << "episodeNumber: " << episodeNumber << endl
        << "label: " << label << endl;
        if (categories != nullptr){
            cout << "categories: {";
            for (const auto & category: *categories){
                cout << category << "; ";
            }
            cout << "}" << endl;
        }
        
    }
    
    void addCategory(const string & str){
        if (categories != nullptr){
            categories->insert(str);
        }
    }
    
    void setTitle(const string & str){
        title = str;
    }
    void setPubDate(const string & str){
        pubDate = str;
    }
    void setDescription(const string &str){
        description = str;
    }
    
    void setUniqueId(const string &str){
        uniqueId = str;
    }
    
    void setPlaybackUrl(const string &str){
        playbackUrl = str;
    }
    
    void setSourceId(const string &str){
        sourceId = str;
    }

    void setDuration(int value){
        duration = value;
    }
    
    void setEpisode(double value){
        episodeNumber = value;
    }
    
    void setThumbnailUrl(const string &str){
        thumbnailUrl = str;
    }
    
    void setProvider(const string &str){
        provider = str;
    }
    
    void setLabel(const string &str){
        label = str;
    }
    
    Content * createContent (){
        Format * contentF = new Format();
        contentF->at = "M";
        //contentF.availEndTime;
        //contentF.broadcastTime;
        contentF->downloadLink = "";
        contentF->playbackLink = playbackUrl;
        contentF->thumbnailLink = thumbnailUrl;
        if (uniqueId.size() <= sizeof("4290562359001")){
            contentF->oigid = atoi(uniqueId.c_str());
        }
        else {
            contentF->oigid = atoi(uniqueId.substr(sizeof("4290562359001")).c_str());
        }
        
        contentF->d = duration;
        contentF->provider = provider;
        contentF->sz = 0; //*new* leave this, size of program in KB - why is this mandatory? - leave for now
        contentF->hd = false; //HD or SD
        
        Content * content = new Content();
        content->formats->push_back(contentF);
        content->uuid = uniqueId;
        //content->r;
        //content->seasonUuid;
        //content->seriesUuid;
        content->sy = description; //Mandatory - synopsis
        //content->marketingMessage;
        content->t = title; //Mandatory - title of programme
        
        return content;
    }
    
    ProgrammeNode * createProgrammeNode (){
        ProgrammeNode * programme = new ProgrammeNode();
        programme->nodeId = uniqueId;
        programme->t = title;
        //programme->renderhints = ;
        programme->sy = description;  //synopsis
        //programme->uuid;
        //programme->seasonuuid;
        //programme->seriesuuid;
        //programme->episodeNumber;
        //programme->seasonNumber;
        programme->episodeTitle = secondaryTitle;
        //programme->int availEndTime;
        //programme->broadcastTime = pubDate; TODO date conversion
        programme->provider = provider;
        //programme->cutv;
        return programme;
    }
    
};

#endif /* defined(__BytePlay__Item1MS__) */
