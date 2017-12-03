//
// Created by Edzia on 2017-05-21.
//

#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include "Timer.h"

/*struct TabuElement{
    int* solution;
    int lifetime;
};
*/

struct TabuElement{
    int i;
    int j;
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
    bool diversification = false; //czy dywersyfikacja jest wlaczaona, 0 - nie, 1 - tak
    double stopCriterium = INT32_MAX; //czas w sekundach
    neighbourhood currentNeighbourhood = SWAP;
    std::vector<TabuElement> tabuList;
    int numberOfIterations = 8000;
    int start = 0;

    void swap(int *permutation, int left, int right);
    void insert(int *permutation, int left, int right);
    void invert(int *permutation, int left, int right);

    TabuElement newSolution(int *result_permutation);
    void permute(int *permutation, int left, int right, int &min, int *result);
    int countPath(int *permutation);

public:
    TravellingSalesmanProblem();

    void setDiversification(bool diversification);
    void setStopCriterium(double stopCriterium);
    void setCurrentNeighbourhood(neighbourhood currentNeighbourhood);
    void setNumberOfIterations(int numberOfIterations);

    std::string bruteForce();
    std::string tabuSearch();

    bool loadFromFile(std::string filename);
    void saveToFile(std::string filename);
    void generateRandom(int size);

    double testTime(int algorithmType);
    void menu();

    bool inTabuList(int i, int j);
    void restart(int *current_permutation);
    bool CriticalEvent(int number);
    void restart_random(int *permutation);
    int beginning(int *current_permutation);
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
