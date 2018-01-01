#include "TravellingSalesmanProblem.h"
#include "FileParser.h"
#include "Path.h"
#include <sstream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdlib>

std::string TravellingSalesmanProblem::geneticAlgorithm() {
    Timer t;
    t.start();

    for(int i = 0; i < populationSize+5; i++){
        Path path;
        path.setRandom();
        population.push_back(path);
        if (path.getLength() < bestInPopulation.getLength()){
            bestInPopulation = path;
        }
    }

    int nOI = 0;
    while (/*t.getWithoutStopping() < stopCriterium && */ nOI++ < numberOfIterations){
        chooseMatingPool();
        crossover();
        mutation();
        newPopulation();
        checkBest();
    }

    std::stringstream ss;
    ss << "Algorytm genetyczny.\nWynik " << std::endl;
    ss << bestInPopulation;
    ss << ": " << bestInPopulation.getLength() << std::endl;

    return ss.str();
}

void TravellingSalesmanProblem::chooseMatingPool() {

}

void TravellingSalesmanProblem::crossover() {

}

void TravellingSalesmanProblem::mutation() {
    for (Path p: population){
        int tmp = rand()%100;
        if (tmp <= mutationRate*100){
            switch(mutationMethod){
                case INSERT:
                    p.insert(rand()%numberOfCities,rand()%numberOfCities);
                    break;
                case INVERT:
                    p.invert(rand()%numberOfCities,rand()%numberOfCities);
                    break;
            }
        }
    }
}

void TravellingSalesmanProblem::newPopulation() {
    sortPopulation();
    if (population.size() > populationSize){
        deleteDuplicates();
    }
    if (population.size() > populationSize){
        deleteWorst();
    }
}

void TravellingSalesmanProblem::checkBest() {
    for (const Path &p: population){
        if (p.getLength() < bestInPopulation.getLength()){
            bestInPopulation = p;
        }
    }
}

void TravellingSalesmanProblem::sortPopulation() {
    std::sort(population.begin(), population.end());
}

void TravellingSalesmanProblem::deleteDuplicates() {
    population.erase( unique( population.begin(), population.end() ), population.end() );
}

void TravellingSalesmanProblem::deleteWorst() {
    if (population.size() > populationSize){
        for (int i = populationSize; i < population.size();){
            population.pop_back();
        }
    }
}


void TravellingSalesmanProblem::restart_random(int *current_permutation) { //restart calkowicie losowy - nieuzywana funkcja, okazala sie niezbyt skuteczna
    bool* visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    visited[0] = true;
    current_permutation[0] = 0;
    for (int i = 1; i < numberOfCities; i++){
        do{
            current_permutation[i] = rand()%numberOfCities;
        }while(visited[current_permutation[i]]);
        visited[current_permutation[i]] = true;
    }
    delete[] visited;
}

void TravellingSalesmanProblem::restart(int *current_permutation) { //restart - algorytm zachlanny, w ktorym drugi wierzcholek jest losowy
    bool *visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }

    int k = start;
    current_permutation[0] = start; //pierwszy wierzcholek to wierzcholek startowy
    visited[start] = true;
    current_permutation[1] = rand()%(numberOfCities-1)+1; //wylosowanie drugiego wierzcholka
    visited[current_permutation[1]] = true;
    for(int i = 2; i < numberOfCities; i++){ //algorytm zachlanny szuka kolejnych wierzcholkow
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++){ //szukanie minimum od obecnej pozycji
            if (!visited[j] && citiesDistances.getEdgeLength(k,j) != -1 && citiesDistances.getEdgeLength(k,j) < min){
                min = citiesDistances.getEdgeLength(k,j);
                position = j;
            }
        }
        k = position;
        current_permutation[i] = position;
    }
    delete[] visited;
}

int TravellingSalesmanProblem::beginning(int *current_permutation) { //zwykly algorytm zachlanny szuka poczatkowego rozwiazania
    bool *visited = new bool[numberOfCities];
    int length = 0;

    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    int k = start;

    current_permutation[0] = start;
    for(int i = 1; i < numberOfCities; i++){ //algorytm zachlanny szuka ustawienia poczatkowego
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++){ //szukanie minimum od obecnej pozycji
            if (!visited[j] && citiesDistances.getEdgeLength(k,j) != -1 && citiesDistances.getEdgeLength(k,j) < min){
                min = citiesDistances.getEdgeLength(k,j);
                position = j;
            }
        }
        length += min;
        k = position;
        current_permutation[i] = position;
    }
    length += citiesDistances.getEdgeLength(k,start);

    delete[] visited;
    return length;
}

/*void TravellingSalesmanProblem::permute(int *permutation, int left, int right, int &min, int *result) {
    if (left == right){
        int length = countPath(permutation);
        if (length < min){
            min = length;
            for (int i = 0; i < numberOfCities; i++){
                result[i] =  permutation[i];
            }
        }
    }
    else
    {
        for (int i = left; i <= right; i++)
        {
            swap(permutation, i, left);
            permute(permutation, left + 1, right, min, result);
            swap(permutation, i, left); //powrót do poprzedniego
        }
    }

}
*/
void TravellingSalesmanProblem::saveToFile(std::string filename) {
    std::ofstream fout;
    fout.open(filename.c_str());
    fout << numberOfCities << std::endl;
    for (int i = 0; i < numberOfCities; i++){
        for (int j = 0; j < numberOfCities; j++){
            int length = citiesDistances.getEdgeLength(i,j);
            fout << length << " ";
        }
        fout << std::endl;
    }
}

