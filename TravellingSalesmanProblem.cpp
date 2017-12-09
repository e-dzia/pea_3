//
// Created by Edzia on 2017-05-21.
//

#include "TravellingSalesmanProblem.h"
#include <sstream>
#include <chrono>
#include <cmath>

std::string TravellingSalesmanProblem::bruteForce() {
    int *permutation = new int[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        permutation[i] = i;
    }
    int *result = new int[numberOfCities];

    int min = INT32_MAX;
    permute(permutation, 1, numberOfCities - 1, min, result);

    std::stringstream ss;
    ss << "Przeglad zupelny.\nWynik: " << std::endl;
    for (int i = 0; i < numberOfCities; i++){
        ss << result[i] << " ";
    }
    ss << ": " << min << std::endl;
    return ss.str();
}

std::string TravellingSalesmanProblem::tabuSearch() {
    tabuList.clear();
    Timer t;
    t.start();
    int *current_permutation = new int[numberOfCities];
    int length = beginning(current_permutation);

    int *result_permutation = new int[numberOfCities];
    int *notRepeatable = new int[numberOfCities];
    result_permutation[0] = start;
    for (int i = 0; i < numberOfCities; i++){
        result_permutation[i] = current_permutation[i];
        notRepeatable[i] = current_permutation[i];
    }
    int result_length = length;

    int nOI = numberOfIterations;
    int numberOfIterationsWithoutChange = numberOfCities*3;
    while (nOI-->0 && t.getWithoutStopping() < stopCriterium){
        //nowe rozwiazanie
        TabuElement te = newSolution(current_permutation);
        int tmp = countPath(current_permutation);

        /*for (int i = 0; i < numberOfCities; i++){
            std::cout << current_permutation[i] << " ";
        }
         std::cout << tmp << std::endl;
        */

        if (tmp <= length){
            //std::cout << numberOfIterationsWithoutChange << std::endl;
            numberOfIterationsWithoutChange = numberOfCities*4;
            length = tmp;
            for (int i = 0; i < numberOfCities; i++){
                result_permutation[i] = current_permutation[i];
            }
        }

        //aktualizacja listy tabu: zmniejszenie kadencji i usuniecie zbednych pozycji
        for(auto it = tabuList.begin(); it != tabuList.end(); ++it) {
            it->lifetime--;

            if (it->lifetime == 0){
                //delete[] it->solution;
                tabuList.erase(it);
                --it;
            }
        }

        //TODO: dodaj nowe elementy do listy tabu
        if (tabuList.size() < numberOfCities*2){ //TODO: ???
            te.lifetime = numberOfCities;
            tabuList.insert(tabuList.begin(),te);
            /*TabuElement te{new int[numberOfCities],numberOfCities*2};
            for (int i = 0; i < numberOfCities; i++) te.solution[i] = current_permutation[i];
            tabuList.insert(tabuList.begin(),te);*/
        }


        //TODO: if criticalEvent() to restart [Dywersyfikacja]; if lepsze po restarcie, to podmien
        if(diversification && CriticalEvent(numberOfIterationsWithoutChange)){
            //std::cout << nOI << " Critical Event\n";
            restart(current_permutation);
            tmp = countPath(current_permutation);

           /* for (int i = 0; i < numberOfCities; i++){
                std::cout << current_permutation[i] << " ";
            }
            std::cout << tmp << std::endl;
*/
            numberOfIterationsWithoutChange = numberOfCities*3;
            if (tmp <= length){
                numberOfIterationsWithoutChange = numberOfCities*2;
                length = tmp;
                for (int i = 0; i < numberOfCities; i++){
                    result_permutation[i] = current_permutation[i];
                }
            }
        }
        numberOfIterationsWithoutChange--;
    }


    /*for (int i = 0; i < numberOfCities; i++){
        std::cout << current_permutation[i] << " ";
    }
    std::cout << std::endl;

    invert(current_permutation,0,3);

    for (int i = 0; i < numberOfCities; i++){
        std::cout << current_permutation[i] << " ";
    }
    std::cout << std::endl;

    invert(current_permutation,3,0);

    for (int i = 0; i < numberOfCities; i++){
        std::cout << current_permutation[i] << " ";
    }
    std::cout << std::endl;
*/

    std::stringstream ss;
    ss << "Algorytm tabu search.\nWynik: " << std::endl;
    for (int i = 0; i < numberOfCities; i++){
        ss << result_permutation[i] << " ";
    }
    //ss << result << " ";
    ss << ": " << length << std::endl;

    delete[] current_permutation;
    delete[] result_permutation;
    return ss.str();
}


