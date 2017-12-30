//
// Created by Edzia on 2017-12-30.
//

#include <cmath>
#include "FileParser.h"

bool FileParser::parse(const std::string &filename) {
    this->filename = filename;
    parse();
}

bool FileParser::parse() {
    if (filename.empty()) {
        return false;
    }

    fin.open(this->filename.c_str());
    if (!fin.is_open()) {
        return false;
    }

    if (filename.find(".atsp")!=std::string::npos){
        return atsp();
    }
    else if (filename.find(".tsp")!=std::string::npos){
        return tsp();
    }
    else if (filename.find(".txt")!=std::string::npos){
        return txt();
    }
    else return false;
}

bool FileParser::atsp() {
    std::string tmp;
    if (!searchInFile("DIMENSION:")) return false;

    fin >> numberOfCities;
    graphMatrix.createMatrix(numberOfCities);

    if (!searchInFile("EDGE_WEIGHT_TYPE:")) return false;
    if (!searchInFile("EXPLICIT")) return false;
    if (!searchInFile("EDGE_WEIGHT_FORMAT:")) return false;
    if (!searchInFile("FULL_MATRIX")) return false;

    if (!searchInFile("EDGE_WEIGHT_SECTION")) return false;
    return fullMatrix();
}

bool FileParser::tsp() {
    std::string tmp;
    if (!searchInFile("DIMENSION:")) return false;

    fin >> numberOfCities;
    graphMatrix.createMatrix(numberOfCities);

    if (!searchInFile("EDGE_WEIGHT_TYPE:")) return false;
    fin >> tmp;
    if (tmp == ":") fin >> tmp;
    if (tmp == "EXPLICIT") {
        return explicitFile();
    }
    else if (tmp == "EUC_2D") {
        return euc2d();
    }
    return false;
}

bool FileParser::txt() {
    fin >> numberOfCities;
    graphMatrix.createMatrix(numberOfCities);
    return fullMatrix();
}

bool FileParser::searchInFile(std::string toFind) {
    std::string tmp;
    std::string toFind2;
    if (toFind.substr(toFind.size()-1,1)==":"){
        toFind2 = toFind.substr(0,toFind.size()-1);
    }
    int check = 20;
    do {
        fin >> tmp;
        check--;
    }
    while (tmp != toFind && tmp != toFind2  && check > 0);
    if (check == 0) return false;
    return true;
}

bool FileParser::explicitFile() {
    std::string tmp;
    if (!searchInFile("EDGE_WEIGHT_FORMAT:")) return false;
    fin >> tmp;
    if (!searchInFile("EDGE_WEIGHT_SECTION")) return false;
    if (tmp.find("LOWER_DIAG_ROW") != std::string::npos){
        return lowerDiagRow();
    }
    else if (tmp.find("FULL_MATRIX") != std::string::npos) {
        return fullMatrix();
    }
    else if (tmp.find("UPPER_DIAG_ROW") != std::string::npos){
        return upperDiagRow();
    }
    return false;
}

bool FileParser::euc2d() {
    if (!searchInFile("NODE_COORD_SECTION")) return false;

    double **eucCoord = new double *[numberOfCities];
    for (int i = 0; i < numberOfCities; i++) {
        eucCoord[i] = new double[2];
        int n;
        fin >> n;
        fin >> eucCoord[i][0] >> eucCoord[i][1];
    }

    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j <= i; j++) {
            double xDistance = eucCoord[i][0] - eucCoord[j][0];
            double yDistance = eucCoord[i][1] - eucCoord[j][1];
            int distance = static_cast<int>(round(sqrt(xDistance * xDistance + yDistance * yDistance)));
            graphMatrix.setEdge(i, j, distance);
            graphMatrix.setEdge(j, i, distance);
        }
    }
    delete[] eucCoord;
    return true;
}

bool FileParser::lowerDiagRow() {
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j <= i; j++) {
            int length;
            fin >> length;
            graphMatrix.setEdge(i, j, length);
            graphMatrix.setEdge(j, i, length);
        }
    }
    return true;
}

bool FileParser::fullMatrix() {
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = 0; j < numberOfCities; j++) {
            int length;
            fin >> length;
            graphMatrix.setEdge(i, j, length);
        }
    }
    return true;
}

bool FileParser::upperDiagRow() {
    for (int i = 0; i < numberOfCities; i++) {
        for (int j = i; j < numberOfCities; j++) {
            int length;
            fin >> length;
            graphMatrix.setEdge(i, j, length);
            graphMatrix.setEdge(j, i, length);
        }
    }
    return true;
}

const GraphMatrix &FileParser::getGraphMatrix() const {
    return graphMatrix;
}

int FileParser::getNumberOfCities() const {
    return numberOfCities;
}

