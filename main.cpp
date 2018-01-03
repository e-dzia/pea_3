#include <iostream>
#include <ctime>
#include <thread>
#include "TravellingSalesmanProblem.h"
#include "Path.h"

void test();
void test2();
void test3();


int main() {
    srand(time(NULL));

    test2();

    /*TravellingSalesmanProblem * tsp = new TravellingSalesmanProblem;
    tsp->loadFromFile("swiss42.tsp");
    //tsp->loadFromFile("ftv70.atsp");
    tsp->menu();
    delete tsp;
*/

    return 0;
}

void test(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    tsp->setNumberOfIterations(INT32_MAX);
    std::ofstream fout;
    std::string filenames[] = {"gr21.tsp", "swiss42.tsp", "ftv70.atsp"};
    std::string filename, result;
    unsigned long long int result_position;
    tsp->setStopCriterium(3);
    tsp->setMutationRate(0.01);
    tsp->setCrossoverRate(0.8);

    for (const auto &i : filenames) { //pliki
        tsp->loadFromFile(i);
        int sizes[] = {tsp->getNumberOfCities(), 2*tsp->getNumberOfCities(), 3*tsp->getNumberOfCities()};
        filename = "res_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 2; j++){ //metoda krzyżowania
            tsp->setCrossoverMethod(static_cast<TravellingSalesmanProblem::CrosoverMethod>(j));
            for (int k = 0; k < 2; k++){ //metoda mutacji
                tsp->setMutationMethod(static_cast<TravellingSalesmanProblem::MutationMethod >(k));
                for (int size : sizes) { //czas wykonania programu
                    tsp->setSizeOfPopulation(size);
                    for (int m = 0; m < 10; m++){
                        result = tsp->geneticAlgorithm();
                        result_position = result.find(':',30);
                        result = result.substr(result_position, result.size()- result_position-1);
                        fout << j << " " << k << " " << size << " " << result << std::endl;
                        std::cout << j << " " << k << " " << size << " " << result << std::endl;
                    }
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void test2(){
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

    double mutationRates[] = {0.01, 0.05, 0.1};
    for (const auto &i : filenames) { //pliki
        tsp->loadFromFile(i);
        int size = 2*tsp->getNumberOfCities(); //TODO: Wykalibrować do każdego pliku oddzielnie
        filename = "res2_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 3; j++){ //współczynnik mutacji
            tsp->setMutationRate(mutationRates[j]);
            for (int m = 0; m < 10; m++){
                result = tsp->geneticAlgorithm();
                result_position = result.find(':',30);
                result = result.substr(result_position, result.size()- result_position-1);
                fout << j << " " << size << " " << result << std::endl;
                std::cout << j << " " << size << " " << result << std::endl;
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void test3(){
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

    double mutationRates[] = {0.01, 0.05, 0.1};
    for (const auto &i : filenames) { //pliki
        tsp->loadFromFile(i);
        int size = 2*tsp->getNumberOfCities(); //TODO: Wykalibrować do każdego pliku oddzielnie
        filename = "res3_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 3; j++){ //współczynnik mutacji
            tsp->setMutationRate(mutationRates[j]);
            for (int m = 0; m < 10; m++){
                result = tsp->geneticAlgorithm();
                result_position = result.find(':',30);
                result = result.substr(result_position, result.size()- result_position-1);
                fout << j << " " << size << " " << result << std::endl;
                std::cout << j << " " << size << " " << result << std::endl;
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}