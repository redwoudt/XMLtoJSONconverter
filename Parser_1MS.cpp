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

ifstream & operator>>(ifstream &input, Parser_1MS &parser){
    parser.display();
    string current;
    bool bCollectData = false;
    Item1MS * item = nullptr;
#if 0
    for (int i = 0; i < 50; i++){
        //input >> current;
        getline(input, current);
        cout << i << "{ " << current << " }" << endl;
    }
#endif
    vector<MenuNode *> channels;
    vector<Content *> contentItems;
    MenuNode * channel = nullptr;
    
    int channelItemCounter = 0;
    int channelCounter = 0;
    string brandUri = "";
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
            
           //item->display();
        }
        
        //each item will be a ProgrammeNode, with the expection of the first item that has an empty content url and source id (3 matches: first item, no url and no source id)
        
        if(current.find("<item>")!=string::npos){
            item = new Item1MS(parser.provider);
            channelItemCounter++;
            bCollectData = true;
        }
        else if (current.find("</item>")!=string::npos){
            if (channelItemCounter==1){
                brandUri = item->thumbnailUrl;
            }
            
            if (item != nullptr){
                string id;
                if (item->sourceId == "" && channelItemCounter == 1){
                    id = "Channel"+ std::to_string(50000*channelCounter + channelItemCounter);
                    item->setUniqueId(id);
                    //create programme
                    ProgrammeNode * programme = item->createProgrammeNode();
                    channel->setData(programme);
                }
                else if (item->sourceId != ""){
                    id = item->sourceId + std::to_string(10000*channelCounter + channelItemCounter);
                    item->setUniqueId(id);
                    //create programme
                    ProgrammeNode * programme = item->createProgrammeNode();
                    if (channel != nullptr){
                        channel->addProgramme(programme);
                    }
                    Content * content = item->createContent();
                    contentItems.push_back(content);
                }
                
                //now reset
                bCollectData = false;
                item = nullptr;
            }
            
        }
        else if (current.find("<channel>")!=string::npos){
            channelCounter++;
            channel = new MenuNode();
        }
        else if(current.find("</channel>")!=string::npos){
            if (channel != nullptr){
                //set channel metadata
                channel->setBrandUri(brandUri);
                channelItemCounter = 0;
                brandUri = "";
                channels.push_back(channel);
                channel = nullptr;
            }
            
        }
    }
#if 0
    //display content per content unique ID
    for (int i = 0; i < channels.size(); i++){
        cout <<  i << ":  channel " << endl;
        channels[i]->display();
        cout << endl;
    }

    
    //display content 
    for (const auto & content : contentItems){
        cout << content->uuid << ":  content " << endl;
        content->display();
        cout << endl;
        
    }
#endif
    
    return input;
};