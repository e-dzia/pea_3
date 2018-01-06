#ifndef SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
#define SDIZO_3_TRAVELLINGSALESMANPROBLEM_H


#include "GraphMatrix.h"
#include <fstream>
#include <vector>
#include <map>
#include <random>
#include "Timer.h"
#include "Path.h"


class TravellingSalesmanProblem{
public:

    enum CrosoverMethod{
        PMX, OX
    };

    enum MutationMethod{
        INSERT, INVERT
    };

private:
    GraphMatrix citiesDistances;
    int numberOfCities;
public:
    int getNumberOfCities() const;

private:
    double stopCriterium = 3; //czas w sekundach

    int populationSize = 50;
    double mutationRate = 0.01; //0.01
    double crossoverRate = 0.8; //0.8
    CrosoverMethod crossoverMethod = OX;
    MutationMethod mutationMethod = INSERT;

    int numberOfIterations = 1000; //for debugging
    int start = 0;

    std::mt19937 rnd;

    std::vector<Path> population;
    Path bestInPopulation;

    void chooseMatingPool();
    void crossover(const Path &mother, const Path &father);
    void mutation();
    void newPopulation();
    void checkBest();

    void sortPopulation();
    void deleteDuplicates();
    void deleteWorst();

    Path selectRandomParent();

public:

    TravellingSalesmanProblem();

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

    double testTime(int algorithmType);
    void menu();

    void printPopulation();

};


#endif //SDIZO_3_TRAVELLINGSALESMANPROBLEM_H
