//
//  DataTree.cpp
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 24/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "DataTree.h"
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>
#include <cstring>
#include <string.h>
#include "pugixml.h"
#include "Item.h"

namespace ITEM_ENTRY{
    const char pubDate[] = "pubDate";
    const char mediaTitle[] = "media:title";
    const char mediaDescription[] = "media:description";
    const char mediaContent[] = "media:content";
    const char mediaThumbnail[] = "media:thumbnail";
    const char mediaRating[] = "media:rating";
    const char mediaGroup[] = "media:group";
    const char mediaCategory[] = "media:category";
    const char mediaPrice[] = "media:price";
    const char mediaSecondaryTitle[] = "media:secondaryTitle";
    const char mediaRestriction[] = "media:restriction";
    const int maxSize = 100;
    
}

static int numberOfIgnores = 1;
static string categoryToIgnore[] = {"Tastemade Brasil"};

ifstream & operator>>(ifstream &input, DataNode &node){
    pugi::xml_document data;
    pugi::xml_parse_result result = data.load(input);
    
    
    cout << "Node: " << node.getValue() << endl;
    
    pugi::xml_node items = data.child("rss").child("channel");
    for (pugi::xml_node item = items.child("item"); item; item = item.next_sibling("item"))
    {
        //cout << "Item: ";
        Item * newItem = new Item(node.getValue());
        
        for (pugi::xml_node entry = item.first_child(); entry; entry = entry.next_sibling()){
            if (strncmp(entry.name(),ITEM_ENTRY::pubDate, ITEM_ENTRY::maxSize)==0){
                newItem->setPubDate(entry.first_child().value());
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaTitle, ITEM_ENTRY::maxSize)==0){
                newItem->setTitle(entry.first_child().value());
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaDescription, ITEM_ENTRY::maxSize)==0){
                newItem->setDescription(entry.first_child().value());
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaContent, ITEM_ENTRY::maxSize)==0){
                source * content = new source();
                content->type = entry.attribute("type").value();
                content->sourceId = entry.attribute("sourceId").value();
                content->medium = entry.attribute("medium").value();
                content->duration = entry.attribute("duration").as_int();
                content->playbackUrl = entry.attribute("url").value();
                newItem->addSource(content);
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaThumbnail, ITEM_ENTRY::maxSize)==0){
                thumbnail *image = new thumbnail(); //TODO - delete this
                image->scheme = entry.attribute("scheme").value();
                image->url = entry.attribute("url").value();
                newItem->addThumbnail(image);
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaRating, ITEM_ENTRY::maxSize)==0){
                rating mediaR;
                mediaR.scheme = entry.attribute("scheme").value();
                mediaR.value = entry.first_child().value();
                newItem->setRating(mediaR);
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaGroup, ITEM_ENTRY::maxSize)==0){
                //more than one mediaCategory
                source * content = nullptr; //TODO - delete this
                for (pugi::xml_node src = entry.first_child(); src; src = src.next_sibling()){
                    content = new source();
                    content->type = src.attribute("type").value();
                    content->sourceId = src.attribute("sourceId").value();
                    content->medium = src.attribute("medium").value();
                    content->duration = src.attribute("duration").as_int();
                    content->playbackUrl = src.attribute("url").value();
                    newItem->addSource(content);
                }
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaCategory, ITEM_ENTRY::maxSize)==0){
                category * cat = new category();
                cat->label = entry.attribute("label").value();
                cat->scheme = entry.attribute("scheme").value();
                cat->value = entry.first_child().value();
                newItem->addCategory(cat);
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaPrice, ITEM_ENTRY::maxSize)==0){
                price priceTag;
                priceTag.sku = entry.attribute("sku").value();
                priceTag.type = entry.attribute("type").value();
                newItem->setPrice(priceTag);
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaSecondaryTitle, ITEM_ENTRY::maxSize)==0){
                newItem->setSecondaryTitle(entry.first_child().value());
            }
            else if (strncmp(entry.name(),ITEM_ENTRY::mediaRestriction, ITEM_ENTRY::maxSize)==0){
                restrictions bad;
                bad.relationship = entry.attribute("relationship").value();
                bad.type = entry.attribute("type").value();
                bad.value = entry.first_child().value();
                newItem->setRestrictions(bad);
            }
            else {
                cout << "unknown entry type: " << entry.name() << endl;
            }
            
        }
        
        //add item
        //B) create trees from categories
        //C) add content items
        bool addItem = true;
        
        if (!newItem->getPrice().isEmpty()){
            addItem = false;
            delete newItem;
            continue;
        }
        else if (newItem->getSources().empty()){
            addItem = false;
            delete newItem;
            continue;
        }
        
        vector<category *> categories = newItem->getCategories();
        if (categories.empty()){
            addItem = false;
            delete newItem;
            continue;
        }
        else {
            for (int j = 0; j < categories.size() && addItem; ++j){
                for (int i = 0; i < numberOfIgnores; ++i){
                    if (categories[i]->value.find(categoryToIgnore[i])!=string::npos){
                        addItem = false;
                        break;
                    }
                }
            }
        }
        
        //TODO - 1) create data tree for categories
        // root will be node.t
        // then traverse through tree untill all items are covered
        // 2) create data tree for series if required (only TasteMade)
        
        if (addItem){
            //newItem->display();
            node.AddItem(newItem);
        }
        else {
            delete newItem;
        }
        
        //cout << endl;
    }
    return input;
}

ostream & operator<<(ostream &output, DataNode &node){
    return output;
}