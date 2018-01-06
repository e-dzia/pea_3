#include "TravellingSalesmanProblem.h"
#include "FileParser.h"
#include "Path.h"
#include <sstream>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <cstdlib>
#include <set>
#include <random>


std::string TravellingSalesmanProblem::geneticAlgorithm() {
    population.clear();
    bestInPopulation.setRandom();

    Timer t;
    t.start();

    Path p;
    p.setGreedy();
    population.push_back(p);

    for(int i = 1; i < populationSize; i++){
        Path path;
        path.setRandom();
        population.push_back(path);
        if (path.getLength() < bestInPopulation.getLength()){
            bestInPopulation = path;
        }
    }

    //int nOI = 0; //used for debugging
    while (t.getWithoutStopping() < stopCriterium /*&&  nOI++ < numberOfIterations*/){
        chooseMatingPool();
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
    int matingPoolSize = populationSize/2;
    for (int i = 0; i < matingPoolSize; i++){
        int tmp = rand()%100;
        if (tmp <= crossoverRate*100) {
            Path mother = selectRandomParent();
            Path father = selectRandomParent();
            if (mother == father);
            else {
                crossover(mother, father);
            }
        }
    }
}

Path TravellingSalesmanProblem::selectRandomParent() {
    double sum = 0;
    for (const Path &p: population){
        sum += 1.0/p.getLength();
    }

    std::uniform_real_distribution < double > dist( 0, sum );
    double random = dist( rnd );

    double search = 0;
    double previous = search;
    for (const Path &p: population){
        search += 1.0/p.getLength();
        if (random < search && random >= previous) return p;
        previous = search;
    }
    return population[population.size()-1];
}

void TravellingSalesmanProblem::crossover(const Path &mother, const Path &father) {
    Path child;
    switch (crossoverMethod) {
        case PMX:
            child = mother.crossoverPMXfirstChild(father, rand()%numberOfCities, rand()%numberOfCities);
            population.push_back(child);
            child = mother.crossoverPMXsecondChild(father, rand()%numberOfCities, rand()%numberOfCities);
            population.push_back(child);
            break;
        case OX:
            child = mother.crossoverOXfirstChild(father, rand()%numberOfCities, rand()%numberOfCities);
            population.push_back(child);
            child = mother.crossoverOXsecondChild(father, rand()%numberOfCities, rand()%numberOfCities);
            population.push_back(child);
            break;
    }
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
    deleteDuplicates();
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
    std::set<Path> s;
    for (Path i : population)
        s.insert(i);
    population.assign( s.begin(), s.end() );
}

void TravellingSalesmanProblem::deleteWorst() {
    if (population.size() > populationSize){
        for (int i = populationSize; i < population.size();){
            population.pop_back();
        }
    }
}

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

void TravellingSalesmanProblem::menu() {
    std::cout << "MENU - Problem komiwojazera\n"
            "1. Wczytaj z pliku.\n"
            "2. Wprowadz kryterium stopu. Teraz: " << stopCriterium << " s.\n"
            "3. Wielkosc populacji poczatkowej. Teraz: " << populationSize << ".\n"
            "4. Wspolczynnik mutacji. Teraz: " << mutationRate << ".\n"
            "5. Wspolczynnik krzyzowania. Teraz: " << crossoverRate << ".\n"
            "6. Metoda krzyzowania. Teraz: " << crossoverMethod << ". "
            "(0 - PMX, 1 - OX)\n"
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
                    "0 - PMX\n"
                    "1 - OX\n";
            int crossover;
            std::cin >> crossover;
            switch(crossover){
                case 0:
                    crossoverMethod = PMX;
                    break;
                case 1:
                    crossoverMethod = OX;
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
    std::random_device rd;
    std::mt19937 e2(rd());
    rnd = e2;
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

int TravellingSalesmanProblem::getNumberOfCities() const {
    return numberOfCities;
}

void TravellingSalesmanProblem::printPopulation() {
    for (Path p: population){
        std::cout << p.getLength() << " ##### " << p;
    }
    std::cout << "#########################\n";
}
