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
    std::ifstream fin;
    int numberOfCities = 0;
    std::string filename = "";
    GraphMatrix graphMatrix;

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
    FileParser() = default;
    ~FileParser() = default;

    explicit FileParser(GraphMatrix gm);

    bool parse();

    bool parse(const std::string &filename);

    const GraphMatrix& getGraphMatrix();
    int getNumberOfCities() const;

    const std::string &getFilename() const;
    void setFilename(const std::string &filename);



};


#endif //PEA_3_FILEPARSER_H
