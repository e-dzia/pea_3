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
    Timer t;
    t.start();
    bool visited[numberOfCities];
    int path[numberOfCities];
    int length = 0;

    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
        path[i] = -1;
    }
    int start = 0;
    int i = start;

    while(!allVisited(visited)){
        visited[i] = true;
        int min = INT32_MAX;
        int position = -1;
        for (int j = 0; j < numberOfCities; j++){
            if (!visited[j] && gm.getEdgeLength(i,j) != -1 && gm.getEdgeLength(i,j)  < min){
                min = gm.getEdgeLength(i,j);
                position = j;
            }
        }
        if (min != INT32_MAX)
            length += min;
        else {
            length += gm.getEdgeLength(i,start);
        }
        path[i] = position;
        i = position;
    }
    int *permutation = new int[numberOfCities];
    int * result_permutation = new int[numberOfCities];
    int j = start;
    permutation[0] = start;
    result_permutation[0] = start;
    //std::string result = "";
    for (int i = 0; i < numberOfCities-1; i++){
        permutation[i+1] = path[j];
        result_permutation[i+1] = path[j];
        j = path[j];
        //int temp = permutation[i];
    }

    for (int i = 1; i < numberOfCities-1 && t.getWithoutStopping() < stopCriterium; i++){
        for (int j = 1; j < numberOfCities -1 && t.getWithoutStopping() < stopCriterium; j++) {
            if (i != j) {
                switch(currentNeighbourhood){
                    case SWAP:
                        swap(permutation, i, j);
                        break;
                    case INSERT:
                        insert(permutation, i, j);
                        break;
                    case INVERT:
                        invert(permutation, i, j);
                        break;
                }
                int tmp = countPath(permutation);
                if (tmp <= length) {
                    length = tmp;
                    for (int i = 0; i < numberOfCities; i++) {
                        result_permutation[i] = permutation[i];
                    }
                } else {
                    switch(currentNeighbourhood){
                        case SWAP:
                            swap(permutation, i, j);
                            break;
                        case INSERT:
                            insert(permutation, j, i);
                            break;
                        case INVERT:
                            invert(permutation, i, j);
                            break;
                    }
                }
            }
        }
    }

    /*for (int i = 0; i < numberOfCities; i++){
        std::cout << permutation[i] << " ";
    }
    std::cout << std::endl;

    invert(permutation,0,3);

    for (int i = 0; i < numberOfCities; i++){
        std::cout << permutation[i] << " ";
    }
    std::cout << std::endl;

    invert(permutation,3,0);

    for (int i = 0; i < numberOfCities; i++){
        std::cout << permutation[i] << " ";
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
    return ss.str();
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

void TravellingSalesmanProblem::loadFromFile(std::string filename) {
    std::ifstream fin;
    fin.open(filename.c_str());
    fin >> numberOfCities;
    gm.createMatrix(numberOfCities);
    for (int i = 0; i < numberOfCities; i++){
        for (int j = 0; j < numberOfCities; j++){
            int length;
            fin >> length;
            if (length == -1) length = 0;
            gm.setEdge(i,j,length);
        }
    }
}

void TravellingSalesmanProblem::generateRandom(int size) {
    numberOfCities = size;
    gm.createRandom(numberOfCities,100);
    //gm.makeBothWaysEqual();
}

bool TravellingSalesmanProblem::allVisited(bool *visited) {
    for (int i = 0; i < numberOfCities; i++){
        if (!visited[i]) return false;
    }
    return true;
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
            this->loadFromFile(file_name);
            break;
        case 2:
            std::cout << "Prosze podac kryterium stopu (w sekundach).\n";
            //int v;
            std::cin >> stopCriterium;
            //this->generateRandom(v);
            break;
        case 3:
            std::cout << "Prosze podac wlaczenie/wylaczenie dywersyfikacji.\n"
                    "Wlaczenie - 1; Wylaczenie - 0.\n";
            //int v;
            std::cin >> diversification;
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
