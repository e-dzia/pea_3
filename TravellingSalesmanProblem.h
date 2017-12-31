#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include "Timer.h"

struct TabuElement{
    int i;
    int j;
    int lifetime;
};

class TravellingSalesmanProblem{
public:
    enum Neighbourhood{
        SWAP = 0,
        INSERT = 1,
        INVERT = 2
    };

    enum CrosoverMethod{

    };

    enum MutationMethod{

    };



private:
    GraphMatrix citiesDistances;
    int numberOfCities;
    double stopCriterium = 10; //czas w sekundach
    int sizeOfPopulation = 10;
    double mutationRate = 0.5;
    double crossoverRate = 0.5;
    CrosoverMethod crossoverMethod;
    MutationMethod mutationMethod;

    int numberOfIterations = INT32_MAX;
    int start = 0;

    bool diversification = false; //czy dywersyfikacja jest wlaczaona, 0 - nie, 1 - tak
    Neighbourhood currentNeighbourhood = SWAP;
    std::vector<TabuElement> tabuList;


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
    void setCurrentNeighbourhood(Neighbourhood currentNeighbourhood);
    void setNumberOfIterations(int numberOfIterations);

    std::string tabuSearch();
    std::string geneticAlgorithm();

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