void TravellingSalesmanProblem::restart_random(int *current_permutation) { //TODO: algorytm zachlanny od wylosowanego wierzcholka
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

void TravellingSalesmanProblem::restart(int *current_permutation) {
    bool *visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }

    int k = start;
    current_permutation[0] = start;
    visited[start] = true;
    current_permutation[1] = rand()%(numberOfCities-1)+1;
    visited[current_permutation[1]] = true;
    for(int i = 2; i < numberOfCities; i++){ //algorytm zachlanny szuka ustawienia poczatkowego
        visited[k] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++){ //szukanie minimum od obecnej pozycji
            if (!visited[j] && gm.getEdgeLength(k,j) != -1 && gm.getEdgeLength(k,j) < min){
                min = gm.getEdgeLength(k,j);
                position = j;
            }
        }
        k = position;
        current_permutation[i] = position;
    }
    delete[] visited;
}

bool TravellingSalesmanProblem::CriticalEvent(int number) {
    return (number <= 0);
}

/*
 * Znajduje najlepsze rozwiazanie w okolicy
 * */
TabuElement TravellingSalesmanProblem::newSolution(int *result_permutation) {
    TabuElement te; //changed
    int min = INT32_MAX;
    int *current_permutation = new int[numberOfCities];
    for (int i = 0; i < numberOfCities; i++) current_permutation[i] = result_permutation[i];

    for (int i = 1; i < numberOfCities-1; i++){
        for (int j = 1; j < numberOfCities -1; j++) {
            if (i != j && !inTabuList(i, j)) {
                switch(currentNeighbourhood){
                    case SWAP: swap(current_permutation, i, j); break;
                    case INSERT: insert(current_permutation, i, j); break;
                    case INVERT: invert(current_permutation, i, j); break;
                }
                int tmp = countPath(current_permutation);
                /*for (int k = 0; k < numberOfCities; k++){
                    std::cout << current_permutation[k] << " ";
                }
                std::cout << tmp << std::endl;*/
                if (tmp <= min) { //TODO: I nie w liscie tabu
                    te.i = i; te.j = j; //changed
                    min = tmp;
                    for (int i = 0; i < numberOfCities; i++) {
                        result_permutation[i] = current_permutation[i];
                    }
                }
                switch(currentNeighbourhood){
                        case SWAP: swap(current_permutation, i, j); break;
                        case INSERT: insert(current_permutation, j, i); break;
                        case INVERT: invert(current_permutation, i, j); break;
                }

            }
        }
    }
    return te; //changed
}

bool TravellingSalesmanProblem::inTabuList(int i, int j) {
    switch(currentNeighbourhood){
        case SWAP:
            for (auto &it : tabuList) {
                if ((it.i == i && it.j == j)||(it.i == j && it.j == i)) //przypadki, w ktorych nie zamieniamy
                    return true;
            }
            break;
        case INSERT:
            for (auto &it : tabuList) {
                if (j == it.i && i == it.j)
                    return true;
            }
            break;
        case INVERT:
            if (i < j) {int tmp = i; i = j; j = i;} //zawsze i > j
            for (auto &it : tabuList) {
                if(!((i < it.i && j < it.i)||(i > it.j && j > it.j))) //dwa przypadki, w których zamieniamy (z negacja)
                    return true;
            }
            break;
    }
    return false;
}

/*
bool TravellingSalesmanProblem::inTabuList(int *current_permutation) {
    bool inTabu = false;
    for (auto it = tabuList.begin(); it != tabuList.end(); ++it){

        inTabu = true;
        for (int i = 0; i < numberOfCities; i++) {
            if(it->solution[i]!=current_permutation[i]){
                inTabu = false;
                break;
            }
        }
        if (inTabu) return true;
    }
    return false;
}
*/

