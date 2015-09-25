//
//  DataTree.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 24/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__DataTree__
#define __BytePlay__DataTree__

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;
#include <unordered_map>
using std::unordered_map;
#include <queue>
using std::queue;
#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ostream;
using std::getline;
#include "json.h"
#include "Item.h"

static int numberOfCategoriesToSkip = 1;
static const string categoriesToSkip[] = {"Tastemade"};


class DataNode{
private:
    string value;
    string uniqueId;
    string title;
    string renderHints;
    string description;
    string type;
    string thumbnailUrl;
    
    //children becomes ChildNodes
    //items become ProgrammeNodes
    unordered_map<string, DataNode *> children;
    vector<Item *> items;
public:
    friend ifstream & operator>>(ifstream &input, DataNode &node);
    friend ostream & operator<<(ostream &output, DataNode &node);
    
    DataNode(string val = "", Item * item = nullptr) : value(val){
        if (item != nullptr){
            title = item->title;
            renderHints = item->renderHints;
            description = item->description;
            type = item->type;
            thumbnailUrl = ((item->thumbnails.size() > 0 )? item->thumbnails[0]->url : "");
        }
        else {
            title = "Root";
            renderHints = "{ \"template\": \"3COL\" , \"imagetype\": \"16-9\" }";
            description = "";
            type = "MENU";
            thumbnailUrl = "";
        }
    }
    void setValue(string val){value = val;}
    string getValue() {return value;}
    string getTitle() {return title;}
    string getRenderHints() {return renderHints;}
    string getType() {return type;}
    string getThumbnailUrl() {return thumbnailUrl;}
    string getDescription() {return description;}
    vector<Item *> & getItems(){return items;}
    string & getId() {return uniqueId;}
    unordered_map<string, DataNode *> & getChildren() {return children;}
    
    DataNode * findNode(string & str){
        if (this->getValue().compare(str)==0){
            return this;
        }
        for (const auto & child : this->getChildren()){
            if (child.second->findNode(str) != nullptr){
                return child.second;
            }
        }
        return nullptr;
    }
    
    void addId(int base, int counter){
        uniqueId = std::to_string(1000000*base + counter++);
        
        for (const auto & child : this->getChildren()){
            counter++;
            child.second->addId(base+1, counter);
        }
        
        for (const auto & item : items){
            counter++;
            item->uniqueId = std::to_string(1000000*(base+1) + counter);
        }
    }
    
    void AddItem(Item * item){
        if (item == nullptr) return;
        vector<category *> categories = item->getCategories();
        for (const auto & cat: categories){
            if (cat->scheme == "" && cat->label==""){
                string category ("Category");
                Item * catItem = new Item(item->getProvider());
                catItem->type = "MENU";
                catItem->title = "Category";
                catItem->renderHints = "{ \"template\": \"3COL\" , \"imagetype\": \"16-9\" }";
                
                DataNode * catStart = nullptr;
                catStart = this->findNode(category);
                if (catStart == nullptr){
                    catStart = this->AddNode(category, catItem);
                }
                //add item
                catStart->AddNode(cat->value, item);
            }
            else {
                string series ("Series");
                Item * ser = new Item(item->getProvider());
                ser->type = "MENU";
                ser->title = "Series";
                ser->renderHints = "{ \"template\": \"3COL\" , \"imagetype\": \"16-9\" }";
                
                DataNode * seriesStart = nullptr;
                seriesStart = this->findNode(series);
                if (seriesStart == nullptr){
                    seriesStart = this->AddNode(series, ser);
                }
                //add item
                seriesStart->AddNode(cat->label, item);
            }
        }
    }
    
