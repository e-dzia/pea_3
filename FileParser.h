//
// Created by Edzia on 2017-12-30.
//

#ifndef PEA_3_FILEPARSER_H
#define PEA_3_FILEPARSER_H

#include <string>
#include <fstream>

class FileParser {
    std::ifstream fin = nullptr;

    explicit FileParser(std::string filename){
        fin.open(filename.c_str());
    }

    

};


#endif //PEA_3_FILEPARSER_H
