//
//  Parser_1MS.cpp
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 01/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "Parser_1MS.h"
#include "MenuNode.h"
#include "Content.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

ifstream & operator>>(ifstream &input, Parser_1MS &parser){
    cout << "Parsing file..." << endl;
    string current;
    bool bCollectData = false;
    Item1MS * item = nullptr;
    MenuNode * channel = nullptr;
    unordered_map<string, MenuChildNode *> categories;
    unordered_map<string, MenuChildNode *> labels;
    
    int channelItemCounter = 0;
    int channelCounter = 0;
    
    //build item
    while(getline(input, current)/*input>>current*/){
    
        if (bCollectData){
            if (current.find("<pubDate>")!=string::npos){
                size_t begin = current.find(">");
                size_t end = current.find("</");
                if (begin != string::npos || end != string::npos){
                    begin +=1;
                    if (begin < end){
                        item->setPubDate(current.substr(begin, end-begin));
                    }
                    
                }
            }
            else if (current.find("<media:title")!=string::npos){
                while (getline(input, current)/*input>>current*/){
                    if (current.find("</media:title>")!=string::npos){
                        break;
                    }
                    size_t pos = current.find("CDATA");
                    if (pos != string::npos){
                        size_t begin = current.find("[", pos);
                        if (begin != string::npos){
                            string str = current.substr(begin+1);
                            size_t end = str.find("]]>");
                            if (end != string::npos){
                                str = str.substr(0, end);
                            }
                            item->setTitle(str);
                        }
                    }
                }
            }
            else if (current.find("<media:description")!=string::npos){
                while (getline(input, current)/*input>>current*/){
                    if (current.find("</media:description>")!=string::npos){
                        break;
                    }
                    size_t pos = current.find("CDATA");
                    if (pos != string::npos){
                        size_t begin = current.find("[", pos);
                        if (begin != string::npos){
                            string str = current.substr(begin+1);
                            size_t end = str.find("]]>");
                            if (end != string::npos){
                                str = str.substr(0, end);
                                item->setDescription(str);
                            }
                        }
                    }
                }
            }
            else if (current.find("<media:content")!=string::npos){
                string type = parser.getKeyValue(current, "type");
                string playbackURL = parser.getKeyValue(current, "url");
                
                if (playbackURL != "" && type == "application/x-mpegurl"){
                    item->setPlaybackUrl(playbackURL);
                    string sourceId = parser.getKeyValue(current, "sourceId");
                    item->setSourceId(sourceId);
                    string duration = parser.getKeyValue(current, "duration");
                    item->setDuration(atoi(duration.c_str()));
                }
                
                
            }
            else if (current.find("<media:thumbnail")!=string::npos){
                string scheme = parser.getKeyValue(current, "scheme");
                string url = parser.getKeyValue(current, "url");
                if (scheme == ""){
                    item->setThumbnailUrl(url);
                }
            }
            else if (current.find("<media:category")!=string::npos){
                string label = parser.getKeyValue(current, "label"); //this is the series name
                if (label != ""){
                    item->setLabel(label);
                    //get scheme type (series heading/child node OR episode)
                    string scheme = parser.getKeyValue(current, "scheme");
                    if (scheme.find(EPISODE)!=string::npos){
                        item->setLabelScheme(EPISODE);
                    }
                    else if (scheme.find(SERIES)!=string::npos){
                        item->setLabelScheme(SERIES);
                    }
                    //get episode number
                    size_t start = current.find(":episode\"");
                    start = current.find(">", start);
                    if (start != string::npos) start+=1;
                    size_t end = current.find("<", start);
                    if (start != string::npos && end != string::npos && end > start){
                        string episode = current.substr(start,end-start);
                        double ep = stod(episode);
                        item->setEpisode(ep);
                    }
                }
                else {
                    size_t start = current.find(">");
                    if (start != string::npos) start += 1;
                    size_t end = current.find("<", start);
                    if (start != string::npos && end != string::npos && end > start){
                        string category = current.substr(start, end - start);
                        if (parser.isCategory(category)){
                            item->addCategory(category);
                        }
                    }
                }
                
                
            }
        }
        
        //each item will be a ProgrammeNode, with the expection of the first item that has an empty content url and source id (3 matches: first item, no url and no source id)
        
        if(current.find("<item>")!=string::npos){
            item = new Item1MS(parser.provider);
            channelItemCounter++;
            bCollectData = true;
        }
        else if (current.find("</item>")!=string::npos){
            //different the following items differently:
            // 1. channel header
            // 2. item with label
            // 3. item with categories
            
            if (item != nullptr){
                //item->display();
                if (parser.isChannelHeader(item)){
                    string id = "Channel"+ std::to_string(channelCounter) + std::to_string(50000 + channelItemCounter);
                    item->setUniqueId(id);
                    //create programme
                    ProgrammeNode * programme = item->createProgrammeNode();
                    channel->setBrandUri(item->thumbnailUrl);
                    channel->setData(programme);
                    delete programme;
                }
                else {
                    if (item->label != ""){
                        if (!labels[item->label]){
                            labels[item->label] = new MenuChildNode(item->provider);
                            channel->addChildNode(labels[item->label]);
                            labels[item->label]->setTitle(item->label);
                        }
                        //check if series metadata
                        if (item->labelScheme == SERIES){
                            //fill in metadata for series
                            string id = "Series"+item->sourceId +  std::to_string(channelCounter) + std::to_string(40000 + channelItemCounter); //nodeId
                            item->setUniqueId(id);
                            MenuChildNode * childNode = item->createChildNode();
                            labels[item->label]->setData(childNode);
                            delete childNode;
                        }
                        //check if episode in series
                        else if (item->labelScheme == EPISODE){
                            //fill in metadata for programme and add to series
                            string id = "Episode"+item->sourceId +  std::to_string(channelCounter) + std::to_string(10000 + channelItemCounter); //nodeId
                            item->setUniqueId(id);
                            ProgrammeNode * program = item->createProgrammeNode();
                            labels[item->label]->programmeNode.push_back(program);
                            Content * content = item->createContent();
                            parser.contentItems.push_back(content);
                        }
                    }
                    if (item->categories != nullptr && !item->categories->empty()){
                        for (const auto & category : *item->categories){
                            if (!categories[category]){
                                categories[category] = new MenuChildNode(item->provider);
                                categories[category]->setTitle(category);
                                string id = "Category"+ item->sourceId +  std::to_string(channelCounter) + std::to_string(30000 + channelItemCounter); //nodeId
                                categories[category]->setNodeId(id);
                                channel->addChildNode(categories[category]);
                            }
                            string id = "Episode"+item->sourceId +  std::to_string(channelCounter) + std::to_string(20000 + channelItemCounter); //nodeId
                            item->setUniqueId(id);
                            ProgrammeNode * program = item->createProgrammeNode();
                            categories[category]->programmeNode.push_back(program);
                            Content * content = item->createContent();
                            parser.contentItems.push_back(content);
                        }
                        
                    }
                }
            }
            
            //now reset
            bCollectData = false;
            item = nullptr;
            
        }
        else if (current.find("<channel>")!=string::npos){
            channelCounter++;
            channel = new MenuNode();
            parser.channels.push_back(channel);
        }
        else if(current.find("</channel>")!=string::npos){
            //channel->display();
            if (channel != nullptr){
                //set channel metadata
                channelItemCounter = 0;
                channel = nullptr;
            }
            
        }
    }
#if 0
    //display labels
    for (const auto & label : labels){
        label.second->display();
    }
#endif
#if 0
    //display categories
    for (const auto & category : categories){
        category.second->display();
    }
#endif
#if 0
    //display content per content unique ID
    for (int i = 0; i < channels.size(); i++){
        cout <<  i << ":  channel " << endl;
        channels[i]->display();
        cout << endl;
    }
#endif
#if 0
    //display content 
    for (const auto & content : contentItems){
        cout << content->uuid << ":  content " << endl;
        content->display();
        cout << endl;
        
    }
#endif
    //next is to output the files:
    //contentdetails directory contains list of files with name being the programmeId/nodeId
    //cataloguenode directoy contains list of files with parent menu and child menus, named after parent nodeId(only one at the moment for only one channel); and list of files with child menu and programme list (named after child menu nodeId)
    
    return input;
};

