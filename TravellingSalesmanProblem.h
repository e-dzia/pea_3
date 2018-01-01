#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include "Timer.h"
#include "Path.h"


class TravellingSalesmanProblem{
public:

    enum CrosoverMethod{
        XX, YY
    };

    enum MutationMethod{
        AA, BB
    };



private:
    GraphMatrix citiesDistances;
    int numberOfCities;
    double stopCriterium = 10; //czas w sekundach

    int populationSize = 10;
    double mutationRate = 0.01;
    double crossoverRate = 0.8;
    CrosoverMethod crossoverMethod;
    MutationMethod mutationMethod;

    int numberOfIterations = 20; //for debugging
    int start = 0;

    void swap(int *permutation, int left, int right);
    void insert(int *permutation, int left, int right);
    void invert(int *permutation, int left, int right);

    void permute(int *permutation, int left, int right, int &min, int *result);
    int countPath(int *permutation);

    Path bestInPopulation;
    std::vector<Path> population;

public:

    TravellingSalesmanProblem();

public:
    void setSizeOfPopulation(int sizeOfPopulation);
    void setMutationRate(double mutationRate);
    void setCrossoverRate(double crossoverRate);
    void setCrossoverMethod(CrosoverMethod crossoverMethod);
    void setMutationMethod(MutationMethod mutationMethod);


    void setStopCriterium(double stopCriterium);
    void setNumberOfIterations(int numberOfIterations);

    std::string geneticAlgorithm();

    bool loadFromFile(std::string filename);
    void saveToFile(std::string filename);
    void generateRandom(int size);

    double testTime(int algorithmType);
    void menu();

    void restart(int *current_permutation);
    void restart_random(int *permutation);
    int beginning(int *current_permutation);
};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
