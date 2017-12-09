#include <iostream>
#include <ctime>
#include <thread>
#include "TravellingSalesmanProblem.h"

void test();
void test2();
void test_both();


int main() {
    srand(time(NULL));

    //test();

    TravellingSalesmanProblem * tsp = new TravellingSalesmanProblem;
    tsp->loadFromFile("ftv70.atsp");

    tsp->menu();

    //tsp->saveToFile("data_salesman.txt");
    //std::cout << tsp->testTime(3);
    //tsp->loadFromFile("dane.txt");
    //std::cout << tsp->localSearch();
    delete tsp;

    //test();
    //test_both();


    return 0;
}


void test2(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    std::ofstream fout;
    fout.open("results_test.txt");
    //int size[] = {8, 10, 12, 14, 16, 18, 20, 22};
    for (int i = 8; i < 50; i++){ //rozmiar
        //for (int j = 0; j < 100; j++){
            tsp->generateRandom(i);
            fout << i << " " << tsp->testTime(3)<< std::endl;
       // }
    }

    fout.close();
    delete tsp;
}


void test(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    tsp->setNumberOfIterations(INT32_MAX);
    std::ofstream fout;
    std::string filenames[] = {"gr21.tsp", "swiss42.tsp", "ftv70.atsp", "gr17.tsp"};
    std::string filename, result;
    unsigned long long int result_position;
    double times[] = {0.5, 1, 2, 5, 10, 15, 20, 30, 60};
    for (const auto &i : filenames) { //pliki
        tsp->loadFromFile(i);
        filename = "res_"; filename += i; filename += ".txt";
        fout.open(filename);
        for (int j = 0; j < 3; j++){ //sasiedztwo
            tsp->setCurrentNeighbourhood(static_cast<TravellingSalesmanProblem::neighbourhood>(j));
            for (int k = 0; k < 2; k++){ //dywersyfikacja
                tsp->setDiversification(static_cast<bool>(k));
                for (double time : times) { //czas wykonania programu
                    tsp->setStopCriterium(time);
                    result = tsp->tabuSearch();
                    result_position = result.find(':',27);
                    result = result.substr(result_position, result.size()- result_position-1);
                    fout << j << " " << k << " " << time << " " << result << std::endl;
                    //std::cout << j << " " << k << " " << time << " " << result << std::endl;
                }
            }
        }
        fout.close();
    }

    fout.close();
    delete tsp;
}

void test_both(){
    TravellingSalesmanProblem *tsp = new TravellingSalesmanProblem();
    std::ofstream fout;
    fout.open("results_both.txt");
    int size[] = {8,9,10,11,12};
    for (int i = 0; i < 5; i++){ //rozmiar
        for (int j = 0; j < 100; j++){
            tsp->generateRandom(size[i]);
            fout << size[i] << " " << tsp->testTime(0) << " " << tsp->testTime(3)<< std::endl;
        }
    }

    fout.close();
    delete tsp;
}