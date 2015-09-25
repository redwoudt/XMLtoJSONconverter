//
//  Item.h
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 23/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#ifndef __BytePlay__Item__
#define __BytePlay__Item__

#include <iostream>
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

struct source{ //media:content and media:group
    int duration;
    string medium;
    string type;
    string playbackUrl;
    string sourceId;
};

struct thumbnail{
    string url;
    string scheme;
    thumbnail():scheme(""), url(""){}
    bool isEmpty() const{
        return url == "" && scheme == "";
    }
};

struct rating{
    string scheme;
    string value;
    rating():scheme(""), value(""){}
    bool isEmpty() const{
        return scheme == "" && value == "";
    }
};

struct price{
    string type;
    string sku;
    price():type(""), sku(""){}
    bool isEmpty() const{
        return type == "" && sku == "";
    }
};

struct restrictions{
    string type;
    string relationship;
    string value;
    restrictions():type(""), relationship(""), value(""){}
    bool isEmpty() const{  //TODO - add to other structs as well
        return type == "" && relationship == "" && value == "";
    }
};

struct category{
    string label;
    string scheme;
    string value;
    category():label(""), scheme(""), value(""){}
    bool isEmpty() const{
        return label == "" && scheme == "" && value == "";
    }
};

class Item{
public:
    string uniqueId;
    string provider;
    string pubDate;
    string title;
    string type;
    string description;
    string renderHints;
    vector<source *> sources;
    vector<thumbnail *> thumbnails;
    vector<category *> categories;
    rating pr;
    string secondaryTitle;
    price priceType;
    restrictions restriction;
    
public:
    Item(string p = "unknown", string id = ""): uniqueId(id), provider(p){
        pubDate = "";
        title = "";
        description = "";
        secondaryTitle = "";
    }
    ~Item(){
        // TODO need to delete vector elements
    }
    
    void display() const{
        cout << " For Item(" << uniqueId <<") " << endl
        << "pubDate: " << pubDate << endl
        << "title: " << title << endl
        << "description: " << description << endl
        << "secondaryTitle: " << secondaryTitle << endl
        << "provider: " << provider<< endl;
        if (!pr.isEmpty()){
            cout << "rating: scheme " << pr.scheme << ", value "<<pr.value << endl;
        }
        if (!priceType.isEmpty()){
            cout << "price: type " <<priceType.type << ", sku " <<priceType.sku << endl;
        }
        if (!restriction.isEmpty()){
            cout << "restrictions: type " <<restriction.type << ", relationship " <<restriction.relationship << ", value " << restriction.value  << endl;
        }
        
        if (!sources.empty()){
            cout << "sources: {" << endl;
            for (const auto & source: sources){
                cout << "{ " << source->duration << "; " << source->medium << "; " << source->playbackUrl << "; " << source->type << "; " << source->sourceId << "}" << endl;
            }
            cout << "}" << endl;
        }
        
        if (!thumbnails.empty()){
            cout << "thumbnails: {" << endl;
            for (const auto & thumbnail: thumbnails){
                cout << "{ " << thumbnail->scheme << "; " << thumbnail->url << "}" << endl;
            }
            cout << "}" << endl;
        }
        
        if (!categories.empty()){
            cout << "categories: {" << endl;
            for (const auto & category: categories){
                cout << "{ " << category->label << "; " << category->scheme << "; " << category->value << "}" << endl;
            }
            cout << "}" << endl;
        }
        
    }
    
    string & getProvider(){
        return provider;
    }
    
    vector<category *> & getCategories() {
        return categories;
    }
    
    void addCategory(category * cat){
        if (cat != nullptr){
            categories.push_back(cat);
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
    
    void setRestrictions(restrictions & bad){
        restriction = bad;
    }
    
    void setProvider(const string &str){
        provider = str;
    }
    
    void setSecondaryTitle(const string &str){
        secondaryTitle = str;
    }
    vector<source *> & getSources(){
        return sources;
    }
    
    void addSource(source * src){
        if (src != nullptr){
            sources.push_back(src);
        }
    }
    
    void addThumbnail(thumbnail * img){
        if (img != nullptr){
            thumbnails.push_back(img);
        }
    }
    void setRating (rating & rate){
        pr = rate;
    }
    
    price & getPrice(){
        return priceType;
    }
    
    void setPrice(price & money){
        priceType = money;
    }
};



#endif /* defined(__BytePlay__Item__) */