    void AddData(Item * item){
        if (item == nullptr) return;
        items.push_back(item);
    }
    void display(){
        for (const auto & item : items){
            cout << item->title << ", ";
        }
    }
    void displayMenu(DataNode * parent = nullptr){
        int count = 1;
        cout << endl;
        unordered_map<int,DataNode *> list;
        cout << "0: go back to root" << endl;
        if (children.size()!=0){
            for (const auto & child : children){
                cout << count << ": " << child.first << " ("<< child.second->getItems().size()<< ", " << child.second->getId() <<")"<< endl;
                list[count] = child.second;
                count++;
            }
            int input;
            std::cin >> input;
            if (input == 0 && parent != nullptr){
                parent->displayMenu(parent);
            }
            else if (list[input]){
                list[input]->displayMenu(parent);
            }
        }
        else {
            for (const auto & item : items){
                cout << count << ": *" << item->title << "(" << item->uniqueId << ")*" << endl;
                count++;
            }
            int input;
            std::cin >> input;
            if (input == 0 && parent != nullptr){
                parent->displayMenu(parent);
            }
            else if (input-1 >= 0 && input-1 < items.size()){
                items[input-1]->display();
                parent->displayMenu(parent);
            }
        }
    }
    
    DataNode * AddNode(string & str, Item * item){
        if (str == "") {
            if (item != nullptr){
                this->AddData(item);
            }
            return this;
        }
        //get first category
        size_t pos = str.find('/');
        string category = "";
        if (pos == string::npos){
            category = str;
            str = "";
        }
        else {
            category = str.substr(0, pos);
            str = str.substr(pos+1);
        }
        bool bSkip = false;
        //check if it needs to be skipped
        for (int i = 0; i < numberOfCategoriesToSkip; ++i){
            if (category.compare(categoriesToSkip[i])==0){
                bSkip = true;
                break;
            }
        }
        if (bSkip){
            return this->AddNode(str, item);
        }
        else{
            if (!this->getChildren()[category]){
                if (item == nullptr){ //root menu
                    item = new Item();
                    item->title = category;
                    item->description = "Root Node for " + category;
                }
                item->type = "MENU";
                item->renderHints = "{ \"template\": \"3COL\" , \"imagetype\": \"16-9\" }";
                
                this->getChildren()[category] = new DataNode(category, item);
            }
            return this->getChildren()[category]->AddNode(str, item);
        }
    }
    
    void createContentFile(ofstream & file, Item * item){
        if (file.is_open() && item != nullptr){
            Json::Value root;
            root["programmeid"] = item->uniqueId;
            //root["seasonuuid"] = item->seasonUuid;
            //root["seriesuuid"] = item->seriesUuid;
            root["sy"] = item->description;
            root["t"] = item->title;
            Json::Value child;
            if (!item->sources.empty()){
                for (const auto & source : item->sources){
                    child.clear();
                    child["at"] = "S";
                    child["streamLink"] = source->playbackUrl;
                    child["streamType"] = source->type;
                    child["imageUri"] = item->thumbnails.empty()?"":item->thumbnails[0]->url;
                    //child["oigid"] = format->oigid;
                    child["d"] = source->duration;
                    //child["hd"] = format->hd;
                    child["provider"] = item->provider;
                    root["formats"].append(child);
                }
                
            }
            else {
                root["formats"] = Json::Value::null;
            }
            file << root;
        }
    }
    
    void createProgrammeNode(ofstream & file){
        if (file.is_open()){
            Json::Value root;
            root["nodeid"] = uniqueId;
            root["nodetype"] = type;
            root["t"] = title;
            root["imageUri"] = thumbnailUrl;
            root["renderhints"] = renderHints;
            
            Json::Value child;
            if (!items.empty()){
                for (const auto & item : items){
                    child.clear();
                    child["nodetype"] = item->type;
                    child["programmeid"] = item->uniqueId;
                    child["t"] = item->title;
                    child["sy"] = item->description;
                    child["imageUri"] = item->thumbnails.empty()? "" : item->thumbnails[0]->url;
                    //child["uuid"] = node->uuid;
                    root["childnodes"].append(child);
                }
            }
            else {
                root["childnodes"] = Json::Value::null;
            }
            file << root;
        }
    }
    
