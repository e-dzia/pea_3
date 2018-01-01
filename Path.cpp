//
// Created by Edzia on 2018-01-01.
//

#include "Path.h"

void Path::swap(int left, int right) {
    if (right == left) return;
    int tmp = permutation[left];
    permutation[left] = permutation[right];
    permutation[right] = tmp;
}

void Path::insert(int left, int right) {
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

void Path::invert(int left, int right) {
    if (right == left) return;
    if (right < left){int tmp = left; left = right; right = tmp;}
    for (int i = 0; i < (right - left +1)/2; i++){
        swap(left+i,right-i);
    }
}

void Path::countPath() {
    length = 0;
    int end;
    for (int i = 1; i < numberOfCities; i++){
        length += citiesDistances.getEdgeLength(permutation[i-1],permutation[i]);
        end = i;
    }
    length += citiesDistances.getEdgeLength(permutation[end],permutation[0]);
}

void Path::setPath(const int path[numberOfCities]) {
    for (int i = 0; i < numberOfCities; i++){
        permutation[i] = path[i];
    }
    countPath();
}

Path::~Path() {
    delete[] permutation;
}

void Path::setPath(const Path p) {
    for (int i = 0; i < numberOfCities; i++){
        permutation[i] = p[i];
    }
    countPath();
}

int Path::operator[](int i) const {
    return permutation[i];
}

Path &Path::operator=(Path p) {
    this->setPath(p);
    return *this;
}

void Path::setRandom() {
    bool* visited = new bool[numberOfCities];
    for (int i = 0; i < numberOfCities; i++){
        visited[i] = false;
    }
    visited[0] = true;
    permutation[0] = 0;
    for (int i = 1; i < numberOfCities; i++){
        do{
            permutation[i] = rand()%numberOfCities;
        }while(visited[permutation[i]]);
        visited[permutation[i]] = true;
    }
    countPath();
    delete[] visited;
}

bool Path::isShorter(Path p) {
    return this->length < p.getLength();
}

int Path::getLength() const {
    return length;
}

void Path::setCitiesDistances(const GraphMatrix &citiesDistances) {
    Path::citiesDistances = citiesDistances;
    numberOfCities = citiesDistances.getNumberOfVertexes();
}

void Path::print(std::ostream &str) const {
    for (int i = 0; i < numberOfCities; i++){
        str << permutation[i] << " ";
    }
    str << "\n";
}

Path::Path() = default;