#define CATALOGUE_DIR "/Users/fre04/Documents/NicheVideo/1mainstream/catalognode/"
#define CONTENT_DETAILS_DIR "/Users/fre04/Documents/NicheVideo/1mainstream/contentdetails/"

ostream & operator<<(ostream &output, Parser_1MS &parser){
    output << "Starting with catalognode output..." << endl;
    for (const auto & channel : parser.channels){
        //remove files in directory
        DIR *channelDir = opendir(CATALOGUE_DIR);
        
        if (channelDir){
            struct dirent * next_file;
            char filepath[256];
            while(next_file = readdir(channelDir)){
                // build the full path for each file in the folder
                sprintf(filepath, "%s/%s", CATALOGUE_DIR, next_file->d_name);
                remove(filepath);
            }
        }
        
        //create channel files:
        mkdir(CATALOGUE_DIR, 0777);
        ofstream channelFile (CATALOGUE_DIR + channel->nodeId +".txt");
        //push out channel metadata:
        if (channelFile.is_open()){
            parser.addChannelMetadata(channelFile, channel);
        }
        if (channel->childrenNodes){
            for (const auto & node : *channel->childrenNodes){
                cout << "."<<std::flush;
                if (node->nodeId != ""){
                    ofstream nodeFile (CATALOGUE_DIR + node->nodeId +".txt");
                    if (nodeFile.is_open()){
                        parser.addChildMetadata(nodeFile, node);
                        nodeFile.close();
                    }
                }
            }
        }
        channelFile.close();
        cout << endl;
    }

    output << "Starting with contentdetails output..." << endl;
    int counter = 1;
    //remove files in directory
    DIR *channelDir = opendir(CONTENT_DETAILS_DIR);
    
    if (channelDir){
        struct dirent * next_file;
        char filepath[256];
        while(next_file = readdir(channelDir)){
            // build the full path for each file in the folder
            sprintf(filepath, "%s/%s", CONTENT_DETAILS_DIR, next_file->d_name);
            remove(filepath);
        }
    }
    mkdir(CONTENT_DETAILS_DIR, 0777);
    for (const auto & item : parser.contentItems){
        cout << "." <<std::flush;
        ofstream contentFile (CONTENT_DETAILS_DIR + item->uuid +".txt");
        if (contentFile.is_open()){
            parser.addContentMetadata(contentFile, item);
            contentFile.close();
        }
    }
    cout << endl;

    output << "...Complete!" << endl;
    return output;
}