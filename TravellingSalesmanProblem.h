//
// Created by Edzia on 2017-05-21.
//

#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include "Timer.h"

struct TabuElement{
    int* solution;
    int lifetime;
};

class TravellingSalesmanProblem{
private:
    enum neighbourhood{
        SWAP = 0,
        INSERT = 1,
        INVERT = 2
    };

    GraphMatrix gm;
    int numberOfCities;
    bool diversification = true; //czy dywersyfikacja jest wlaczaona, 0 - nie, 1 - tak
    double stopCriterium = INT32_MAX; //czas w sekundach
    neighbourhood currentNeighbourhood = SWAP;
    std::vector<TabuElement> tabuList;
    int numberOfIterations = 5000;
    int start = 0;

    void swap(int *permutation, int left, int right);
    void insert(int *permutation, int left, int right);
    void invert(int *permutation, int left, int right);

    void newSolution(int *result_permutation);
    void permute(int *permutation, int left, int right, int &min, int *result);
    int countPath(int *permutation);

public:
    TravellingSalesmanProblem();

    std::string bruteForce();

    std::string tabuSearch();

    bool loadFromFile(std::string filename);

    void generateRandom(int size);

    double testTime(int algorithmType);

    void saveToFile(std::string filename);

    void menu();

    bool inTabuList(int *current_permutation);

    void restart(int *current_permutation);

    bool CriticalEvent(int number);

    bool allVisited(bool *visited);

    void restart_random(int *permutation);

    int beginning(int *current_permutation);
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
