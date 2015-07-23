//
//  Parser.cpp
//  BytePlay
//
//  Created by Redelinghuys, Ferdinand on 23/07/2015.
//  Copyright (c) 2015 Redelinghuys, Ferdinand. All rights reserved.
//

#include "Parser.h"
#include "json.h"

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
    cout << platform << endl;
    
    if (platform != "undefined" && parser.root == nullptr){ //root menu
        parser.root = new MenuNode(platform, "root");
        ifstream *inputFile = nullptr;
        
        const Json::Value providers = data["contentProviders"];
        for ( int index = 0; index < providers.size(); ++index )  {
            MenuNode * child = new MenuNode(providers[index]["provider"].asString(),"Provider"+std::to_string(1000+index));
            //cout << providers[index]["dataLink"].asString() << endl;
            inputFile = new ifstream(providers[index]["dataLink"].asString());

            (*inputFile) >> (*child);
            parser.root->addMenuNode(child);
            delete inputFile;
        }
    }
        
    return input;
}

ostream & operator<<(ostream &output, Parser &parser){
    return output;
}