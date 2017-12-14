#include "TravellingSalesmanProblem.h"
#include <sstream>
#include <chrono>
#include <cmath>

std::string TravellingSalesmanProblem::tabuSearch() {
    tabuList.clear();
    Timer t;
    t.start();
    int *current_permutation = new int[numberOfCities]; //przechowuje aktualnie analizowana permutacje miast

    //pierwsza permutacja - algorytm zachlanny
    int length = beginning(current_permutation); //przechowuje najlepsza znana dotychczas dlugosc

    int *result_permutation = new int[numberOfCities]; //przechowuje najlepsza znana permutacje miast
    for (int i = 0; i < numberOfCities; i++){
        result_permutation[i] = current_permutation[i];
    }

    //int nOI = numberOfIterations; //liczba iteracji glownej petli - uzywana do debuggowania
    int numberOfIterationsWithoutChange = numberOfCities*3; //maksymalna liczba iteracji bez zmiany - dywersyfikacja
    while (/*nOI-->0 && */t.getWithoutStopping() < stopCriterium){
        //nowe rozwiazanie - najlepsze z sasiedztwa
        TabuElement te = newSolution(current_permutation); //zwraca zmienione i, j

        int tmp = countPath(current_permutation); //dlugosc znalezionej trasy

        if (tmp < length){ //jesli rozwiazanie jest lepsze, to je zmienia na nowe
            numberOfIterationsWithoutChange = numberOfCities*3;
            length = tmp;
            for (int i = 0; i < numberOfCities; i++){
                result_permutation[i] = current_permutation[i];
            }
        }

        //aktualizacja listy tabu: zmniejszenie kadencji i usuniecie zbednych pozycji
        for(auto it = tabuList.begin(); it != tabuList.end(); ++it) {
            it->lifetime--;

            if (it->lifetime == 0){
                tabuList.erase(it);
                --it;
            }
        }

        //dodaj nowe elementy do listy tabu
        if (tabuList.size() < numberOfCities*2){ //maksymalny rozmiar listy tabu
            te.lifetime = numberOfCities*2;
            tabuList.push_back(te);
        }


        //Dywersyfikacja; jeśli lepsze po restarcie, to podmien
        if(diversification && CriticalEvent(numberOfIterationsWithoutChange)){
            restart(current_permutation);
            tmp = countPath(current_permutation);

            numberOfIterationsWithoutChange = numberOfCities*3;
            if (tmp <= length){
                length = tmp;
                for (int i = 0; i < numberOfCities; i++){
                    result_permutation[i] = current_permutation[i];
                }
            }
        }
        numberOfIterationsWithoutChange--;
    }

    std::stringstream ss;
    ss << "Algorytm tabu search.\nWynik " << std::endl;
    for (int i = 0; i < numberOfCities; i++){
        ss << result_permutation[i] << " ";
    }
    ss << ": " << length << std::endl;

    delete[] current_permutation;
    delete[] result_permutation;
    tabuList.clear();
    return ss.str();
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
    TabuElement te;
    int min = INT32_MAX;
    int *current_permutation = new int[numberOfCities]; //obecna permutacja
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

                if (tmp <= min) {
                    te.i = i; te.j = j;
                    min = tmp;
                    for (int k = 0; k < numberOfCities; k++) {
                        result_permutation[k] = current_permutation[k]; //tu trafia najlepsza znaleziona permutacja z okolicy
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
    return te;
}

bool TravellingSalesmanProblem::inTabuList(int i, int j) {
    for (auto &it : tabuList) {
        if ((it.i == i && it.j == j)||(it.i == j && it.j == i)) //przypadki, w ktorych nie zamieniamy
            return true;
    }
    return false;
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

bool TravellingSalesmanProblem::searchInFile(std::ifstream &fin, std::string toFind){
    std::string tmp;
    std::string toFind2;
    if (toFind.substr(toFind.size()-1,1)==":"){
        toFind2 = toFind.substr(0,toFind.size()-1);
    }
    int check = 20;
    do {
        fin >> tmp;
        check--;
    }
    while (tmp != toFind && tmp != toFind2  && check > 0);
    if (check == 0) return false;
    return true;
}

bool TravellingSalesmanProblem::loadFromFile(std::string filename) {
    std::ifstream fin;
    fin.open(filename.c_str());
    if (filename.find(".atsp")!=std::string::npos){ //atsp
        std::string tmp;
        if (!searchInFile(fin,"DIMENSION:")) return false;

        fin >> numberOfCities;
        gm.createMatrix(numberOfCities);

        if (!searchInFile(fin,"EDGE_WEIGHT_TYPE:")) return false;
        if (!searchInFile(fin,"EXPLICIT")) return false;
        if (!searchInFile(fin,"EDGE_WEIGHT_FORMAT:")) return false;
        if (!searchInFile(fin,"FULL_MATRIX")) return false;

        if (!searchInFile(fin,"EDGE_WEIGHT_SECTION")) return false;
        for (int i = 0; i < numberOfCities; i++){
            for (int j = 0; j < numberOfCities; j++){
                int length;
                fin >> length;
                gm.setEdge(i,j,length);
            }
        }
        return true;
    }
    else if (filename.find(".tsp")!=std::string::npos) { //tsp
        std::string tmp;
        if (!searchInFile(fin, "DIMENSION:")) return false;

        fin >> numberOfCities;
        gm.createMatrix(numberOfCities);

        if (!searchInFile(fin, "EDGE_WEIGHT_TYPE:")) return false;
        fin >> tmp;
        if (tmp == ":") fin >> tmp;
        if (tmp == "EXPLICIT") {
            if (!searchInFile(fin, "EDGE_WEIGHT_FORMAT:")) return false;
            fin >> tmp;
            if (tmp.find("LOWER_DIAG_ROW") != std::string::npos) {
                if (!searchInFile(fin, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = 0; j <= i; j++) {
                        int length;
                        fin >> length;
                        gm.setEdge(i, j, length);
                        gm.setEdge(j, i, length);
                    }
                }
                return true;
            } else if (tmp.find("FULL_MATRIX") != std::string::npos) {
                if (!searchInFile(fin, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = 0; j < numberOfCities; j++) {
                        int length;
                        fin >> length;
                        gm.setEdge(i, j, length);
                    }
                }
                return true;
            } else if (tmp.find("UPPER_DIAG_ROW") != std::string::npos) {
                if (!searchInFile(fin, "EDGE_WEIGHT_SECTION")) return false;

                for (int i = 0; i < numberOfCities; i++) {
                    for (int j = i; j < numberOfCities; j++) {
                        int length;
                        fin >> length;
                        gm.setEdge(i, j, length);
                        gm.setEdge(j, i, length);
                    }
                }
                return true;
            } else return false;
        } else if (tmp == "EUC_2D") {
            if (!searchInFile(fin, "NODE_COORD_SECTION")) return false;

            double **eucCoord = new double *[numberOfCities];
            for (int i = 0; i < numberOfCities; i++) {
                eucCoord[i] = new double[2];
                int n;
                fin >> n;
                fin >> eucCoord[i][0] >> eucCoord[i][1];
            }

            for (int i = 0; i < numberOfCities; i++) {
                for (int j = 0; j <= i; j++) {
                    double xDistance = eucCoord[i][0] - eucCoord[j][0];
                    double yDistance = eucCoord[i][1] - eucCoord[j][1];
                    int distance = (int) sqrt(xDistance * xDistance + yDistance * yDistance);
                    gm.setEdge(i, j, distance);
                    gm.setEdge(j, i, distance);
                }
            }
            delete[] eucCoord;
            return true;
        }
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