    void createMenuNode(ofstream & file){
        if (file.is_open()){
            Json::Value root;
            root["nodeid"] = uniqueId;
            root["nodetype"] = type;
            root["t"] = title;
            root["imageUri"] = thumbnailUrl;
            root["renderhints"] = renderHints;
            
            Json::Value child;
            if (!children.empty()){
                for (const auto & item : children){
                    child.clear();
                    child["nodetype"] = item.second->getType();
                    child["nodeid"] = item.second->getId();
                    child["t"] = item.second->getTitle();
                    child["renderhints"] = item.second->getRenderHints();
                    child["sy"] = item.second->getDescription();
                    child["imageUri"] = item.second->getThumbnailUrl();
                    //child["uuid"] = node->uuid;
                    root["childnodes"].append(child);
                }
            }
            else {
                root["childnodes"] = Json::Value::null;
            }
            file << root;
        }
    }
    
    void createNode(string dir){
        if (!children.empty()){
            //create menu node structure
            
            ofstream menuNodeFile (dir + "catalognode/" + uniqueId + ".json");
            cout << "#" << std::flush;
            if (menuNodeFile.is_open()){
                createMenuNode(menuNodeFile);
            }
            menuNodeFile.close();
            for (const auto & child : children){
                child.second->createNode(dir);
            }
        }
        else if (!items.empty()){
            //create programme node structure
            ofstream programmeNodeFile (dir + "catalognode/" + uniqueId + ".json");
            cout << "*" << std::flush;
            if (programmeNodeFile.is_open()){
                createProgrammeNode(programmeNodeFile);
            }
            programmeNodeFile.close();
            //create content node for each item
            for(const auto & item : items){
                ofstream contentFile (dir + "contentdetails/" + item->uniqueId + ".json");
                cout << "." << std::flush;
                if (contentFile.is_open()){
                    createContentFile(contentFile, item);
                }
                contentFile.close();
            }
        }
        else {
            //ignore
        }
    }
};

static bool compareLeaves (Item * leaveA, Item * leaveB) {
    return (leaveA->title.compare(leaveB->title)<=0);
}

class DataTree{
private:
    DataNode * root;
public:
    DataTree(string val = "root"){
        root = new DataNode(val);
    }
    ~DataTree(){
        delete root;
    }
    
    void display(){
#if 0
        queue<DataNode *> toPrint;
        toPrint.push(root);
        int counter = 0;
        DataNode * newLine = new DataNode("\n" + std::to_string(counter++)+ ": ");
        toPrint.push(newLine);
        while(!toPrint.empty()){
            DataNode * front = toPrint.front();
            toPrint.pop();
            for (const auto & child : front->getChildren()){
                toPrint.push(child.second);
            }
            if (!front->getChildren().empty()){
                DataNode * newLine = new DataNode("\n" + std::to_string(counter++)+ ": ");
                toPrint.push(newLine);
            }
            cout << front->getValue() << "{";
            front->display() ;
            cout << "}" << ". ";
        }
        delete newLine;
#else
        root->displayMenu(root);
#endif
    }
    void addId(){
        root->addId(0, 0);
    }
    
    void sortLeaves(){
        queue<DataNode *> toSort;
        toSort.push(root);
        while(!toSort.empty()){
            DataNode * front = toSort.front();
            toSort.pop();
            for (const auto & child : front->getChildren()){
                toSort.push(child.second);
            }
            if (!front->getItems().empty()){
                std::sort(front->getItems().begin(), front->getItems().end(), compareLeaves);
            }
        }
    }
    
    DataNode * AddNode(string & str, Item * item = nullptr){
        return root->AddNode(str, item);
    }
    
    void createNode(string dir){
        root->createNode(dir);
    }
    
};
#endif /* defined(__BytePlay__DataTree__) */