bool TravellingSalesmanProblem::loadFromFile(std::string filename) {
    FileParser fileParser;
    if (fileParser.parse(filename)){
        citiesDistances = fileParser.getGraphMatrix();
        numberOfCities = fileParser.getNumberOfCities();
        Path::setCitiesDistances(citiesDistances);
       return true;
    }
    else return false;
}

void TravellingSalesmanProblem::generateRandom(int size) {
    numberOfCities = size;
    citiesDistances.createRandom(numberOfCities,100);
    //citiesDistances.makeBothWaysEqual();
}

void TravellingSalesmanProblem::menu() {
    std::cout << "MENU - Problem komiwojazera\n"
            "1. Wczytaj z pliku.\n"
            "2. Wprowadz kryterium stopu. Teraz: " << stopCriterium << " s.\n"
            "3. Wielkosc populacji poczatkowej. Teraz: " << populationSize << ".\n"
            "4. Wspolczynnik mutacji. Teraz: " << mutationRate << ".\n"
            "5. Wspolczynnik krzyzowania. Teraz: " << crossoverRate << ".\n"
            "6. Metoda krzyzowania. Teraz: " << crossoverMethod << ". "
            "(0 - XX, 1 - YY)\n"
            "7. Metoda mutacji. Teraz: " << mutationMethod << ". "
            "(0 - INSERT, 1 - INVERT)\n"
            "8. Uruchom algorytm genetyczny.\n"
            "9. Wyjdz.\n"
            "Prosze wpisac odpowiednia liczbe.\n";
    int chosen;
    std::string file_name;
    std::cin >> chosen;
    switch(chosen){
        case 1:
            std::cout << "Prosze podac nazwe pliku.\n";
            std::cin >> file_name;
            if (this->loadFromFile(file_name)){
                std::cout << "Poprawnie wczytano.\n";
                std::cout << citiesDistances;
            }
            else std::cout << "Nie udalo sie wczytac pliku.\n";
            break;
        case 2:
            std::cout << "Prosze podac kryterium stopu (w sekundach).\n";
            std::cin >> stopCriterium;
            break;
        case 3:
            std::cout << "Prosze podac wielkoc populacji poczatkowej.\n";
            std::cin >> populationSize;
            break;
        case 4:
            std::cout << "Prosze podac wielkoc wspolczynnik mutacji.\n";
            std::cin >> mutationRate;
            break;
        case 5:
            std::cout << "Prosze podac wielkoc wspolczynnik krzyzowania.\n";
            std::cin >> crossoverRate;
            break;
        case 6:
            std::cout << "Prosze wybrac metode krzyzowania.\n"
                    "0 - XX\n"
                    "1 - YY\n";
            int crossover;
            std::cin >> crossover;
            switch(crossover){
                case 0:
                    crossoverMethod = XX;
                    break;
                case 1:
                    crossoverMethod = YY;
                    break;
                default:break;
            }
            break;
        case 7:
            std::cout << "Prosze wybrac metode mutacji.\n"
                    "0 - INSERT\n"
                    "1 - INVERT\n";
            int mutation;
            std::cin >> mutation;
            switch(mutation){
                case 0:
                    mutationMethod = INSERT;
                    break;
                case 1:
                    mutationMethod = INVERT;
                    break;
                default:break;
            }
            break;
        case 8:
            std::cout << "\n########################################\n" << this->geneticAlgorithm();
            break;
        case 9:
            return;
        default:
            std::cout  << "Prosze podac poprawna liczbe.\n";
            std::cin.clear();
            std::cin.sync();
            break;
    }
    this->menu();
}

double TravellingSalesmanProblem::testTime(int algorithmType) {
    /*algorithmType:
     * 0 - bruteforce
     * 1 - tabu search
     * */

    Timer *timer = new Timer;
    //std::chrono::nanoseconds time_start;
    //std::chrono::nanoseconds time_end;
    //double time_duration;

    //this->loadFromFile("data_salesman.txt");
    switch (algorithmType){
        case 1:
            timer->start();
            this->geneticAlgorithm();
            timer->stop();
            break;
    }


    //return (time_end - time_start) / std::chrono::nanoseconds(1);
    return timer->get();
}

TravellingSalesmanProblem::TravellingSalesmanProblem() {
}

void TravellingSalesmanProblem::setStopCriterium(double stopCriterium) {
    TravellingSalesmanProblem::stopCriterium = stopCriterium;
}

void TravellingSalesmanProblem::setNumberOfIterations(int numberOfIterations) {
    TravellingSalesmanProblem::numberOfIterations = numberOfIterations;
}

void TravellingSalesmanProblem::setSizeOfPopulation(int sizeOfPopulation) {
    TravellingSalesmanProblem::populationSize = sizeOfPopulation;
}

void TravellingSalesmanProblem::setMutationRate(double mutationRate) {
    TravellingSalesmanProblem::mutationRate = mutationRate;
}

void TravellingSalesmanProblem::setCrossoverRate(double crossoverRate) {
    TravellingSalesmanProblem::crossoverRate = crossoverRate;
}

void TravellingSalesmanProblem::setCrossoverMethod(TravellingSalesmanProblem::CrosoverMethod crossoverMethod) {
    TravellingSalesmanProblem::crossoverMethod = crossoverMethod;
}

void TravellingSalesmanProblem::setMutationMethod(TravellingSalesmanProblem::MutationMethod mutationMethod) {
    TravellingSalesmanProblem::mutationMethod = mutationMethod;
}

void TravellingSalesmanProblem::printPopulation() {
    for (Path p: population){
        std::cout << p.getLength() << " ##### " << p;
    }
}
