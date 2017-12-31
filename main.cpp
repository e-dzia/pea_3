#include <iostream>
#include <ctime>
#include <thread>
#include "TravellingSalesmanProblem.h"

void test();


int main() {
    srand(time(NULL));

    //test();

    TravellingSalesmanProblem * tsp = new TravellingSalesmanProblem;
    tsp->loadFromFile("gr21.tsp");
    tsp->menu();
    delete tsp;

    return 0;
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
            tsp->setCurrentNeighbourhood(static_cast<TravellingSalesmanProblem::Neighbourhood>(j));
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