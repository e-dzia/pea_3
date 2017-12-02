//
// Created by Edzia on 2017-05-21.
//

#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include "Timer.h"

class TravellingSalesmanProblem{
private:
    enum neighbourhood{
        SWAP = 0,
        INSERT = 1,
        INVERT = 2
    };

    GraphMatrix gm;
    int numberOfCities;
    bool diversification = 0; //czy dywersyfikacja jest wlaczaona, 0 - nie, 1 - tak
    double stopCriterium = 10; //czas w sekundach
    neighbourhood currentNeighbourhood = SWAP;
    std::vector<int> tabuList;

    void swap(int *permutation, int left, int right);
    void insert(int *permutation, int left, int right);
    void invert(int *permutation, int left, int right);

    bool allVisited(bool pBoolean[]);
    void permute(int *permutation, int left, int right, int &min, int *result);
    int countPath(int *permutation);

public:
    TravellingSalesmanProblem();

    std::string bruteForce();

    std::string tabuSearch();

    void loadFromFile(std::string filename);

    void generateRandom(int size);

    double testTime(int algorithmType);

    void saveToFile(std::string filename);

    void menu();
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
