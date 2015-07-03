//
//  Content.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Content__
#define __BytePlay__Content__
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <iostream>
using std::cout;
using std::endl;
using std::boolalpha;

class Format{
public:
    string at; //audio type: "M", "S", "DS" , "DD" - Mandatory
    int availEndTime;
    int broadcastTime;
    string downloadLink; //Mandatory - need URL link
    string playbackLink; //*new* Mandatory - use to playback content
    string thumbnailLink; //*new* Mandatory - use to display thumbnail
    int oigid; //Oig programme ID - mandatory
    int d; //duration in seconds - mandatory
    string provider; //Mandatory - provider of programme
    int sz; //*new* leave this, size of program in KB - why is this mandatory? - leave for now
    bool hd; //HD or SD
public:
    Format(){
        at = ""; //audio type: "M", "S", "DS" , "DD" - Mandatory
        availEndTime = 0;
        broadcastTime = 0;
        downloadLink = ""; //Mandatory - need URL link
        playbackLink = ""; //*new* Mandatory - use to playback content
        thumbnailLink = ""; //*new* Mandatory - use to display thumbnail
        oigid = 0; //Oig programme ID - mandatory
        d = 0; //duration in seconds - mandatory
        provider = ""; //Mandatory - provider of programme
        sz = 0; //*new* leave this, size of program in KB - why is this mandatory? - leave for now
        hd = false;
    }
    void display(void){
        cout << "\tFormat " << endl;
        cout << "\t{at: " << at
        << ", availEndTime: " << availEndTime
        << ", broadcastTime: " << broadcastTime
        << ", downloadLink: " << downloadLink
        << ", playbackLink: " << playbackLink
        << ", thumbnailLink: " << thumbnailLink
        << ", oigid: " << oigid
        << ", d: " << d
        << ", provider: " << provider
        << ", sz: " << sz
        << ", hd: " << boolalpha << hd << "}"<< endl;
    }
};

class Content{
public:
    vector<Format *> * formats;
    string uuid;
    string r;
    string seasonUuid;
    string seriesUuid;
    string sy; //Mandatory - synopsis
    string marketingMessage;
    string t; //Mandatory - title of programme
public:
    Content(){
        formats = new vector<Format *>();
        uuid = "";
        r = "";
        seasonUuid = "";
        seriesUuid = "";
        sy = ""; //Mandatory - synopsis
        marketingMessage = "";
        t = "";
    }
    ~Content(){
        delete formats;
    }
    void display(void){
        cout << "Content " << endl;
        cout << "{uuid: " << uuid
        << ", r: " << r
        << ", seasonUuid: " << seasonUuid
        << ", seriesUuid: " << seriesUuid
        << ", sy: " << sy
        << ", marketingMessage: " << marketingMessage
        << ", t: " << t <<  "}" << endl;
        if (formats != nullptr){
            for (const auto & format: *formats){
                format->display();
            }
        }
        
    }
};
#endif /* defined(__BytePlay__Content__) */
