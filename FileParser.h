//
// Created by Edzia on 2017-12-30.
//

#ifndef PEA_3_FILEPARSER_H
#define PEA_3_FILEPARSER_H

#include <string>
#include <fstream>
#include "GraphMatrix.h"

class FileParser {
private:
    std::ifstream fin = nullptr;
    int numberOfCities = 0;
    GraphMatrix gm = nullptr;

    bool atsp();
    bool tsp();
    bool txt();

    bool searchInFile(std::string toFind);

    bool explicitFile();
    bool euc2d();
    bool lowerDiagRow();
    bool fullMatrix();
    bool upperDiagRow();

public:
    explicit FileParser(const std::string &filename){
        fin.open(filename.c_str());
    }

    bool parse(const std::string &filename);

};


#endif //PEA_3_FILEPARSER_H
