#include <iostream>
#include <ctime>
#include <thread>
#include "TravellingSalesmanProblem.h"
#include "Path.h"

void test_size();
void test_crossover();
void test_mutation();


int main() {
    srand(time(NULL));

    TravellingSalesmanProblem * tsp = new TravellingSalesmanProblem;
    tsp->loadFromFile("gr21.tsp");
    tsp->menu();
    delete tsp;

    return 0;
}

void test_size(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    tsp->setNumberOfIterations(INT32_MAX);
    std::ofstream fout;
    std::string filenames[] = {"gr21.tsp", "swiss42.tsp", "ftv70.atsp"};
    std::string filename, result;
    unsigned long long int result_position;
    tsp->setStopCriterium(3);
    tsp->setMutationRate(0.01);
    tsp->setCrossoverRate(0.8);

    double times[] = {0.1, 0.3, 0.5, 0.7, 1.0, 2.0, 5.0, 8.0};

    for (const auto &i : filenames) { //pliki
        if (!tsp->loadFromFile(i)) return;
        int sizes[] = {tsp->getNumberOfCities(), 2*tsp->getNumberOfCities(), 3*tsp->getNumberOfCities()};
        filename = "res_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 2; j++){
            tsp->setCrossoverMethod(static_cast<TravellingSalesmanProblem::CrosoverMethod>(j));
            for (int k = 0; k < 2; k++){
                tsp->setMutationMethod(static_cast<TravellingSalesmanProblem::MutationMethod >(k));
                for (int size : sizes) {
                    tsp->setSizeOfPopulation(size);
                    for (double time: times){
                        tsp->setStopCriterium(time);
                        for (int m = 0; m < 10; m++){
                            result = tsp->geneticAlgorithm();
                            result_position = result.find(':',30);
                            result = result.substr(result_position, result.size()- result_position-1);
                            fout << j << " " << k << " " << size << " " << time << " " << result << std::endl;
                            //std::cout << j << " " << k << " " << size << " " << time << " " << result << std::endl;
                        }
                    }
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void test_mutation(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    tsp->setNumberOfIterations(INT32_MAX);
    std::ofstream fout;
    std::string filenames[] = {"gr21.tsp", "swiss42.tsp", "ftv70.atsp"};
    std::string filename, result;
    unsigned long long int result_position;
    tsp->setStopCriterium(3);
    tsp->setCrossoverMethod(TravellingSalesmanProblem::OX); //TODO: ocenić na podstawie poprzednich
    tsp->setMutationMethod(TravellingSalesmanProblem::INSERT); //TODO: ocenić na podstawie poprzednich
    tsp->setCrossoverRate(0.8);

    double times[] = {0.1, 0.3, 0.5, 0.7, 1.0, 2.0, 5.0, 8.0};

    double mutationRates[] = {0.01, 0.05, 0.1};
    int sizes[] = {42,126,71};
    int size = 0;
    for (const auto &i : filenames) {
        if (!tsp->loadFromFile(i)) return;
        filename = "res3_"; filename += i; filename += ".txt";
        fout.open(filename);
        tsp->setSizeOfPopulation(sizes[size++]);
        for (int j = 0; j < 2; j++){
            tsp->setCrossoverMethod(static_cast<TravellingSalesmanProblem::CrosoverMethod>(j));
            for (int k = 0; k < 2; k++){
                tsp->setMutationMethod(static_cast<TravellingSalesmanProblem::MutationMethod >(k));
                for (double mutationRate : mutationRates) {
                    tsp->setMutationRate(mutationRate);
                    for (double time: times){
                        tsp->setStopCriterium(time);
                        for (int m = 0; m < 10; m++){
                            result = tsp->geneticAlgorithm();
                            result_position = result.find(':',30);
                            result = result.substr(result_position, result.size()- result_position-1);
                            fout << j << " " << k << " " << mutationRate << " " << time << " " << result << std::endl;
                            //std::cout << j << " " << k << " " << mutationRate << " " << time << " " << result << std::endl;
                        }
                    }
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void test_crossover(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    tsp->setNumberOfIterations(INT32_MAX);
    std::ofstream fout;
    std::string filenames[] = {"ftv70.atsp"/*, "gr21.tsp", "swiss42.tsp"*/};
    std::string filename, result;
    unsigned long long int result_position;
    tsp->setStopCriterium(3);
    tsp->setCrossoverMethod(TravellingSalesmanProblem::OX); //TODO: ocenić na podstawie poprzednich
    tsp->setMutationMethod(TravellingSalesmanProblem::INSERT); //TODO: ocenić na podstawie poprzednich
    tsp->setMutationRate(0.01);


    double times[] = {0.1, 0.3, 0.5, 0.7, 1.0, 2.0, 5.0, 8.0};

    double crossoverRates[] = {0.5, 0.7, 0.9};
    int sizes[] = {71, 42, 126};
    int size = 0;
    for (const auto &i : filenames) {
        if (!tsp->loadFromFile(i)) return;
        filename = "res2_tmp_";
        filename += i;
        filename += ".txt";
        //std::cout << filename << "\n";
        fout.open(filename);
        tsp->setSizeOfPopulation(sizes[size++]);
        for (int j = 0; j < 2; j++) {
            tsp->setCrossoverMethod(static_cast<TravellingSalesmanProblem::CrosoverMethod>(j));
            for (int k = 0; k < 2; k++) {
                tsp->setMutationMethod(static_cast<TravellingSalesmanProblem::MutationMethod >(k));
                for (double crossoverRate : crossoverRates) {
                    tsp->setCrossoverRate(crossoverRate);
                    for (double time: times) {
                        tsp->setStopCriterium(time);
                        for (int m = 0; m < 10; m++) {
                            result = tsp->geneticAlgorithm();
                            result_position = result.find(':', 30);
                            result = result.substr(result_position, result.size() - result_position - 1);
                            fout << j << " " << k << " " << crossoverRate << " " << time << " " << result << std::endl;
                            //std::cout << j << " " << k << " " << crossoverRate << " " << time << " " << result << std::endl;
                        }
                    }
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}