int TravellingSalesmanProblem::beginning(int *current_permutation) {
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
            if (!visited[j] && gm.getEdgeLength(k,j) != -1 && gm.getEdgeLength(k,j) < min){
                min = gm.getEdgeLength(k,j);
                position = j;
            }
        }
        length += min;
        k = position;
        current_permutation[i] = position;
    }
    length += gm.getEdgeLength(k,start);

    delete[] visited;
    return length;
}

void TravellingSalesmanProblem::permute(int *permutation, int left, int right, int &min, int *result) {
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

void TravellingSalesmanProblem::swap(int *permutation, int left, int right) {
    if (right == left) return;
    int tmp = permutation[left];
    permutation[left] = permutation[right];
    permutation[right] = tmp;
}

void TravellingSalesmanProblem::insert(int *permutation, int left, int right) {
    if (right == left) return;
    if (right < left){
        int tmp = permutation[left];
        for (int i = left; i > right; i--){
            permutation[i] = permutation[i-1];
        }
        permutation[right] = tmp;
    }
    else {
        int tmp = permutation[left];
        for (int i = left; i < right; i++){
            permutation[i] = permutation[i+1];
        }
        permutation[right] = tmp;
    }

}

void TravellingSalesmanProblem::invert(int *permutation, int left, int right) {
    if (right == left) return;
    if (right < left){int tmp = left; left = right; right = tmp;}
    for (int i = 0; i < (right - left +1)/2; i++){
        swap(permutation,left+i,right-i);
    }
}

int TravellingSalesmanProblem::countPath(int *permutation) {
    int length = 0;
    int end;
    /* for (int i = 0; i < numberOfCities; i++){
         std:: cout << permutation[i];
     }
     std::cout << std::endl;*/
    for (int i = 1; i < numberOfCities; i++){
        length += gm.getEdgeLength(permutation[i-1],permutation[i]);
        end = i;
    }
    length += gm.getEdgeLength(permutation[end],permutation[0]);

    return length;
}

void TravellingSalesmanProblem::saveToFile(std::string filename) {
    std::ofstream fout;
    fout.open(filename.c_str());
    fout << numberOfCities << std::endl;
    for (int i = 0; i < numberOfCities; i++){
        for (int j = 0; j < numberOfCities; j++){
            int length = gm.getEdgeLength(i,j);
            fout << length << " ";
        }
        fout << std::endl;
    }
}

bool TravellingSalesmanProblem::loadFromFile(std::string filename) {
    //std::cout << gm;
    std::ifstream fin;
    fin.open(filename.c_str());
    if (filename.find(".atsp")!=std::string::npos){ //atsp
        std::string tmp;
        int check = 15;
        do {
            fin >> tmp;
            check--;
        }
        while (tmp != "DIMENSION:" && check > 0);
        if (check == 0) return false;

        fin >> numberOfCities;
        gm.createMatrix(numberOfCities);
        check = 15;
        do {
            fin >> tmp;
            check--;
        }
        while (tmp != "EDGE_WEIGHT_SECTION" && check > 0);
        if (check == 0) return false;

        for (int i = 0; i < numberOfCities; i++){
            for (int j = 0; j < numberOfCities; j++){
                int length;
                fin >> length;
                //if (length == -1) length = 0;
                gm.setEdge(i,j,length);
            }
        }
        return true;
    }
    else if (filename.find(".tsp")!=std::string::npos){ //tsp
        std::string tmp;
        int check = 15;
        do {
            fin >> tmp;
            check--;
        }
        while (tmp != "DIMENSION:" && check > 0);
        if (check == 0) return false;

        fin >> numberOfCities;
        gm.createMatrix(numberOfCities);
        check = 15;
        do {
            fin >> tmp;
            check--;
        }
        while (tmp != "EDGE_WEIGHT_FORMAT:"&& check > 0);
        if (check == 0) return false;

        fin >> tmp;
        if (tmp.find("LOWER_DIAG_ROW")!=std::string::npos){
            do {
                fin >> tmp;
            }
            while (tmp != "EDGE_WEIGHT_SECTION");
            for (int i = 0; i < numberOfCities; i++){
                for (int j = 0; j <= i; j++){
                    int length;
                    fin >> length;
                    //if (length == -1) length = 0;
                    gm.setEdge(i,j,length);
                    gm.setEdge(j,i,length);
                }
            }
            return true;
        }
        else return false;
    }
    else if (filename.find(".txt")!=std::string::npos){
        fin >> numberOfCities;
        gm.createMatrix(numberOfCities);
        for (int i = 0; i < numberOfCities; i++){
            for (int j = 0; j < numberOfCities; j++){
                int length;
                fin >> length;
                //if (length == -1) length = 0;
                gm.setEdge(i,j,length);
            }
        }
        return true;
    }
    return false;
}

void TravellingSalesmanProblem::generateRandom(int size) {
    numberOfCities = size;
    gm.createRandom(numberOfCities,100);
    //gm.makeBothWaysEqual();
}

void TravellingSalesmanProblem::menu() {
    std::cout << "MENU - Problem komiwojazera\n"
            "1. Wczytaj z pliku.\n"
            "2. Wprowadz kryterium stopu. Teraz: " << stopCriterium << " s.\n"
            "3. Dywersyfikacja. Teraz: " << diversification << ". "
            "(0 - wylaczona, 1 - wlaczona).\n"
            "4. Wybor sasiedztwa. Teraz: " << currentNeighbourhood << ". "
            "(0 - swap, 1 - insert, 2 - invert).\n"
            "5. Tabu search.\n"
            "6. Wyjdz.\n"
            "7. Generuj losowo.\n"
            "8. Przeglad zupelny.\n"
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
            }
            else std::cout << "Nie udalo sie wczytac pliku.\n";
            break;
        case 2:
            std::cout << "Prosze podac kryterium stopu (w sekundach).\n";
            //int v;
            std::cin >> stopCriterium;
            //this->generateRandom(v);
            break;
        case 3:
            std::cout << "Zmieniono ustawienia dywersyfikacji.\n";
            //int v;
            diversification = !diversification;
            //this->generateRandom(v);
            break;
        case 4:
            std::cout << "Prosze wybrac sasiedztwo.\n"
                    "0 - swap\n"
                    "1 - insert\n"
                    "2 - invert\n";
            int neighbours;
            std::cin >> neighbours;
            switch(neighbours){
                case 0:
                    currentNeighbourhood = SWAP;
                    break;
                case 1:
                    currentNeighbourhood = INSERT;
                    break;
                case 2:
                    currentNeighbourhood = INVERT;
                    break;
            }
            break;
        case 5:
            std::cout << "\n########################################\n" << this->tabuSearch();
            break;
        case 6:
            return;
        case 7:
            std::cout << "Prosze podac liczbe miast.\n";
            int v;
            std::cin >> v;
            this->generateRandom(v);
            break;
        case 8:
            std::cout << "\n########################################\n" << this->bruteForce();
            break;
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
        case 0:
            timer->start();
            this->bruteForce();
            timer->stop();
            break;
        case 1:
            timer->start();
            this->tabuSearch();
            timer->stop();
            break;
    }


    //return (time_end - time_start) / std::chrono::nanoseconds(1);
    return timer->get();
}

TravellingSalesmanProblem::TravellingSalesmanProblem() {

}

void TravellingSalesmanProblem::setDiversification(bool diversification) {
    TravellingSalesmanProblem::diversification = diversification;
}

void TravellingSalesmanProblem::setStopCriterium(double stopCriterium) {
    TravellingSalesmanProblem::stopCriterium = stopCriterium;
}

void TravellingSalesmanProblem::setCurrentNeighbourhood(TravellingSalesmanProblem::neighbourhood currentNeighbourhood) {
    TravellingSalesmanProblem::currentNeighbourhood = currentNeighbourhood;
}

void TravellingSalesmanProblem::setNumberOfIterations(int numberOfIterations) {
    TravellingSalesmanProblem::numberOfIterations = numberOfIterations;
}
